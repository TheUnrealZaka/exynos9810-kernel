

#ifndef __SEC_ADAPTATION_H
#define __SEC_ADAPTATION_H

#include <sound/maxim_dsm.h>
#include <sound/samsung/abox.h>

#ifdef CONFIG_SEC_SND_ADAPTATION
int maxim_dsm_write(uint32_t *data, int offset, int size);
int maxim_dsm_read(int offset, int size, void *dsm_data);
#else
static inline int maxim_dsm_write(uint32_t *data, int offset, int size)
{
	return -ENOSYS;
}

static inline int maxim_dsm_read(int offset, int size, void *dsm_data)
{
	return -ENOSYS;
}
#endif 

static inline int32_t dsm_read_write(void *data)
{
	return -ENOSYS;
}

#endif 

