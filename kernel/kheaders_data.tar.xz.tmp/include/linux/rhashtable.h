

#ifndef _LINUX_RHASHTABLE_H
#define _LINUX_RHASHTABLE_H

#include <linux/atomic.h>
#include <linux/compiler.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/jhash.h>
#include <linux/list_nulls.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>


#define RHT_BASE_BITS		4
#define RHT_HASH_BITS		27
#define RHT_BASE_SHIFT		RHT_HASH_BITS


#define RHT_HASH_RESERVED_SPACE	(RHT_BASE_BITS + 1)

struct rhash_head {
	struct rhash_head __rcu		*next;
};

struct rhlist_head {
	struct rhash_head		rhead;
	struct rhlist_head __rcu	*next;
};


struct bucket_table {
	unsigned int		size;
	unsigned int		rehash;
	u32			hash_rnd;
	unsigned int		locks_mask;
	spinlock_t		*locks;
	struct list_head	walkers;
	struct rcu_head		rcu;

	struct bucket_table __rcu *future_tbl;

	struct rhash_head __rcu	*buckets[] ____cacheline_aligned_in_smp;
};


struct rhashtable_compare_arg {
	struct rhashtable *ht;
	const void *key;
};

typedef u32 (*rht_hashfn_t)(const void *data, u32 len, u32 seed);
typedef u32 (*rht_obj_hashfn_t)(const void *data, u32 len, u32 seed);
typedef int (*rht_obj_cmpfn_t)(struct rhashtable_compare_arg *arg,
			       const void *obj);

struct rhashtable;


struct rhashtable_params {
	size_t			nelem_hint;
	size_t			key_len;
	size_t			key_offset;
	size_t			head_offset;
	unsigned int		insecure_max_entries;
	unsigned int		max_size;
	unsigned int		min_size;
	u32			nulls_base;
	bool			insecure_elasticity;
	bool			automatic_shrinking;
	size_t			locks_mul;
	rht_hashfn_t		hashfn;
	rht_obj_hashfn_t	obj_hashfn;
	rht_obj_cmpfn_t		obj_cmpfn;
};


struct rhashtable {
	struct bucket_table __rcu	*tbl;
	unsigned int			key_len;
	unsigned int			elasticity;
	struct rhashtable_params	p;
	bool				rhlist;
	struct work_struct		run_work;
	struct mutex                    mutex;
	spinlock_t			lock;
	atomic_t			nelems;
};


struct rhltable {
	struct rhashtable ht;
};


struct rhashtable_walker {
	struct list_head list;
	struct bucket_table *tbl;
};


struct rhashtable_iter {
	struct rhashtable *ht;
	struct rhash_head *p;
	struct rhlist_head *list;
	struct rhashtable_walker walker;
	unsigned int slot;
	unsigned int skip;
};

static inline unsigned long rht_marker(const struct rhashtable *ht, u32 hash)
{
	return NULLS_MARKER(ht->p.nulls_base + hash);
}

#define INIT_RHT_NULLS_HEAD(ptr, ht, hash) \
	((ptr) = (typeof(ptr)) rht_marker(ht, hash))

static inline bool rht_is_a_nulls(const struct rhash_head *ptr)
{
	return ((unsigned long) ptr & 1);
}

static inline unsigned long rht_get_nulls_value(const struct rhash_head *ptr)
{
	return ((unsigned long) ptr) >> 1;
}

static inline void *rht_obj(const struct rhashtable *ht,
			    const struct rhash_head *he)
{
	return (char *)he - ht->p.head_offset;
}

static inline unsigned int rht_bucket_index(const struct bucket_table *tbl,
					    unsigned int hash)
{
	return (hash >> RHT_HASH_RESERVED_SPACE) & (tbl->size - 1);
}

static inline unsigned int rht_key_hashfn(
	struct rhashtable *ht, const struct bucket_table *tbl,
	const void *key, const struct rhashtable_params params)
{
	unsigned int hash;

	
	if (!__builtin_constant_p(params.key_len))
		hash = ht->p.hashfn(key, ht->key_len, tbl->hash_rnd);
	else if (params.key_len) {
		unsigned int key_len = params.key_len;

		if (params.hashfn)
			hash = params.hashfn(key, key_len, tbl->hash_rnd);
		else if (key_len & (sizeof(u32) - 1))
			hash = jhash(key, key_len, tbl->hash_rnd);
		else
			hash = jhash2(key, key_len / sizeof(u32),
				      tbl->hash_rnd);
	} else {
		unsigned int key_len = ht->p.key_len;

		if (params.hashfn)
			hash = params.hashfn(key, key_len, tbl->hash_rnd);
		else
			hash = jhash(key, key_len, tbl->hash_rnd);
	}

	return rht_bucket_index(tbl, hash);
}

static inline unsigned int rht_head_hashfn(
	struct rhashtable *ht, const struct bucket_table *tbl,
	const struct rhash_head *he, const struct rhashtable_params params)
{
	const char *ptr = rht_obj(ht, he);

	return likely(params.obj_hashfn) ?
	       rht_bucket_index(tbl, params.obj_hashfn(ptr, params.key_len ?:
							    ht->p.key_len,
						       tbl->hash_rnd)) :
	       rht_key_hashfn(ht, tbl, ptr + params.key_offset, params);
}


static inline bool rht_grow_above_75(const struct rhashtable *ht,
				     const struct bucket_table *tbl)
{
	
	return atomic_read(&ht->nelems) > (tbl->size / 4 * 3) &&
	       (!ht->p.max_size || tbl->size < ht->p.max_size);
}


static inline bool rht_shrink_below_30(const struct rhashtable *ht,
				       const struct bucket_table *tbl)
{
	
	return atomic_read(&ht->nelems) < (tbl->size * 3 / 10) &&
	       tbl->size > ht->p.min_size;
}


static inline bool rht_grow_above_100(const struct rhashtable *ht,
				      const struct bucket_table *tbl)
{
	return atomic_read(&ht->nelems) > tbl->size &&
		(!ht->p.max_size || tbl->size < ht->p.max_size);
}


static inline bool rht_grow_above_max(const struct rhashtable *ht,
				      const struct bucket_table *tbl)
{
	return ht->p.insecure_max_entries &&
	       atomic_read(&ht->nelems) >= ht->p.insecure_max_entries;
}


static inline spinlock_t *rht_bucket_lock(const struct bucket_table *tbl,
					  unsigned int hash)
{
	return &tbl->locks[hash & tbl->locks_mask];
}

#ifdef CONFIG_PROVE_LOCKING
int lockdep_rht_mutex_is_held(struct rhashtable *ht);
int lockdep_rht_bucket_is_held(const struct bucket_table *tbl, u32 hash);
#else
static inline int lockdep_rht_mutex_is_held(struct rhashtable *ht)
{
	return 1;
}

static inline int lockdep_rht_bucket_is_held(const struct bucket_table *tbl,
					     u32 hash)
{
	return 1;
}
#endif 

int rhashtable_init(struct rhashtable *ht,
		    const struct rhashtable_params *params);
int rhltable_init(struct rhltable *hlt,
		  const struct rhashtable_params *params);

void *rhashtable_insert_slow(struct rhashtable *ht, const void *key,
			     struct rhash_head *obj);

void rhashtable_walk_enter(struct rhashtable *ht,
			   struct rhashtable_iter *iter);
void rhashtable_walk_exit(struct rhashtable_iter *iter);
int rhashtable_walk_start(struct rhashtable_iter *iter) __acquires(RCU);
void *rhashtable_walk_next(struct rhashtable_iter *iter);
void rhashtable_walk_stop(struct rhashtable_iter *iter) __releases(RCU);

void rhashtable_free_and_destroy(struct rhashtable *ht,
				 void (*free_fn)(void *ptr, void *arg),
				 void *arg);
void rhashtable_destroy(struct rhashtable *ht);

#define rht_dereference(p, ht) \
	rcu_dereference_protected(p, lockdep_rht_mutex_is_held(ht))

#define rht_dereference_rcu(p, ht) \
	rcu_dereference_check(p, lockdep_rht_mutex_is_held(ht))

#define rht_dereference_bucket(p, tbl, hash) \
	rcu_dereference_protected(p, lockdep_rht_bucket_is_held(tbl, hash))

#define rht_dereference_bucket_rcu(p, tbl, hash) \
	rcu_dereference_check(p, lockdep_rht_bucket_is_held(tbl, hash))

#define rht_entry(tpos, pos, member) \
	({ tpos = container_of(pos, typeof(*tpos), member); 1; })


#define rht_for_each_continue(pos, head, tbl, hash) \
	for (pos = rht_dereference_bucket(head, tbl, hash); \
	     !rht_is_a_nulls(pos); \
	     pos = rht_dereference_bucket((pos)->next, tbl, hash))


#define rht_for_each(pos, tbl, hash) \
	rht_for_each_continue(pos, (tbl)->buckets[hash], tbl, hash)


#define rht_for_each_entry_continue(tpos, pos, head, tbl, hash, member)	\
	for (pos = rht_dereference_bucket(head, tbl, hash);		\
	     (!rht_is_a_nulls(pos)) && rht_entry(tpos, pos, member);	\
	     pos = rht_dereference_bucket((pos)->next, tbl, hash))


#define rht_for_each_entry(tpos, pos, tbl, hash, member)		\
	rht_for_each_entry_continue(tpos, pos, (tbl)->buckets[hash],	\
				    tbl, hash, member)


#define rht_for_each_entry_safe(tpos, pos, next, tbl, hash, member)	    \
	for (pos = rht_dereference_bucket((tbl)->buckets[hash], tbl, hash), \
	     next = !rht_is_a_nulls(pos) ?				    \
		       rht_dereference_bucket(pos->next, tbl, hash) : NULL; \
	     (!rht_is_a_nulls(pos)) && rht_entry(tpos, pos, member);	    \
	     pos = next,						    \
	     next = !rht_is_a_nulls(pos) ?				    \
		       rht_dereference_bucket(pos->next, tbl, hash) : NULL)


#define rht_for_each_rcu_continue(pos, head, tbl, hash)			\
	for (({barrier(); }),						\
	     pos = rht_dereference_bucket_rcu(head, tbl, hash);		\
	     !rht_is_a_nulls(pos);					\
	     pos = rcu_dereference_raw(pos->next))


#define rht_for_each_rcu(pos, tbl, hash)				\
	rht_for_each_rcu_continue(pos, (tbl)->buckets[hash], tbl, hash)


#define rht_for_each_entry_rcu_continue(tpos, pos, head, tbl, hash, member) \
	for (({barrier(); }),						    \
	     pos = rht_dereference_bucket_rcu(head, tbl, hash);		    \
	     (!rht_is_a_nulls(pos)) && rht_entry(tpos, pos, member);	    \
	     pos = rht_dereference_bucket_rcu(pos->next, tbl, hash))


#define rht_for_each_entry_rcu(tpos, pos, tbl, hash, member)		\
	rht_for_each_entry_rcu_continue(tpos, pos, (tbl)->buckets[hash],\
					tbl, hash, member)


#define rhl_for_each_rcu(pos, list)					\
	for (pos = list; pos; pos = rcu_dereference_raw(pos->next))


#define rhl_for_each_entry_rcu(tpos, pos, list, member)			\
	for (pos = list; pos && rht_entry(tpos, pos, member);		\
	     pos = rcu_dereference_raw(pos->next))

static inline int rhashtable_compare(struct rhashtable_compare_arg *arg,
				     const void *obj)
{
	struct rhashtable *ht = arg->ht;
	const char *ptr = obj;

	return memcmp(ptr + ht->p.key_offset, arg->key, ht->p.key_len);
}


static inline struct rhash_head *__rhashtable_lookup(
	struct rhashtable *ht, const void *key,
	const struct rhashtable_params params)
{
	struct rhashtable_compare_arg arg = {
		.ht = ht,
		.key = key,
	};
	const struct bucket_table *tbl;
	struct rhash_head *he;
	unsigned int hash;

	tbl = rht_dereference_rcu(ht->tbl, ht);
restart:
	hash = rht_key_hashfn(ht, tbl, key, params);
	rht_for_each_rcu(he, tbl, hash) {
		if (params.obj_cmpfn ?
		    params.obj_cmpfn(&arg, rht_obj(ht, he)) :
		    rhashtable_compare(&arg, rht_obj(ht, he)))
			continue;
		return he;
	}

	
	smp_rmb();

	tbl = rht_dereference_rcu(tbl->future_tbl, ht);
	if (unlikely(tbl))
		goto restart;

	return NULL;
}


static inline void *rhashtable_lookup(
	struct rhashtable *ht, const void *key,
	const struct rhashtable_params params)
{
	struct rhash_head *he = __rhashtable_lookup(ht, key, params);

	return he ? rht_obj(ht, he) : NULL;
}


static inline void *rhashtable_lookup_fast(
	struct rhashtable *ht, const void *key,
	const struct rhashtable_params params)
{
	void *obj;

	rcu_read_lock();
	obj = rhashtable_lookup(ht, key, params);
	rcu_read_unlock();

	return obj;
}


static inline struct rhlist_head *rhltable_lookup(
	struct rhltable *hlt, const void *key,
	const struct rhashtable_params params)
{
	struct rhash_head *he = __rhashtable_lookup(&hlt->ht, key, params);

	return he ? container_of(he, struct rhlist_head, rhead) : NULL;
}


static inline void *__rhashtable_insert_fast(
	struct rhashtable *ht, const void *key, struct rhash_head *obj,
	const struct rhashtable_params params, bool rhlist)
{
	struct rhashtable_compare_arg arg = {
		.ht = ht,
		.key = key,
	};
	struct rhash_head __rcu **pprev;
	struct bucket_table *tbl;
	struct rhash_head *head;
	spinlock_t *lock;
	unsigned int hash;
	int elasticity;
	void *data;

	rcu_read_lock();

	tbl = rht_dereference_rcu(ht->tbl, ht);
	hash = rht_head_hashfn(ht, tbl, obj, params);
	lock = rht_bucket_lock(tbl, hash);
	spin_lock_bh(lock);

	if (unlikely(rht_dereference_bucket(tbl->future_tbl, tbl, hash))) {
slow_path:
		spin_unlock_bh(lock);
		rcu_read_unlock();
		return rhashtable_insert_slow(ht, key, obj);
	}

	elasticity = ht->elasticity;
	pprev = &tbl->buckets[hash];
	rht_for_each(head, tbl, hash) {
		struct rhlist_head *plist;
		struct rhlist_head *list;

		elasticity--;
		if (!key ||
		    (params.obj_cmpfn ?
		     params.obj_cmpfn(&arg, rht_obj(ht, head)) :
		     rhashtable_compare(&arg, rht_obj(ht, head)))) {
			pprev = &head->next;
			continue;
		}

		data = rht_obj(ht, head);

		if (!rhlist)
			goto out;


		list = container_of(obj, struct rhlist_head, rhead);
		plist = container_of(head, struct rhlist_head, rhead);

		RCU_INIT_POINTER(list->next, plist);
		head = rht_dereference_bucket(head->next, tbl, hash);
		RCU_INIT_POINTER(list->rhead.next, head);
		rcu_assign_pointer(*pprev, obj);

		goto good;
	}

	if (elasticity <= 0)
		goto slow_path;

	data = ERR_PTR(-E2BIG);
	if (unlikely(rht_grow_above_max(ht, tbl)))
		goto out;

	if (unlikely(rht_grow_above_100(ht, tbl)))
		goto slow_path;

	head = rht_dereference_bucket(tbl->buckets[hash], tbl, hash);

	RCU_INIT_POINTER(obj->next, head);
	if (rhlist) {
		struct rhlist_head *list;

		list = container_of(obj, struct rhlist_head, rhead);
		RCU_INIT_POINTER(list->next, NULL);
	}

	rcu_assign_pointer(tbl->buckets[hash], obj);

	atomic_inc(&ht->nelems);
	if (rht_grow_above_75(ht, tbl))
		schedule_work(&ht->run_work);

good:
	data = NULL;

out:
	spin_unlock_bh(lock);
	rcu_read_unlock();

	return data;
}


static inline int rhashtable_insert_fast(
	struct rhashtable *ht, struct rhash_head *obj,
	const struct rhashtable_params params)
{
	void *ret;

	ret = __rhashtable_insert_fast(ht, NULL, obj, params, false);
	if (IS_ERR(ret))
		return PTR_ERR(ret);

	return ret == NULL ? 0 : -EEXIST;
}


static inline int rhltable_insert_key(
	struct rhltable *hlt, const void *key, struct rhlist_head *list,
	const struct rhashtable_params params)
{
	return PTR_ERR(__rhashtable_insert_fast(&hlt->ht, key, &list->rhead,
						params, true));
}


static inline int rhltable_insert(
	struct rhltable *hlt, struct rhlist_head *list,
	const struct rhashtable_params params)
{
	const char *key = rht_obj(&hlt->ht, &list->rhead);

	key += params.key_offset;

	return rhltable_insert_key(hlt, key, list, params);
}


static inline int rhashtable_lookup_insert_fast(
	struct rhashtable *ht, struct rhash_head *obj,
	const struct rhashtable_params params)
{
	const char *key = rht_obj(ht, obj);
	void *ret;

	BUG_ON(ht->p.obj_hashfn);

	ret = __rhashtable_insert_fast(ht, key + ht->p.key_offset, obj, params,
				       false);
	if (IS_ERR(ret))
		return PTR_ERR(ret);

	return ret == NULL ? 0 : -EEXIST;
}


static inline int rhashtable_lookup_insert_key(
	struct rhashtable *ht, const void *key, struct rhash_head *obj,
	const struct rhashtable_params params)
{
	void *ret;

	BUG_ON(!ht->p.obj_hashfn || !key);

	ret = __rhashtable_insert_fast(ht, key, obj, params, false);
	if (IS_ERR(ret))
		return PTR_ERR(ret);

	return ret == NULL ? 0 : -EEXIST;
}


static inline void *rhashtable_lookup_get_insert_key(
	struct rhashtable *ht, const void *key, struct rhash_head *obj,
	const struct rhashtable_params params)
{
	BUG_ON(!ht->p.obj_hashfn || !key);

	return __rhashtable_insert_fast(ht, key, obj, params, false);
}


static inline int __rhashtable_remove_fast_one(
	struct rhashtable *ht, struct bucket_table *tbl,
	struct rhash_head *obj, const struct rhashtable_params params,
	bool rhlist)
{
	struct rhash_head __rcu **pprev;
	struct rhash_head *he;
	spinlock_t * lock;
	unsigned int hash;
	int err = -ENOENT;

	hash = rht_head_hashfn(ht, tbl, obj, params);
	lock = rht_bucket_lock(tbl, hash);

	spin_lock_bh(lock);

	pprev = &tbl->buckets[hash];
	rht_for_each(he, tbl, hash) {
		struct rhlist_head *list;

		list = container_of(he, struct rhlist_head, rhead);

		if (he != obj) {
			struct rhlist_head __rcu **lpprev;

			pprev = &he->next;

			if (!rhlist)
				continue;

			do {
				lpprev = &list->next;
				list = rht_dereference_bucket(list->next,
							      tbl, hash);
			} while (list && obj != &list->rhead);

			if (!list)
				continue;

			list = rht_dereference_bucket(list->next, tbl, hash);
			RCU_INIT_POINTER(*lpprev, list);
			err = 0;
			break;
		}

		obj = rht_dereference_bucket(obj->next, tbl, hash);
		err = 1;

		if (rhlist) {
			list = rht_dereference_bucket(list->next, tbl, hash);
			if (list) {
				RCU_INIT_POINTER(list->rhead.next, obj);
				obj = &list->rhead;
				err = 0;
			}
		}

		rcu_assign_pointer(*pprev, obj);
		break;
	}

	spin_unlock_bh(lock);

	if (err > 0) {
		atomic_dec(&ht->nelems);
		if (unlikely(ht->p.automatic_shrinking &&
			     rht_shrink_below_30(ht, tbl)))
			schedule_work(&ht->run_work);
		err = 0;
	}

	return err;
}


static inline int __rhashtable_remove_fast(
	struct rhashtable *ht, struct rhash_head *obj,
	const struct rhashtable_params params, bool rhlist)
{
	struct bucket_table *tbl;
	int err;

	rcu_read_lock();

	tbl = rht_dereference_rcu(ht->tbl, ht);

	
	while ((err = __rhashtable_remove_fast_one(ht, tbl, obj, params,
						   rhlist)) &&
	       (tbl = rht_dereference_rcu(tbl->future_tbl, ht)))
		;

	rcu_read_unlock();

	return err;
}


static inline int rhashtable_remove_fast(
	struct rhashtable *ht, struct rhash_head *obj,
	const struct rhashtable_params params)
{
	return __rhashtable_remove_fast(ht, obj, params, false);
}


static inline int rhltable_remove(
	struct rhltable *hlt, struct rhlist_head *list,
	const struct rhashtable_params params)
{
	return __rhashtable_remove_fast(&hlt->ht, &list->rhead, params, true);
}


static inline int __rhashtable_replace_fast(
	struct rhashtable *ht, struct bucket_table *tbl,
	struct rhash_head *obj_old, struct rhash_head *obj_new,
	const struct rhashtable_params params)
{
	struct rhash_head __rcu **pprev;
	struct rhash_head *he;
	spinlock_t *lock;
	unsigned int hash;
	int err = -ENOENT;

	
	hash = rht_head_hashfn(ht, tbl, obj_old, params);
	if (hash != rht_head_hashfn(ht, tbl, obj_new, params))
		return -EINVAL;

	lock = rht_bucket_lock(tbl, hash);

	spin_lock_bh(lock);

	pprev = &tbl->buckets[hash];
	rht_for_each(he, tbl, hash) {
		if (he != obj_old) {
			pprev = &he->next;
			continue;
		}

		rcu_assign_pointer(obj_new->next, obj_old->next);
		rcu_assign_pointer(*pprev, obj_new);
		err = 0;
		break;
	}

	spin_unlock_bh(lock);

	return err;
}


static inline int rhashtable_replace_fast(
	struct rhashtable *ht, struct rhash_head *obj_old,
	struct rhash_head *obj_new,
	const struct rhashtable_params params)
{
	struct bucket_table *tbl;
	int err;

	rcu_read_lock();

	tbl = rht_dereference_rcu(ht->tbl, ht);

	
	while ((err = __rhashtable_replace_fast(ht, tbl, obj_old,
						obj_new, params)) &&
	       (tbl = rht_dereference_rcu(tbl->future_tbl, ht)))
		;

	rcu_read_unlock();

	return err;
}


static inline int rhashtable_walk_init(struct rhashtable *ht,
				       struct rhashtable_iter *iter, gfp_t gfp)
{
	rhashtable_walk_enter(ht, iter);
	return 0;
}


static inline void rhltable_walk_enter(struct rhltable *hlt,
				       struct rhashtable_iter *iter)
{
	return rhashtable_walk_enter(&hlt->ht, iter);
}


static inline void rhltable_free_and_destroy(struct rhltable *hlt,
					     void (*free_fn)(void *ptr,
							     void *arg),
					     void *arg)
{
	return rhashtable_free_and_destroy(&hlt->ht, free_fn, arg);
}

static inline void rhltable_destroy(struct rhltable *hlt)
{
	return rhltable_free_and_destroy(hlt, NULL, NULL);
}

#endif 
