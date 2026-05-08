

#ifndef _NF_CONNTRACK_H
#define _NF_CONNTRACK_H

#include <linux/netfilter/nf_conntrack_common.h>

#include <linux/bitops.h>
#include <linux/compiler.h>
#include <linux/atomic.h>

#include <linux/netfilter/nf_conntrack_tcp.h>
#include <linux/netfilter/nf_conntrack_dccp.h>
#include <linux/netfilter/nf_conntrack_sctp.h>
#include <linux/netfilter/nf_conntrack_proto_gre.h>
#include <net/netfilter/ipv6/nf_conntrack_icmpv6.h>

#include <net/netfilter/nf_conntrack_tuple.h>

// KNOX NPA - START
#define PROCESS_NAME_LEN_NAP	128
#define DOMAIN_NAME_LEN_NAP	255
// KNOX NPA - END

union nf_conntrack_proto {
	
	struct nf_ct_dccp dccp;
	struct ip_ct_sctp sctp;
	struct ip_ct_tcp tcp;
	struct nf_ct_gre gre;
};

union nf_conntrack_expect_proto {
	
};

#include <linux/types.h>
#include <linux/skbuff.h>

#ifdef CONFIG_NETFILTER_DEBUG
#define NF_CT_ASSERT(x)		WARN_ON(!(x))
#else
#define NF_CT_ASSERT(x)
#endif

struct nf_conntrack_helper;


#define NF_CT_MAX_EXPECT_CLASSES	4


struct nf_conn_help {
	
	struct nf_conntrack_helper __rcu *helper;

	struct hlist_head expectations;

	
	u8 expecting[NF_CT_MAX_EXPECT_CLASSES];

	
	char data[];
};

#include <net/netfilter/ipv4/nf_conntrack_ipv4.h>
#include <net/netfilter/ipv6/nf_conntrack_ipv6.h>

struct nf_conn {
	
	struct nf_conntrack ct_general;

	spinlock_t	lock;
	u16		cpu;

#ifdef CONFIG_NF_CONNTRACK_ZONES
	struct nf_conntrack_zone zone;
#endif
	
	
	struct nf_conntrack_tuple_hash tuplehash[IP_CT_DIR_MAX];

	
	unsigned long status;

	
	u32 timeout;

	possible_net_t ct_net;

#if IS_ENABLED(CONFIG_NF_NAT)
	struct hlist_node	nat_bysource;
#endif
	
	struct { } __nfct_init_offset;

	
	struct nf_conn *master;

#if defined(CONFIG_NF_CONNTRACK_MARK)
	u_int32_t mark;
#endif

#ifdef CONFIG_NF_CONNTRACK_SECMARK
	u_int32_t secmark;
#endif

	
	struct nf_ct_ext *ext;

	
	union nf_conntrack_proto proto;


	// KNOX NPA - START
	
	__u64   knox_sent;
	
	__u64   knox_recv;
	
	uid_t   knox_uid;
	
	pid_t   knox_pid;
	
	uid_t   knox_puid;
	
	__u64   open_time;
	
	char process_name[PROCESS_NAME_LEN_NAP];
	
	char parent_process_name[PROCESS_NAME_LEN_NAP];
	
	char domain_name[DOMAIN_NAME_LEN_NAP];
	
	pid_t   knox_ppid;
	
	char interface_name[IFNAMSIZ];
	
	atomic_t startFlow;
	
	
	u32 npa_timeout;
	
	atomic_t intermediateFlow;
	// KNOX NPA - END

};

static inline struct nf_conn *
nf_ct_tuplehash_to_ctrack(const struct nf_conntrack_tuple_hash *hash)
{
	return container_of(hash, struct nf_conn,
			    tuplehash[hash->tuple.dst.dir]);
}

static inline u_int16_t nf_ct_l3num(const struct nf_conn *ct)
{
	return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.l3num;
}

static inline u_int8_t nf_ct_protonum(const struct nf_conn *ct)
{
	return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
}

#define nf_ct_tuple(ct, dir) (&(ct)->tuplehash[dir].tuple)


#define master_ct(conntr) (conntr->master)

extern struct net init_net;

static inline struct net *nf_ct_net(const struct nf_conn *ct)
{
	return read_pnet(&ct->ct_net);
}


void nf_conntrack_alter_reply(struct nf_conn *ct,
			      const struct nf_conntrack_tuple *newreply);


int nf_conntrack_tuple_taken(const struct nf_conntrack_tuple *tuple,
			     const struct nf_conn *ignored_conntrack);


static inline struct nf_conn *
nf_ct_get(const struct sk_buff *skb, enum ip_conntrack_info *ctinfo)
{
	*ctinfo = skb->nfctinfo;
	return (struct nf_conn *)skb->nfct;
}


static inline void nf_ct_put(struct nf_conn *ct)
{
	NF_CT_ASSERT(ct);
	nf_conntrack_put(&ct->ct_general);
}


int nf_ct_l3proto_try_module_get(unsigned short l3proto);
void nf_ct_l3proto_module_put(unsigned short l3proto);


void *nf_ct_alloc_hashtable(unsigned int *sizep, int nulls);

void nf_ct_free_hashtable(void *hash, unsigned int size);

int nf_conntrack_hash_check_insert(struct nf_conn *ct);
bool nf_ct_delete(struct nf_conn *ct, u32 pid, int report);

bool nf_ct_get_tuplepr(const struct sk_buff *skb, unsigned int nhoff,
		       u_int16_t l3num, struct net *net,
		       struct nf_conntrack_tuple *tuple);
bool nf_ct_invert_tuplepr(struct nf_conntrack_tuple *inverse,
			  const struct nf_conntrack_tuple *orig);

void __nf_ct_refresh_acct(struct nf_conn *ct, enum ip_conntrack_info ctinfo,
			  const struct sk_buff *skb,
			  unsigned long extra_jiffies, int do_acct);


static inline void nf_ct_refresh_acct(struct nf_conn *ct,
				      enum ip_conntrack_info ctinfo,
				      const struct sk_buff *skb,
				      unsigned long extra_jiffies)
{
	__nf_ct_refresh_acct(ct, ctinfo, skb, extra_jiffies, 1);
}


static inline void nf_ct_refresh(struct nf_conn *ct,
				 const struct sk_buff *skb,
				 unsigned long extra_jiffies)
{
	__nf_ct_refresh_acct(ct, 0, skb, extra_jiffies, 0);
}


bool nf_ct_kill_acct(struct nf_conn *ct, enum ip_conntrack_info ctinfo,
		     const struct sk_buff *skb);


static inline bool nf_ct_kill(struct nf_conn *ct)
{
	return nf_ct_delete(ct, 0, 0);
}


extern s32 (*nf_ct_nat_offset)(const struct nf_conn *ct,
			       enum ip_conntrack_dir dir,
			       u32 seq);


DECLARE_PER_CPU(struct nf_conn, nf_conntrack_untracked);
static inline struct nf_conn *nf_ct_untracked_get(void)
{
	return raw_cpu_ptr(&nf_conntrack_untracked);
}
void nf_ct_untracked_status_or(unsigned long bits);


void nf_ct_iterate_cleanup(struct net *net,
			   int (*iter)(struct nf_conn *i, void *data),
			   void *data, u32 portid, int report);

struct nf_conntrack_zone;

void nf_conntrack_free(struct nf_conn *ct);
struct nf_conn *nf_conntrack_alloc(struct net *net,
				   const struct nf_conntrack_zone *zone,
				   const struct nf_conntrack_tuple *orig,
				   const struct nf_conntrack_tuple *repl,
				   gfp_t gfp);

static inline int nf_ct_is_template(const struct nf_conn *ct)
{
	return test_bit(IPS_TEMPLATE_BIT, &ct->status);
}


static inline int nf_ct_is_confirmed(const struct nf_conn *ct)
{
	return test_bit(IPS_CONFIRMED_BIT, &ct->status);
}

static inline int nf_ct_is_dying(const struct nf_conn *ct)
{
	return test_bit(IPS_DYING_BIT, &ct->status);
}

static inline int nf_ct_is_untracked(const struct nf_conn *ct)
{
	return test_bit(IPS_UNTRACKED_BIT, &ct->status);
}


static inline bool nf_is_loopback_packet(const struct sk_buff *skb)
{
	return skb->dev && skb->skb_iif && skb->dev->flags & IFF_LOOPBACK;
}

#define nfct_time_stamp ((u32)(jiffies))


static inline unsigned long nf_ct_expires(const struct nf_conn *ct)
{
	s32 timeout = ct->timeout - nfct_time_stamp;

	return timeout > 0 ? timeout : 0;
}

static inline bool nf_ct_is_expired(const struct nf_conn *ct)
{
	return (__s32)(ct->timeout - nfct_time_stamp) <= 0;
}


static inline bool nf_ct_should_gc(const struct nf_conn *ct)
{
	return nf_ct_is_expired(ct) && nf_ct_is_confirmed(ct) &&
	       !nf_ct_is_dying(ct);
}

struct kernel_param;

int nf_conntrack_set_hashsize(const char *val, const struct kernel_param *kp);
int nf_conntrack_hash_resize(unsigned int hashsize);

extern struct hlist_nulls_head *nf_conntrack_hash;
extern unsigned int nf_conntrack_htable_size;
extern seqcount_t nf_conntrack_generation;
extern unsigned int nf_conntrack_max;


static inline void
nf_conntrack_get_ht(struct hlist_nulls_head **hash, unsigned int *hsize)
{
	struct hlist_nulls_head *hptr;
	unsigned int sequence, hsz;

	do {
		sequence = read_seqcount_begin(&nf_conntrack_generation);
		hsz = nf_conntrack_htable_size;
		hptr = nf_conntrack_hash;
	} while (read_seqcount_retry(&nf_conntrack_generation, sequence));

	*hash = hptr;
	*hsize = hsz;
}

struct nf_conn *nf_ct_tmpl_alloc(struct net *net,
				 const struct nf_conntrack_zone *zone,
				 gfp_t flags);
void nf_ct_tmpl_free(struct nf_conn *tmpl);

u32 nf_ct_get_id(const struct nf_conn *ct);

#define NF_CT_STAT_INC(net, count)	  __this_cpu_inc((net)->ct.stat->count)
#define NF_CT_STAT_INC_ATOMIC(net, count) this_cpu_inc((net)->ct.stat->count)
#define NF_CT_STAT_ADD_ATOMIC(net, count, v) this_cpu_add((net)->ct.stat->count, (v))

#define MODULE_ALIAS_NFCT_HELPER(helper) \
        MODULE_ALIAS("nfct-helper-" helper)

#endif 
