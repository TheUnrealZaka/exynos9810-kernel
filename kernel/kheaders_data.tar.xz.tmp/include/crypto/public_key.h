

#ifndef _LINUX_PUBLIC_KEY_H
#define _LINUX_PUBLIC_KEY_H


struct public_key {
	void *key;
	u32 keylen;
	const char *id_type;
	const char *pkey_algo;
};

extern void public_key_free(struct public_key *key);


struct public_key_signature {
	struct asymmetric_key_id *auth_ids[2];
	u8 *s;			
	u8 *digest;
	u32 s_size;		
	u32 digest_size;	
	const char *pkey_algo;
	const char *hash_algo;
};

extern void public_key_signature_free(struct public_key_signature *sig);

extern struct asymmetric_key_subtype public_key_subtype;

struct key;
struct key_type;
union key_payload;

extern int restrict_link_by_signature(struct key *trust_keyring,
				      const struct key_type *type,
				      const union key_payload *payload);

extern int verify_signature(const struct key *key,
			    const struct public_key_signature *sig);

int public_key_verify_signature(const struct public_key *pkey,
				const struct public_key_signature *sig);

#endif 
