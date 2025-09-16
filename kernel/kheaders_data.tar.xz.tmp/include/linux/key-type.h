

#ifndef _LINUX_KEY_TYPE_H
#define _LINUX_KEY_TYPE_H

#include <linux/key.h>
#include <linux/errno.h>

#ifdef CONFIG_KEYS


struct key_construction {
	struct key	*key;	
	struct key	*authkey;
};


struct key_preparsed_payload {
	char		*description;	
	union key_payload payload;	
	const void	*data;		
	size_t		datalen;	
	size_t		quotalen;	
	time_t		expiry;		
};

typedef int (*request_key_actor_t)(struct key_construction *key,
				   const char *op, void *aux);


struct key_match_data {
	
	bool (*cmp)(const struct key *key,
		    const struct key_match_data *match_data);

	const void	*raw_data;	
	void		*preparsed;	
	unsigned	lookup_type;	
#define KEYRING_SEARCH_LOOKUP_DIRECT	0x0000	
#define KEYRING_SEARCH_LOOKUP_ITERATE	0x0001	
};


struct key_type {
	
	const char *name;

	
	size_t def_datalen;

	
	int (*vet_description)(const char *description);

	
	int (*preparse)(struct key_preparsed_payload *prep);

	
	void (*free_preparse)(struct key_preparsed_payload *prep);

	
	int (*instantiate)(struct key *key, struct key_preparsed_payload *prep);

	
	int (*update)(struct key *key, struct key_preparsed_payload *prep);

	
	int (*match_preparse)(struct key_match_data *match_data);

	
	void (*match_free)(struct key_match_data *match_data);

	
	void (*revoke)(struct key *key);

	
	void (*destroy)(struct key *key);

	
	void (*describe)(const struct key *key, struct seq_file *p);

	
	long (*read)(const struct key *key, char __user *buffer, size_t buflen);

	
	request_key_actor_t request_key;

	
	struct list_head	link;		
	struct lock_class_key	lock_class;	
};

extern struct key_type key_type_keyring;

extern int register_key_type(struct key_type *ktype);
extern void unregister_key_type(struct key_type *ktype);

extern int key_payload_reserve(struct key *key, size_t datalen);
extern int key_instantiate_and_link(struct key *key,
				    const void *data,
				    size_t datalen,
				    struct key *keyring,
				    struct key *instkey);
extern int key_reject_and_link(struct key *key,
			       unsigned timeout,
			       unsigned error,
			       struct key *keyring,
			       struct key *instkey);
extern void complete_request_key(struct key_construction *cons, int error);

static inline int key_negate_and_link(struct key *key,
				      unsigned timeout,
				      struct key *keyring,
				      struct key *instkey)
{
	return key_reject_and_link(key, timeout, ENOKEY, keyring, instkey);
}

extern int generic_key_instantiate(struct key *key, struct key_preparsed_payload *prep);

#endif 
#endif 
