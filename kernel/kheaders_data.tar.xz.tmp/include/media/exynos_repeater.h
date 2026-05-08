

#ifndef _EXYNOS_REPEATER_H_
#define _EXYNOS_REPEATER_H_

#include <linux/dma-buf.h>

#define MAX_SHARED_BUF_NUM	3


struct shared_buffer_info {
	int pixel_format;
	int width;
	int height;
	int buffer_count;
	struct dma_buf *bufs[MAX_SHARED_BUF_NUM];
};


#ifdef CONFIG_VIDEO_EXYNOS_REPEATER
int hwfc_request_buffer(struct shared_buffer_info *info, int owner);
#else
static inline int hwfc_request_buffer(struct shared_buffer_info *info, int owner)
{
	return -1;
}
#endif


#ifdef CONFIG_VIDEO_EXYNOS_REPEATER
int hwfc_get_valid_buffer(int *buf_idx);
#else
static inline int hwfc_get_valid_buffer(int *buf_idx)
{
	return -1;
}
#endif


#ifdef CONFIG_VIDEO_EXYNOS_REPEATER
int hwfc_set_valid_buffer(int buf_idx, int capture_idx);
#else
static inline int hwfc_set_valid_buffer(int buf_idx, int capture_idx)
{
	return -1;
}
#endif


#ifdef CONFIG_VIDEO_EXYNOS_REPEATER
int hwfc_encoding_done(int encoding_ret);
#else
static inline int hwfc_encoding_done(int encoding_ret)
{
	return -1;
}
#endif

#endif 
