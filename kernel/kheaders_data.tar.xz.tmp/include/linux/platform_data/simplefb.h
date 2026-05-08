

#ifndef __PLATFORM_DATA_SIMPLEFB_H__
#define __PLATFORM_DATA_SIMPLEFB_H__

#include <drm/drm_fourcc.h>
#include <linux/fb.h>
#include <linux/kernel.h>


#define SIMPLEFB_FORMATS \
{ \
	{ "r5g6b5", 16, {11, 5}, {5, 6}, {0, 5}, {0, 0}, DRM_FORMAT_RGB565 }, \
	{ "x1r5g5b5", 16, {10, 5}, {5, 5}, {0, 5}, {0, 0}, DRM_FORMAT_XRGB1555 }, \
	{ "a1r5g5b5", 16, {10, 5}, {5, 5}, {0, 5}, {15, 1}, DRM_FORMAT_ARGB1555 }, \
	{ "r8g8b8", 24, {16, 8}, {8, 8}, {0, 8}, {0, 0}, DRM_FORMAT_RGB888 }, \
	{ "x8r8g8b8", 32, {16, 8}, {8, 8}, {0, 8}, {0, 0}, DRM_FORMAT_XRGB8888 }, \
	{ "a8r8g8b8", 32, {16, 8}, {8, 8}, {0, 8}, {24, 8}, DRM_FORMAT_ARGB8888 }, \
	{ "a8b8g8r8", 32, {0, 8}, {8, 8}, {16, 8}, {24, 8}, DRM_FORMAT_ABGR8888 }, \
	{ "x2r10g10b10", 32, {20, 10}, {10, 10}, {0, 10}, {0, 0}, DRM_FORMAT_XRGB2101010 }, \
	{ "a2r10g10b10", 32, {20, 10}, {10, 10}, {0, 10}, {30, 2}, DRM_FORMAT_ARGB2101010 }, \
}


struct simplefb_format {
	const char *name;
	u32 bits_per_pixel;
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
	u32 fourcc;
};


struct simplefb_platform_data {
	u32 width;
	u32 height;
	u32 stride;
	const char *format;
};

#endif 
