

#ifndef _LINUX_KEY_H
#define _LINUX_KEY_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/rcupdate.h>
#include <linux/sysctl.h>
#include <linux/rwsem.h>
#include <linux/atomic.h>
#include <linux/assoc_array.h>

#ifdef __KERNEL__
#include <linux/uidgid.h>


typedef int32_t key_serial_t;


typedef uint32_t key_perm_t;

struct key;

#ifdef CONFIG_KEYS

#undef KEY_DEBUGGING

#define KEY_POS_VIEW	0x01000000	
#define KEY_POS_READ	0x02000000	
#define KEY_POS_WRITE	0x04000000	
#define KEY_POS_SEARCH	0x08000000	
#define KEY_POS_LINK	0x10000000	
#define KEY_POS_SETATTR	0x20000000	
#define KEY_POS_ALL	0x3f000000

#define KEY_USR_VIEW	0x00010000	
#define KEY_USR_READ	0x00020000
#define KEY_USR_WRITE	0x00040000
#define KEY_USR_SEARCH	0x00080000
#define KEY_USR_LINK	0x00100000
#define KEY_USR_SETATTR	0x00200000
#define KEY_USR_ALL	0x003f0000

#define KEY_GRP_VIEW	0x00000100	
#define KEY_GRP_READ	0x00000200
#define KEY_GRP_WRITE	0x00000400
#define KEY_GRP_SEARCH	0x00000800
#define KEY_GRP_LINK	0x00001000
#define KEY_GRP_SETATTR	0x00002000
#define KEY_GRP_ALL	0x00003f00

#define KEY_OTH_VIEW	0x00000001	
#define KEY_OTH_READ	0x00000002
#define KEY_OTH_WRITE	0x00000004
#define KEY_OTH_SEARCH	0x00000008
#define KEY_OTH_LINK	0x00000010
#define KEY_OTH_SETATTR	0x00000020
#define KEY_OTH_ALL	0x0000003f

#define KEY_PERM_UNDEF	0xffffffff

struct seq_file;
struct user_struct;
struct signal_struct;
struct cred;

struct key_type;
struct key_owner;
struct keyring_list;
struct keyring_name;

struct keyring_index_key {
	struct key_type		*type;
	const char		*description;
	size_t			desc_len;
};

union key_payload {
	void __rcu		*rcu_data0;
	void			*data[4];
};



typedef struct __key_reference_with_attributes *key_ref_t;

static inline key_ref_t make_key_ref(const struct key *key,
				     bool possession)
{
	return (key_ref_t) ((unsigned long) key | possession);
}

static inline struct key *key_ref_to_ptr(const key_ref_t key_ref)
{
	return (struct key *) ((unsigned long) key_ref & ~1UL);
}

static inline bool is_key_possessed(const key_ref_t key_ref)
{
	return (unsigned long) key_ref & 1UL;
}

enum key_state {
	KEY_IS_UNINSTANTIATED,
	KEY_IS_POSITIVE,		
};



struct key {
	atomic_t		usage;		
	key_serial_t		serial;		
	union {
		struct list_head graveyard_link;
		struct rb_node	serial_node;
	};
	struct rw_semaphore	sem;		
	struct key_user		*user;		
	void			*security;	
	union {
		time_t		expiry;		
		time_t		revoked_at;	
	};
	time_t			last_used_at;	
	kuid_t			uid;
	kgid_t			gid;
	key_perm_t		perm;		
	unsigned short		quotalen;	
	unsigned short		datalen;	
	short			state;		

#ifdef KEY_DEBUGGING
	unsigned		magic;
#define KEY_DEBUG_MAGIC		0x18273645u
#define KEY_DEBUG_MAGIC_X	0xf8e9dacbu
#endif

	unsigned long		flags;		
#define KEY_FLAG_DEAD		0	
#define KEY_FLAG_REVOKED	1	
#define KEY_FLAG_IN_QUOTA	2	
#define KEY_FLAG_USER_CONSTRUCT	3	
#define KEY_FLAG_ROOT_CAN_CLEAR	4	
#define KEY_FLAG_INVALIDATED	5	
#define KEY_FLAG_BUILTIN	6	
#define KEY_FLAG_ROOT_CAN_INVAL	7	
#define KEY_FLAG_KEEP		8	
#define KEY_FLAG_UID_KEYRING	9	

	
	union {
		struct keyring_index_key index_key;
		struct {
			struct key_type	*type;		
			char		*description;
		};
	};

	
	union {
		union key_payload payload;
		struct {
			
			struct list_head name_link;
			struct assoc_array keys;
		};
	};

	
	int (*restrict_link)(struct key *keyring,
			     const struct key_type *type,
			     const union key_payload *payload);
};

extern struct key *key_alloc(struct key_type *type,
			     const char *desc,
			     kuid_t uid, kgid_t gid,
			     const struct cred *cred,
			     key_perm_t perm,
			     unsigned long flags,
			     int (*restrict_link)(struct key *,
						  const struct key_type *,
						  const union key_payload *));


#define KEY_ALLOC_IN_QUOTA		0x0000	
#define KEY_ALLOC_QUOTA_OVERRUN		0x0001	
#define KEY_ALLOC_NOT_IN_QUOTA		0x0002	
#define KEY_ALLOC_BUILT_IN		0x0004	
#define KEY_ALLOC_BYPASS_RESTRICTION	0x0008	
#define KEY_ALLOC_UID_KEYRING		0x0010	

extern void key_revoke(struct key *key);
extern void key_invalidate(struct key *key);
extern void key_put(struct key *key);

static inline struct key *__key_get(struct key *key)
{
	atomic_inc(&key->usage);
	return key;
}

static inline struct key *key_get(struct key *key)
{
	return key ? __key_get(key) : key;
}

static inline void key_ref_put(key_ref_t key_ref)
{
	key_put(key_ref_to_ptr(key_ref));
}

extern struct key *request_key(struct key_type *type,
			       const char *description,
			       const char *callout_info);

extern struct key *request_key_with_auxdata(struct key_type *type,
					    const char *description,
					    const void *callout_info,
					    size_t callout_len,
					    void *aux);

extern struct key *request_key_async(struct key_type *type,
				     const char *description,
				     const void *callout_info,
				     size_t callout_len);

extern struct key *request_key_async_with_auxdata(struct key_type *type,
						  const char *description,
						  const void *callout_info,
						  size_t callout_len,
						  void *aux);

extern int wait_for_key_construction(struct key *key, bool intr);

extern int key_validate(const struct key *key);

extern key_ref_t key_create_or_update(key_ref_t keyring,
				      const char *type,
				      const char *description,
				      const void *payload,
				      size_t plen,
				      key_perm_t perm,
				      unsigned long flags);

extern int key_update(key_ref_t key,
		      const void *payload,
		      size_t plen);

extern int key_link(struct key *keyring,
		    struct key *key);

extern int key_unlink(struct key *keyring,
		      struct key *key);

extern struct key *keyring_alloc(const char *description, kuid_t uid, kgid_t gid,
				 const struct cred *cred,
				 key_perm_t perm,
				 unsigned long flags,
				 int (*restrict_link)(struct key *,
						      const struct key_type *,
						      const union key_payload *),
				 struct key *dest);

extern int restrict_link_reject(struct key *keyring,
				const struct key_type *type,
				const union key_payload *payload);

extern int keyring_clear(struct key *keyring);

extern key_ref_t keyring_search(key_ref_t keyring,
				struct key_type *type,
				const char *description);

extern int keyring_add_key(struct key *keyring,
			   struct key *key);

extern struct key *key_lookup(key_serial_t id);

static inline key_serial_t key_serial(const struct key *key)
{
	return key ? key->serial : 0;
}

extern void key_set_timeout(struct key *, unsigned);


#define	KEY_NEED_VIEW	0x01	
#define	KEY_NEED_READ	0x02	
#define	KEY_NEED_WRITE	0x04	
#define	KEY_NEED_SEARCH	0x08	
#define	KEY_NEED_LINK	0x10	
#define	KEY_NEED_SETATTR 0x20	
#define	KEY_NEED_ALL	0x3f	

static inline short key_read_state(const struct key *key)
{
	
	return smp_load_acquire(&key->state);
}


static inline bool key_is_positive(const struct key *key)
{
	return key_read_state(key) == KEY_IS_POSITIVE;
}

static inline bool key_is_negative(const struct key *key)
{
	return key_read_state(key) < 0;
}

#define dereference_key_rcu(KEY)					\
	(rcu_dereference((KEY)->payload.rcu_data0))

#define dereference_key_locked(KEY)					\
	(rcu_dereference_protected((KEY)->payload.rcu_data0,		\
				   rwsem_is_locked(&((struct key *)(KEY))->sem)))

#define rcu_assign_keypointer(KEY, PAYLOAD)				\
do {									\
	rcu_assign_pointer((KEY)->payload.rcu_data0, (PAYLOAD));	\
} while (0)

#ifdef CONFIG_SYSCTL
extern struct ctl_table key_sysctls[];
#endif

extern int install_thread_keyring_to_cred(struct cred *cred);
extern void key_fsuid_changed(struct task_struct *tsk);
extern void key_fsgid_changed(struct task_struct *tsk);
extern void key_init(void);

#else 

#define key_validate(k)			0
#define key_serial(k)			0
#define key_get(k) 			({ NULL; })
#define key_revoke(k)			do { } while(0)
#define key_invalidate(k)		do { } while(0)
#define key_put(k)			do { } while(0)
#define key_ref_put(k)			do { } while(0)
#define make_key_ref(k, p)		NULL
#define key_ref_to_ptr(k)		NULL
#define is_key_possessed(k)		0
#define key_fsuid_changed(t)		do { } while(0)
#define key_fsgid_changed(t)		do { } while(0)
#define key_init()			do { } while(0)

#endif 
#endif 
#endif 
