

#ifndef _LINUX_SUNRPC_SVCAUTH_H_
#define _LINUX_SUNRPC_SVCAUTH_H_

#ifdef __KERNEL__

#include <linux/string.h>
#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/cache.h>
#include <linux/sunrpc/gss_api.h>
#include <linux/hash.h>
#include <linux/stringhash.h>
#include <linux/cred.h>

struct svc_cred {
	kuid_t			cr_uid;
	kgid_t			cr_gid;
	struct group_info	*cr_group_info;
	u32			cr_flavor; 
	
	char			*cr_raw_principal;
	
	char			*cr_principal;
	struct gss_api_mech	*cr_gss_mech;
};

static inline void init_svc_cred(struct svc_cred *cred)
{
	cred->cr_group_info = NULL;
	cred->cr_raw_principal = NULL;
	cred->cr_principal = NULL;
	cred->cr_gss_mech = NULL;
}

static inline void free_svc_cred(struct svc_cred *cred)
{
	if (cred->cr_group_info)
		put_group_info(cred->cr_group_info);
	kfree(cred->cr_raw_principal);
	kfree(cred->cr_principal);
	gss_mech_put(cred->cr_gss_mech);
	init_svc_cred(cred);
}

struct svc_rqst;		
struct in6_addr;


struct auth_domain {
	struct kref		ref;
	struct hlist_node	hash;
	char			*name;
	struct auth_ops		*flavour;
};


struct auth_ops {
	char *	name;
	struct module *owner;
	int	flavour;
	int	(*accept)(struct svc_rqst *rq, __be32 *authp);
	int	(*release)(struct svc_rqst *rq);
	void	(*domain_release)(struct auth_domain *);
	int	(*set_client)(struct svc_rqst *rq);
};

#define	SVC_GARBAGE	1
#define	SVC_SYSERR	2
#define	SVC_VALID	3
#define	SVC_NEGATIVE	4
#define	SVC_OK		5
#define	SVC_DROP	6
#define	SVC_CLOSE	7	
#define	SVC_DENIED	8
#define	SVC_PENDING	9
#define	SVC_COMPLETE	10

struct svc_xprt;

extern int	svc_authenticate(struct svc_rqst *rqstp, __be32 *authp);
extern int	svc_authorise(struct svc_rqst *rqstp);
extern int	svc_set_client(struct svc_rqst *rqstp);
extern int	svc_auth_register(rpc_authflavor_t flavor, struct auth_ops *aops);
extern void	svc_auth_unregister(rpc_authflavor_t flavor);

extern struct auth_domain *unix_domain_find(char *name);
extern void auth_domain_put(struct auth_domain *item);
extern int auth_unix_add_addr(struct net *net, struct in6_addr *addr, struct auth_domain *dom);
extern struct auth_domain *auth_domain_lookup(char *name, struct auth_domain *new);
extern struct auth_domain *auth_domain_find(char *name);
extern struct auth_domain *auth_unix_lookup(struct net *net, struct in6_addr *addr);
extern int auth_unix_forget_old(struct auth_domain *dom);
extern void svcauth_unix_purge(struct net *net);
extern void svcauth_unix_info_release(struct svc_xprt *xpt);
extern int svcauth_unix_set_client(struct svc_rqst *rqstp);

extern int unix_gid_cache_create(struct net *net);
extern void unix_gid_cache_destroy(struct net *net);


static inline unsigned long hash_str(char const *name, int bits)
{
	return hashlen_hash(hashlen_string(NULL, name)) >> (32 - bits);
}

static inline unsigned long hash_mem(char const *buf, int length, int bits)
{
	return full_name_hash(NULL, buf, length) >> (32 - bits);
}

#endif 

#endif 
