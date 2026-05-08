
#ifndef __DRM_FOURCC_H__
#define __DRM_FOURCC_H__

#include <linux/types.h>
#include <uapi/drm/drm_fourcc.h>

uint32_t drm_mode_legacy_fb_format(uint32_t bpp, uint32_t depth);
void drm_fb_get_bpp_depth(uint32_t format, unsigned int *depth, int *bpp);
int drm_format_num_planes(uint32_t format);
int drm_format_plane_cpp(uint32_t format, int plane);
int drm_format_horz_chroma_subsampling(uint32_t format);
int drm_format_vert_chroma_subsampling(uint32_t format);
int drm_format_plane_width(int width, uint32_t format, int plane);
int drm_format_plane_height(int height, uint32_t format, int plane);
char *drm_get_format_name(uint32_t format) __malloc;

#endif 
