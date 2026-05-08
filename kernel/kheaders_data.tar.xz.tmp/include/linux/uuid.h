
#ifndef _LINUX_UUID_H_
#define _LINUX_UUID_H_

#include <uapi/linux/uuid.h>


#define	UUID_STRING_LEN		36
#define	UUID_SIZE		16

static inline int uuid_le_cmp(const uuid_le u1, const uuid_le u2)
{
	return memcmp(&u1, &u2, sizeof(uuid_le));
}

static inline int uuid_be_cmp(const uuid_be u1, const uuid_be u2)
{
	return memcmp(&u1, &u2, sizeof(uuid_be));
}

void generate_random_uuid(unsigned char uuid[16]);

extern void uuid_le_gen(uuid_le *u);
extern void uuid_be_gen(uuid_be *u);

bool __must_check uuid_is_valid(const char *uuid);

extern const u8 uuid_le_index[16];
extern const u8 uuid_be_index[16];

int uuid_le_to_bin(const char *uuid, uuid_le *u);
int uuid_be_to_bin(const char *uuid, uuid_be *u);

#endif
