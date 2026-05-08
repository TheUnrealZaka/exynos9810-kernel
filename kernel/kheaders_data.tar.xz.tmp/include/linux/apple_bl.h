

#ifndef _LINUX_APPLE_BL_H
#define _LINUX_APPLE_BL_H

#if defined(CONFIG_BACKLIGHT_APPLE) || defined(CONFIG_BACKLIGHT_APPLE_MODULE)

extern int apple_bl_register(void);
extern void apple_bl_unregister(void);

#else 

static inline int apple_bl_register(void)
{
	return 0;
}

static inline void apple_bl_unregister(void)
{
}

#endif 

#endif 
