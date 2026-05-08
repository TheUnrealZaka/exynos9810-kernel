

#ifndef _LINUX_VERIFICATION_H
#define _LINUX_VERIFICATION_H


#define VERIFY_USE_SECONDARY_KEYRING ((struct key *)1UL)


enum key_being_used_for {
	VERIFYING_MODULE_SIGNATURE,
	VERIFYING_FIRMWARE_SIGNATURE,
	VERIFYING_KEXEC_PE_SIGNATURE,
	VERIFYING_KEY_SIGNATURE,
	VERIFYING_KEY_SELF_SIGNATURE,
	VERIFYING_UNSPECIFIED_SIGNATURE,
	NR__KEY_BEING_USED_FOR
};
extern const char *const key_being_used_for[NR__KEY_BEING_USED_FOR];

struct key;
struct public_key_signature;

extern int verify_signature_one(const struct public_key_signature *sig,
			   struct key *trusted_keys, const char *keyid);

#ifdef CONFIG_SYSTEM_DATA_VERIFICATION

extern int verify_pkcs7_signature(const void *data, size_t len,
				  const void *raw_pkcs7, size_t pkcs7_len,
				  struct key *trusted_keys,
				  enum key_being_used_for usage,
				  int (*view_content)(void *ctx,
						      const void *data, size_t len,
						      size_t asn1hdrlen),
				  void *ctx);

#ifdef CONFIG_SIGNED_PE_FILE_VERIFICATION
extern int verify_pefile_signature(const void *pebuf, unsigned pelen,
				   struct key *trusted_keys,
				   enum key_being_used_for usage);
#endif

#endif 
#endif 
