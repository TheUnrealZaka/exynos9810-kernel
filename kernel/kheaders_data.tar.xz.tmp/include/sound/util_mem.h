#ifndef __SOUND_UTIL_MEM_H
#define __SOUND_UTIL_MEM_H

#include <linux/mutex.h>



struct snd_util_memblk {
	unsigned int size;		
	unsigned int offset;		
	struct list_head list;		
};

#define snd_util_memblk_argptr(blk)	(void*)((char*)(blk) + sizeof(struct snd_util_memblk))


struct snd_util_memhdr {
	unsigned int size;		
	struct list_head block;		
	int nblocks;			
	unsigned int used;		
	int block_extra_size;		
	struct mutex block_mutex;	
};


struct snd_util_memhdr *snd_util_memhdr_new(int memsize);
void snd_util_memhdr_free(struct snd_util_memhdr *hdr);
struct snd_util_memblk *snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size);
int snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk);
int snd_util_mem_avail(struct snd_util_memhdr *hdr);


struct snd_util_memblk *__snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size);
void __snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk);
struct snd_util_memblk *__snd_util_memblk_new(struct snd_util_memhdr *hdr,
					      unsigned int units,
					      struct list_head *prev);

#endif 
