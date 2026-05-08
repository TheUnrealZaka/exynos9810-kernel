

#ifndef _CRYPTO_PKCS7_H
#define _CRYPTO_PKCS7_H

#include <linux/verification.h>
#include <crypto/public_key.h>

struct key;
struct pkcs7_message;


extern struct pkcs7_message *pkcs7_parse_message(const void *data,
						 size_t datalen);
extern void pkcs7_free_message(struct pkcs7_message *pkcs7);

extern int pkcs7_get_content_data(const struct pkcs7_message *pkcs7,
				  const void **_data, size_t *_datalen,
				  size_t *_headerlen);


extern int pkcs7_validate_trust(struct pkcs7_message *pkcs7,
				struct key *trust_keyring);


extern int pkcs7_verify(struct pkcs7_message *pkcs7,
			enum key_being_used_for usage);

extern int pkcs7_supply_detached_data(struct pkcs7_message *pkcs7,
				      const void *data, size_t datalen);

#endif 
