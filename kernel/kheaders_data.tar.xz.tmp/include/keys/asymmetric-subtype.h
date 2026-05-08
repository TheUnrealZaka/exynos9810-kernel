

#ifndef _KEYS_ASYMMETRIC_SUBTYPE_H
#define _KEYS_ASYMMETRIC_SUBTYPE_H

#include <linux/seq_file.h>
#include <keys/asymmetric-type.h>

struct public_key_signature;


struct asymmetric_key_subtype {
	struct module		*owner;
	const char		*name;
	unsigned short		name_len;	

	
	void (*describe)(const struct key *key, struct seq_file *m);

	
	void (*destroy)(void *payload_crypto, void *payload_auth);

	
	int (*verify_signature)(const struct key *key,
				const struct public_key_signature *sig);
};


static inline
struct asymmetric_key_subtype *asymmetric_key_subtype(const struct key *key)
{
	return key->payload.data[asym_subtype];
}

#endif 
