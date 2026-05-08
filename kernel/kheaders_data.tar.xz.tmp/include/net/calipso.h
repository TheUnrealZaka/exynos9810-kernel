



#ifndef _CALIPSO_H
#define _CALIPSO_H

#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/list.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/netlabel.h>
#include <net/request_sock.h>
#include <linux/atomic.h>
#include <asm/unaligned.h>


#define CALIPSO_DOI_UNKNOWN          0x00000000


#define CALIPSO_MAP_UNKNOWN          0
#define CALIPSO_MAP_PASS             2




struct calipso_doi {
	u32 doi;
	u32 type;

	atomic_t refcount;
	struct list_head list;
	struct rcu_head rcu;
};


extern int calipso_cache_enabled;
extern int calipso_cache_bucketsize;

#ifdef CONFIG_NETLABEL
int __init calipso_init(void);
void calipso_exit(void);
bool calipso_validate(const struct sk_buff *skb, const unsigned char *option);
#else
static inline int __init calipso_init(void)
{
	return 0;
}

static inline void calipso_exit(void)
{
}
static inline bool calipso_validate(const struct sk_buff *skb,
				    const unsigned char *option)
{
	return true;
}
#endif 

#endif 
