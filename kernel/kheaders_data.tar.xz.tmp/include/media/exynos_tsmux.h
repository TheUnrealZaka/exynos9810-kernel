

#ifndef EXYNOS_TSMUX_H
#define EXYNOS_TSMUX_H

struct packetizing_param {
	long time_stamp;
};

#ifdef CONFIG_VIDEO_EXYNOS_TSMUX
int g2d_blending_start(int32_t index);
int g2d_blending_end(int32_t index);
int mfc_encoding_start(int32_t index);
int mfc_encoding_end(void);
int packetize(struct packetizing_param *param);
void set_es_size(unsigned int size);
void tsmux_sfr_dump(void);
#else
int g2d_blending_start(int32_t index)
{
	return -1;
}
int g2d_blending_end(int32_t index)
{
	return -1;
}
int mfc_encoding_start(int32_t index)
{
	return -1;
}
int mfc_encoding_end(void)
{
	return -1;
}
static inline int packetize(struct packetizing_param *param)
{
	return -1;
}
static inline void set_es_size(unsigned int size)
{
	return;
}
static inline void tsmux_sfr_dump(void)
{

}
#endif

#endif
