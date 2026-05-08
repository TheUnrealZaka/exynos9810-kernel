

#ifndef _KEYS_USER_TYPE_H
#define _KEYS_USER_TYPE_H

#include <linux/key.h>
#include <linux/rcupdate.h>

#ifdef CONFIG_KEYS



struct user_key_payload {
	struct rcu_head	rcu;		
	unsigned short	datalen;	
	char		data[0] __aligned(__alignof__(u64)); 
};

extern struct key_type key_type_user;
extern struct key_type key_type_logon;

struct key_preparsed_payload;

extern int user_preparse(struct key_preparsed_payload *prep);
extern void user_free_preparse(struct key_preparsed_payload *prep);
extern int user_update(struct key *key, struct key_preparsed_payload *prep);
extern void user_revoke(struct key *key);
extern void user_destroy(struct key *key);
extern void user_describe(const struct key *user, struct seq_file *m);
extern long user_read(const struct key *key,
		      char __user *buffer, size_t buflen);

static inline const struct user_key_payload *user_key_payload_rcu(const struct key *key)
{
	return (struct user_key_payload *)dereference_key_rcu(key);
}

static inline struct user_key_payload *user_key_payload_locked(const struct key *key)
{
	return (struct user_key_payload *)dereference_key_locked((struct key *)key);
}

#endif 

#endif 
