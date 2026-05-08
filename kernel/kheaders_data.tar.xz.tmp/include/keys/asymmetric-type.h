

#ifndef _KEYS_ASYMMETRIC_TYPE_H
#define _KEYS_ASYMMETRIC_TYPE_H

#include <linux/key-type.h>
#include <linux/verification.h>

extern struct key_type key_type_asymmetric;


enum asymmetric_payload_bits {
	asym_crypto,		
	asym_subtype,		
	asym_key_ids,		
	asym_auth		
};


struct asymmetric_key_id {
	unsigned short	len;
	unsigned char	data[];
};

struct asymmetric_key_ids {
	void		*id[2];
};

extern bool asymmetric_key_id_same(const struct asymmetric_key_id *kid1,
				   const struct asymmetric_key_id *kid2);

extern bool asymmetric_key_id_partial(const struct asymmetric_key_id *kid1,
				      const struct asymmetric_key_id *kid2);

extern struct asymmetric_key_id *asymmetric_key_generate_id(const void *val_1,
							    size_t len_1,
							    const void *val_2,
							    size_t len_2);
static inline
const struct asymmetric_key_ids *asymmetric_key_ids(const struct key *key)
{
	return key->payload.data[asym_key_ids];
}

extern struct key *find_asymmetric_key(struct key *keyring,
				       const struct asymmetric_key_id *id_0,
				       const struct asymmetric_key_id *id_1,
				       bool partial);



#endif 
