

#ifndef _LINUX_DNS_RESOLVER_H
#define _LINUX_DNS_RESOLVER_H

#ifdef __KERNEL__

extern int dns_query(const char *type, const char *name, size_t namelen,
		     const char *options, char **_result, time64_t *_expiry);

#endif 

#endif 
