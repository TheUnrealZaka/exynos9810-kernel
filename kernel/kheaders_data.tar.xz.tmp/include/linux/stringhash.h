#ifndef __LINUX_STRINGHASH_H
#define __LINUX_STRINGHASH_H

#include <linux/compiler.h>	
#include <linux/types.h>	
#include <linux/hash.h>






#define init_name_hash(salt)		(unsigned long)(salt)


static inline unsigned long
partial_name_hash(unsigned long c, unsigned long prevhash)
{
	return (prevhash + (c << 4) + (c >> 4)) * 11;
}


static inline unsigned long end_name_hash(unsigned long hash)
{
	return __hash_32((unsigned int)hash);
}


extern unsigned int __pure full_name_hash(const void *salt, const char *, unsigned int);


#define hashlen_hash(hashlen) ((u32)(hashlen))
#define hashlen_len(hashlen)  ((u32)((hashlen) >> 32))
#define hashlen_create(hash, len) ((u64)(len)<<32 | (u32)(hash))


extern u64 __pure hashlen_string(const void *salt, const char *name);

#endif	
