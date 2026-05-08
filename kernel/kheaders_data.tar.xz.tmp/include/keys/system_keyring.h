

#ifndef _KEYS_SYSTEM_KEYRING_H
#define _KEYS_SYSTEM_KEYRING_H

#include <linux/key.h>

#ifdef CONFIG_SYSTEM_TRUSTED_KEYRING

extern int restrict_link_by_builtin_trusted(struct key *keyring,
					    const struct key_type *type,
					    const union key_payload *payload);

#else
#define restrict_link_by_builtin_trusted restrict_link_reject
#endif

#ifdef CONFIG_SECONDARY_TRUSTED_KEYRING
extern int restrict_link_by_builtin_and_secondary_trusted(
	struct key *keyring,
	const struct key_type *type,
	const union key_payload *payload);
#else
#define restrict_link_by_builtin_and_secondary_trusted restrict_link_by_builtin_trusted
#endif

#ifdef CONFIG_IMA_BLACKLIST_KEYRING
extern struct key *ima_blacklist_keyring;

static inline struct key *get_ima_blacklist_keyring(void)
{
	return ima_blacklist_keyring;
}
#else
static inline struct key *get_ima_blacklist_keyring(void)
{
	return NULL;
}
#endif 


#endif 
