



#ifndef _NETLABEL_H
#define _NETLABEL_H

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <net/netlink.h>
#include <net/request_sock.h>
#include <linux/atomic.h>

struct cipso_v4_doi;
struct calipso_doi;






#define NETLBL_PROTO_VERSION            3


#define NETLBL_NLTYPE_NONE              0
#define NETLBL_NLTYPE_MGMT              1
#define NETLBL_NLTYPE_MGMT_NAME         "NLBL_MGMT"
#define NETLBL_NLTYPE_RIPSO             2
#define NETLBL_NLTYPE_RIPSO_NAME        "NLBL_RIPSO"
#define NETLBL_NLTYPE_CIPSOV4           3
#define NETLBL_NLTYPE_CIPSOV4_NAME      "NLBL_CIPSOv4"
#define NETLBL_NLTYPE_CIPSOV6           4
#define NETLBL_NLTYPE_CIPSOV6_NAME      "NLBL_CIPSOv6"
#define NETLBL_NLTYPE_UNLABELED         5
#define NETLBL_NLTYPE_UNLABELED_NAME    "NLBL_UNLBL"
#define NETLBL_NLTYPE_ADDRSELECT        6
#define NETLBL_NLTYPE_ADDRSELECT_NAME   "NLBL_ADRSEL"
#define NETLBL_NLTYPE_CALIPSO           7
#define NETLBL_NLTYPE_CALIPSO_NAME      "NLBL_CALIPSO"




struct netlbl_audit {
	u32 secid;
	kuid_t loginuid;
	unsigned int sessionid;
};




struct netlbl_lsm_cache {
	atomic_t refcount;
	void (*free) (const void *data);
	void *data;
};


#define NETLBL_CATMAP_MAPTYPE           u64
#define NETLBL_CATMAP_MAPCNT            4
#define NETLBL_CATMAP_MAPSIZE           (sizeof(NETLBL_CATMAP_MAPTYPE) * 8)
#define NETLBL_CATMAP_SIZE              (NETLBL_CATMAP_MAPSIZE * \
					 NETLBL_CATMAP_MAPCNT)
#define NETLBL_CATMAP_BIT               (NETLBL_CATMAP_MAPTYPE)0x01
struct netlbl_lsm_catmap {
	u32 startbit;
	NETLBL_CATMAP_MAPTYPE bitmap[NETLBL_CATMAP_MAPCNT];
	struct netlbl_lsm_catmap *next;
};


struct netlbl_lsm_secattr {
	u32 flags;
	
#define NETLBL_SECATTR_NONE             0x00000000
#define NETLBL_SECATTR_DOMAIN           0x00000001
#define NETLBL_SECATTR_DOMAIN_CPY       (NETLBL_SECATTR_DOMAIN | \
					 NETLBL_SECATTR_FREE_DOMAIN)
#define NETLBL_SECATTR_CACHE            0x00000002
#define NETLBL_SECATTR_MLS_LVL          0x00000004
#define NETLBL_SECATTR_MLS_CAT          0x00000008
#define NETLBL_SECATTR_SECID            0x00000010
	
#define NETLBL_SECATTR_FREE_DOMAIN      0x01000000
#define NETLBL_SECATTR_CACHEABLE        (NETLBL_SECATTR_MLS_LVL | \
					 NETLBL_SECATTR_MLS_CAT | \
					 NETLBL_SECATTR_SECID)
	u32 type;
	char *domain;
	struct netlbl_lsm_cache *cache;
	struct {
		struct {
			struct netlbl_lsm_catmap *cat;
			u32 lvl;
		} mls;
		u32 secid;
	} attr;
};


struct netlbl_calipso_ops {
	int (*doi_add)(struct calipso_doi *doi_def,
		       struct netlbl_audit *audit_info);
	void (*doi_free)(struct calipso_doi *doi_def);
	int (*doi_remove)(u32 doi, struct netlbl_audit *audit_info);
	struct calipso_doi *(*doi_getdef)(u32 doi);
	void (*doi_putdef)(struct calipso_doi *doi_def);
	int (*doi_walk)(u32 *skip_cnt,
			int (*callback)(struct calipso_doi *doi_def, void *arg),
			void *cb_arg);
	int (*sock_getattr)(struct sock *sk,
			    struct netlbl_lsm_secattr *secattr);
	int (*sock_setattr)(struct sock *sk,
			    const struct calipso_doi *doi_def,
			    const struct netlbl_lsm_secattr *secattr);
	void (*sock_delattr)(struct sock *sk);
	int (*req_setattr)(struct request_sock *req,
			   const struct calipso_doi *doi_def,
			   const struct netlbl_lsm_secattr *secattr);
	void (*req_delattr)(struct request_sock *req);
	int (*opt_getattr)(const unsigned char *calipso,
			   struct netlbl_lsm_secattr *secattr);
	unsigned char *(*skbuff_optptr)(const struct sk_buff *skb);
	int (*skbuff_setattr)(struct sk_buff *skb,
			      const struct calipso_doi *doi_def,
			      const struct netlbl_lsm_secattr *secattr);
	int (*skbuff_delattr)(struct sk_buff *skb);
	void (*cache_invalidate)(void);
	int (*cache_add)(const unsigned char *calipso_ptr,
			 const struct netlbl_lsm_secattr *secattr);
};




static inline struct netlbl_lsm_cache *netlbl_secattr_cache_alloc(gfp_t flags)
{
	struct netlbl_lsm_cache *cache;

	cache = kzalloc(sizeof(*cache), flags);
	if (cache)
		atomic_set(&cache->refcount, 1);
	return cache;
}


static inline void netlbl_secattr_cache_free(struct netlbl_lsm_cache *cache)
{
	if (!atomic_dec_and_test(&cache->refcount))
		return;

	if (cache->free)
		cache->free(cache->data);
	kfree(cache);
}


static inline struct netlbl_lsm_catmap *netlbl_catmap_alloc(gfp_t flags)
{
	return kzalloc(sizeof(struct netlbl_lsm_catmap), flags);
}


static inline void netlbl_catmap_free(struct netlbl_lsm_catmap *catmap)
{
	struct netlbl_lsm_catmap *iter;

	while (catmap) {
		iter = catmap;
		catmap = catmap->next;
		kfree(iter);
	}
}


static inline void netlbl_secattr_init(struct netlbl_lsm_secattr *secattr)
{
	memset(secattr, 0, sizeof(*secattr));
}


static inline void netlbl_secattr_destroy(struct netlbl_lsm_secattr *secattr)
{
	if (secattr->flags & NETLBL_SECATTR_FREE_DOMAIN)
		kfree(secattr->domain);
	if (secattr->flags & NETLBL_SECATTR_CACHE)
		netlbl_secattr_cache_free(secattr->cache);
	if (secattr->flags & NETLBL_SECATTR_MLS_CAT)
		netlbl_catmap_free(secattr->attr.mls.cat);
}


static inline struct netlbl_lsm_secattr *netlbl_secattr_alloc(gfp_t flags)
{
	return kzalloc(sizeof(struct netlbl_lsm_secattr), flags);
}


static inline void netlbl_secattr_free(struct netlbl_lsm_secattr *secattr)
{
	netlbl_secattr_destroy(secattr);
	kfree(secattr);
}

#ifdef CONFIG_NETLABEL

int netlbl_cfg_map_del(const char *domain,
		       u16 family,
		       const void *addr,
		       const void *mask,
		       struct netlbl_audit *audit_info);
int netlbl_cfg_unlbl_map_add(const char *domain,
			     u16 family,
			     const void *addr,
			     const void *mask,
			     struct netlbl_audit *audit_info);
int netlbl_cfg_unlbl_static_add(struct net *net,
				const char *dev_name,
				const void *addr,
				const void *mask,
				u16 family,
				u32 secid,
				struct netlbl_audit *audit_info);
int netlbl_cfg_unlbl_static_del(struct net *net,
				const char *dev_name,
				const void *addr,
				const void *mask,
				u16 family,
				struct netlbl_audit *audit_info);
int netlbl_cfg_cipsov4_add(struct cipso_v4_doi *doi_def,
			   struct netlbl_audit *audit_info);
void netlbl_cfg_cipsov4_del(u32 doi, struct netlbl_audit *audit_info);
int netlbl_cfg_cipsov4_map_add(u32 doi,
			       const char *domain,
			       const struct in_addr *addr,
			       const struct in_addr *mask,
			       struct netlbl_audit *audit_info);
int netlbl_cfg_calipso_add(struct calipso_doi *doi_def,
			   struct netlbl_audit *audit_info);
void netlbl_cfg_calipso_del(u32 doi, struct netlbl_audit *audit_info);
int netlbl_cfg_calipso_map_add(u32 doi,
			       const char *domain,
			       const struct in6_addr *addr,
			       const struct in6_addr *mask,
			       struct netlbl_audit *audit_info);

int netlbl_catmap_walk(struct netlbl_lsm_catmap *catmap, u32 offset);
int netlbl_catmap_walkrng(struct netlbl_lsm_catmap *catmap, u32 offset);
int netlbl_catmap_getlong(struct netlbl_lsm_catmap *catmap,
			  u32 *offset,
			  unsigned long *bitmap);
int netlbl_catmap_setbit(struct netlbl_lsm_catmap **catmap,
			 u32 bit,
			 gfp_t flags);
int netlbl_catmap_setrng(struct netlbl_lsm_catmap **catmap,
			 u32 start,
			 u32 end,
			 gfp_t flags);
int netlbl_catmap_setlong(struct netlbl_lsm_catmap **catmap,
			  u32 offset,
			  unsigned long bitmap,
			  gfp_t flags);


int netlbl_bitmap_walk(const unsigned char *bitmap, u32 bitmap_len,
		       u32 offset, u8 state);
void netlbl_bitmap_setbit(unsigned char *bitmap, u32 bit, u8 state);


int netlbl_enabled(void);
int netlbl_sock_setattr(struct sock *sk,
			u16 family,
			const struct netlbl_lsm_secattr *secattr);
void netlbl_sock_delattr(struct sock *sk);
int netlbl_sock_getattr(struct sock *sk,
			struct netlbl_lsm_secattr *secattr);
int netlbl_conn_setattr(struct sock *sk,
			struct sockaddr *addr,
			const struct netlbl_lsm_secattr *secattr);
int netlbl_req_setattr(struct request_sock *req,
		       const struct netlbl_lsm_secattr *secattr);
void netlbl_req_delattr(struct request_sock *req);
int netlbl_skbuff_setattr(struct sk_buff *skb,
			  u16 family,
			  const struct netlbl_lsm_secattr *secattr);
int netlbl_skbuff_getattr(const struct sk_buff *skb,
			  u16 family,
			  struct netlbl_lsm_secattr *secattr);
void netlbl_skbuff_err(struct sk_buff *skb, u16 family, int error, int gateway);


void netlbl_cache_invalidate(void);
int netlbl_cache_add(const struct sk_buff *skb, u16 family,
		     const struct netlbl_lsm_secattr *secattr);


struct audit_buffer *netlbl_audit_start(int type,
					struct netlbl_audit *audit_info);
#else
static inline int netlbl_cfg_map_del(const char *domain,
				     u16 family,
				     const void *addr,
				     const void *mask,
				     struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_cfg_unlbl_map_add(const char *domain,
					   u16 family,
					   void *addr,
					   void *mask,
					   struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_cfg_unlbl_static_add(struct net *net,
					      const char *dev_name,
					      const void *addr,
					      const void *mask,
					      u16 family,
					      u32 secid,
					      struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_cfg_unlbl_static_del(struct net *net,
					      const char *dev_name,
					      const void *addr,
					      const void *mask,
					      u16 family,
					      struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_cfg_cipsov4_add(struct cipso_v4_doi *doi_def,
					 struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline void netlbl_cfg_cipsov4_del(u32 doi,
					  struct netlbl_audit *audit_info)
{
	return;
}
static inline int netlbl_cfg_cipsov4_map_add(u32 doi,
					     const char *domain,
					     const struct in_addr *addr,
					     const struct in_addr *mask,
					     struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_cfg_calipso_add(struct calipso_doi *doi_def,
					 struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline void netlbl_cfg_calipso_del(u32 doi,
					  struct netlbl_audit *audit_info)
{
	return;
}
static inline int netlbl_cfg_calipso_map_add(u32 doi,
					     const char *domain,
					     const struct in6_addr *addr,
					     const struct in6_addr *mask,
					     struct netlbl_audit *audit_info)
{
	return -ENOSYS;
}
static inline int netlbl_catmap_walk(struct netlbl_lsm_catmap *catmap,
				     u32 offset)
{
	return -ENOENT;
}
static inline int netlbl_catmap_walkrng(struct netlbl_lsm_catmap *catmap,
					u32 offset)
{
	return -ENOENT;
}
static inline int netlbl_catmap_getlong(struct netlbl_lsm_catmap *catmap,
					u32 *offset,
					unsigned long *bitmap)
{
	return 0;
}
static inline int netlbl_catmap_setbit(struct netlbl_lsm_catmap **catmap,
				       u32 bit,
				       gfp_t flags)
{
	return 0;
}
static inline int netlbl_catmap_setrng(struct netlbl_lsm_catmap **catmap,
				       u32 start,
				       u32 end,
				       gfp_t flags)
{
	return 0;
}
static inline int netlbl_catmap_setlong(struct netlbl_lsm_catmap **catmap,
					u32 offset,
					unsigned long bitmap,
					gfp_t flags)
{
	return 0;
}
static inline int netlbl_enabled(void)
{
	return 0;
}
static inline int netlbl_sock_setattr(struct sock *sk,
				      u16 family,
				      const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline void netlbl_sock_delattr(struct sock *sk)
{
}
static inline int netlbl_sock_getattr(struct sock *sk,
				      struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline int netlbl_conn_setattr(struct sock *sk,
				      struct sockaddr *addr,
				      const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline int netlbl_req_setattr(struct request_sock *req,
				     const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline void netlbl_req_delattr(struct request_sock *req)
{
	return;
}
static inline int netlbl_skbuff_setattr(struct sk_buff *skb,
				      u16 family,
				      const struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline int netlbl_skbuff_getattr(const struct sk_buff *skb,
					u16 family,
					struct netlbl_lsm_secattr *secattr)
{
	return -ENOSYS;
}
static inline void netlbl_skbuff_err(struct sk_buff *skb,
				     int error,
				     int gateway)
{
	return;
}
static inline void netlbl_cache_invalidate(void)
{
	return;
}
static inline int netlbl_cache_add(const struct sk_buff *skb, u16 family,
				   const struct netlbl_lsm_secattr *secattr)
{
	return 0;
}
static inline struct audit_buffer *netlbl_audit_start(int type,
						struct netlbl_audit *audit_info)
{
	return NULL;
}
#endif 

const struct netlbl_calipso_ops *
netlbl_calipso_ops_register(const struct netlbl_calipso_ops *ops);

#endif 
