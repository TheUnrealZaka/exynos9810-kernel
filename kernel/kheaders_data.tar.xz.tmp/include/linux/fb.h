#ifndef _LINUX_FB_H
#define _LINUX_FB_H

#include <linux/kgdb.h>
#include <uapi/linux/fb.h>

#define FBIO_CURSOR            _IOWR('F', 0x08, struct fb_cursor_user)

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <linux/backlight.h>
#include <linux/slab.h>
#include <asm/io.h>

struct vm_area_struct;
struct fb_info;
struct device;
struct file;
struct videomode;
struct device_node;


#define FB_DPMS_ACTIVE_OFF	1
#define FB_DPMS_SUSPEND		2
#define FB_DPMS_STANDBY		4

#define FB_DISP_DDI		1
#define FB_DISP_ANA_700_300	2
#define FB_DISP_ANA_714_286	4
#define FB_DISP_ANA_1000_400	8
#define FB_DISP_ANA_700_000	16

#define FB_DISP_MONO		32
#define FB_DISP_RGB		64
#define FB_DISP_MULTI		128
#define FB_DISP_UNKNOWN		256

#define FB_SIGNAL_NONE		0
#define FB_SIGNAL_BLANK_BLANK	1
#define FB_SIGNAL_SEPARATE	2
#define FB_SIGNAL_COMPOSITE	4
#define FB_SIGNAL_SYNC_ON_GREEN	8
#define FB_SIGNAL_SERRATION_ON	16

#define FB_MISC_PRIM_COLOR	1
#define FB_MISC_1ST_DETAIL	2	
#define FB_MISC_HDMI		4
struct fb_chroma {
	__u32 redx;	
	__u32 greenx;
	__u32 bluex;
	__u32 whitex;
	__u32 redy;
	__u32 greeny;
	__u32 bluey;
	__u32 whitey;
};

struct fb_monspecs {
	struct fb_chroma chroma;
	struct fb_videomode *modedb;	
	__u8  manufacturer[4];		
	__u8  monitor[14];		
	__u8  serial_no[14];		
	__u8  ascii[14];		
	__u32 modedb_len;		
	__u32 model;			
	__u32 serial;			
	__u32 year;			
	__u32 week;			
	__u32 hfmin;			
	__u32 hfmax;			
	__u32 dclkmin;			
	__u32 dclkmax;			
	__u16 input;			
	__u16 dpms;			
	__u16 signal;			
	__u16 vfmin;			
	__u16 vfmax;			
	__u16 gamma;			
	__u16 gtf	: 1;		
	__u16 misc;			
	__u8  version;			
	__u8  revision;			
	__u8  max_x;			
	__u8  max_y;			
};

struct fb_cmap_user {
	__u32 start;			
	__u32 len;			
	__u16 __user *red;		
	__u16 __user *green;
	__u16 __user *blue;
	__u16 __user *transp;		
};

struct fb_image_user {
	__u32 dx;			
	__u32 dy;
	__u32 width;			
	__u32 height;
	__u32 fg_color;			
	__u32 bg_color;
	__u8  depth;			
	const char __user *data;	
	struct fb_cmap_user cmap;	
};

struct fb_cursor_user {
	__u16 set;			
	__u16 enable;			
	__u16 rop;			
	const char __user *mask;	
	struct fbcurpos hot;		
	struct fb_image_user image;	
};



 
#define FB_EVENT_MODE_CHANGE		0x01

#define FB_EVENT_SUSPEND		0x02

#define FB_EVENT_RESUME			0x03

#define FB_EVENT_MODE_DELETE            0x04

#define FB_EVENT_FB_REGISTERED          0x05

#define FB_EVENT_FB_UNREGISTERED        0x06

#define FB_EVENT_GET_CONSOLE_MAP        0x07

#define FB_EVENT_SET_CONSOLE_MAP        0x08

#define FB_EVENT_BLANK                  0x09

#define FB_EVENT_NEW_MODELIST           0x0A

#define FB_EVENT_MODE_CHANGE_ALL	0x0B

#define FB_EVENT_CONBLANK               0x0C

#define FB_EVENT_GET_REQ                0x0D

#define FB_EVENT_FB_UNBIND              0x0E

#define FB_EVENT_REMAP_ALL_CONSOLE      0x0F

#define FB_EARLY_EVENT_BLANK		0x10

#define FB_R_EARLY_EVENT_BLANK		0x11

struct fb_event {
	struct fb_info *info;
	void *data;
};

struct fb_blit_caps {
	u32 x;
	u32 y;
	u32 len;
	u32 flags;
};

#ifdef CONFIG_FB_NOTIFY
extern int fb_register_client(struct notifier_block *nb);
extern int fb_unregister_client(struct notifier_block *nb);
extern int fb_notifier_call_chain(unsigned long val, void *v);
#else
static inline int fb_register_client(struct notifier_block *nb)
{
	return 0;
};

static inline int fb_unregister_client(struct notifier_block *nb)
{
	return 0;
};

static inline int fb_notifier_call_chain(unsigned long val, void *v)
{
	return 0;
};
#endif



#define FB_PIXMAP_DEFAULT 1     
#define FB_PIXMAP_SYSTEM  2     
#define FB_PIXMAP_IO      4     
#define FB_PIXMAP_SYNC    256   

struct fb_pixmap {
	u8  *addr;		
	u32 size;		
	u32 offset;		
	u32 buf_align;		
	u32 scan_align;		
	u32 access_align;	
	u32 flags;		
	u32 blit_x;             
	u32 blit_y;             
	                        
	                        
	
	void (*writeio)(struct fb_info *info, void __iomem *dst, void *src, unsigned int size);
	void (*readio) (struct fb_info *info, void *dst, void __iomem *src, unsigned int size);
};

#ifdef CONFIG_FB_DEFERRED_IO
struct fb_deferred_io {
	
	unsigned long delay;
	struct mutex lock; 
	struct list_head pagelist; 
	
	void (*first_io)(struct fb_info *info);
	void (*deferred_io)(struct fb_info *info, struct list_head *pagelist);
};
#endif



struct fb_ops {
	
	struct module *owner;
	int (*fb_open)(struct fb_info *info, int user);
	int (*fb_release)(struct fb_info *info, int user);

	
	ssize_t (*fb_read)(struct fb_info *info, char __user *buf,
			   size_t count, loff_t *ppos);
	ssize_t (*fb_write)(struct fb_info *info, const char __user *buf,
			    size_t count, loff_t *ppos);

	
	int (*fb_check_var)(struct fb_var_screeninfo *var, struct fb_info *info);

	
	int (*fb_set_par)(struct fb_info *info);

	
	int (*fb_setcolreg)(unsigned regno, unsigned red, unsigned green,
			    unsigned blue, unsigned transp, struct fb_info *info);

	
	int (*fb_setcmap)(struct fb_cmap *cmap, struct fb_info *info);

	
	int (*fb_blank)(int blank, struct fb_info *info);

	
	int (*fb_pan_display)(struct fb_var_screeninfo *var, struct fb_info *info);

	
	void (*fb_fillrect) (struct fb_info *info, const struct fb_fillrect *rect);
	
	void (*fb_copyarea) (struct fb_info *info, const struct fb_copyarea *region);
	
	void (*fb_imageblit) (struct fb_info *info, const struct fb_image *image);

	
	int (*fb_cursor) (struct fb_info *info, struct fb_cursor *cursor);

	
	int (*fb_sync)(struct fb_info *info);

	
	int (*fb_ioctl)(struct fb_info *info, unsigned int cmd,
			unsigned long arg);

	
	int (*fb_compat_ioctl)(struct fb_info *info, unsigned cmd,
			unsigned long arg);

	
	int (*fb_mmap)(struct fb_info *info, struct vm_area_struct *vma);

	
	void (*fb_get_caps)(struct fb_info *info, struct fb_blit_caps *caps,
			    struct fb_var_screeninfo *var);

	
	void (*fb_destroy)(struct fb_info *info);

	
	int (*fb_debug_enter)(struct fb_info *info);
	int (*fb_debug_leave)(struct fb_info *info);
};

#ifdef CONFIG_FB_TILEBLITTING
#define FB_TILE_CURSOR_NONE        0
#define FB_TILE_CURSOR_UNDERLINE   1
#define FB_TILE_CURSOR_LOWER_THIRD 2
#define FB_TILE_CURSOR_LOWER_HALF  3
#define FB_TILE_CURSOR_TWO_THIRDS  4
#define FB_TILE_CURSOR_BLOCK       5

struct fb_tilemap {
	__u32 width;                
	__u32 height;               
	__u32 depth;                
	__u32 length;               
	const __u8 *data;           
};

struct fb_tilerect {
	__u32 sx;                   
	__u32 sy;                   
	__u32 width;                
	__u32 height;               
	__u32 index;                
	__u32 fg;                   
	__u32 bg;                   
	__u32 rop;                  
};

struct fb_tilearea {
	__u32 sx;                   
	__u32 sy;                   
	__u32 dx;                   
	__u32 dy;                   
	__u32 width;                
	__u32 height;               
};

struct fb_tileblit {
	__u32 sx;                   
	__u32 sy;                   
	__u32 width;                
	__u32 height;               
	__u32 fg;                   
	__u32 bg;                   
	__u32 length;               
	__u32 *indices;             
};

struct fb_tilecursor {
	__u32 sx;                   
	__u32 sy;                   
	__u32 mode;                 
	__u32 shape;                
	__u32 fg;                   
	__u32 bg;                   
};

struct fb_tile_ops {
	
	void (*fb_settile)(struct fb_info *info, struct fb_tilemap *map);

	

	
	void (*fb_tilecopy)(struct fb_info *info, struct fb_tilearea *area);
	
	void (*fb_tilefill)(struct fb_info *info, struct fb_tilerect *rect);
	
	void (*fb_tileblit)(struct fb_info *info, struct fb_tileblit *blit);
	
	void (*fb_tilecursor)(struct fb_info *info,
			      struct fb_tilecursor *cursor);
	
	int (*fb_get_tilemax)(struct fb_info *info);
};
#endif 


#define FBINFO_MODULE		0x0001	
#define FBINFO_HWACCEL_DISABLED	0x0002
	


#define FBINFO_VIRTFB		0x0004 
#define FBINFO_PARTIAL_PAN_OK	0x0040 
#define FBINFO_READS_FAST	0x0080 



#define FBINFO_HWACCEL_NONE		0x0000
#define FBINFO_HWACCEL_COPYAREA		0x0100 
#define FBINFO_HWACCEL_FILLRECT		0x0200 
#define FBINFO_HWACCEL_IMAGEBLIT	0x0400 
#define FBINFO_HWACCEL_ROTATE		0x0800 
#define FBINFO_HWACCEL_XPAN		0x1000 
#define FBINFO_HWACCEL_YPAN		0x2000 
#define FBINFO_HWACCEL_YWRAP		0x4000 

#define FBINFO_MISC_USEREVENT          0x10000 
#define FBINFO_MISC_TILEBLITTING       0x20000 


#define FBINFO_MISC_ALWAYS_SETPAR   0x40000


#define FBINFO_MISC_FIRMWARE        0x80000

#define FBINFO_FOREIGN_ENDIAN	0x100000

#define FBINFO_BE_MATH  0x100000


#define FBINFO_CAN_FORCE_OUTPUT     0x200000

struct fb_info {
	atomic_t count;
	int node;
	int flags;
	struct mutex lock;		
	struct mutex mm_lock;		
	struct fb_var_screeninfo var;	
	struct fb_fix_screeninfo fix;	
	struct fb_monspecs monspecs;	
	struct work_struct queue;	
	struct fb_pixmap pixmap;	
	struct fb_pixmap sprite;	
	struct fb_cmap cmap;		
	struct list_head modelist;      
	struct fb_videomode *mode;	

#ifdef CONFIG_FB_BACKLIGHT
	
	
	struct backlight_device *bl_dev;

	
	struct mutex bl_curve_mutex;	
	u8 bl_curve[FB_BACKLIGHT_LEVELS];
#endif
#ifdef CONFIG_FB_DEFERRED_IO
	struct delayed_work deferred_work;
	struct fb_deferred_io *fbdefio;
#endif

	struct fb_ops *fbops;
	struct device *device;		
	struct device *dev;		
	int class_flag;                    
#ifdef CONFIG_FB_TILEBLITTING
	struct fb_tile_ops *tileops;    
#endif
	union {
		char __iomem *screen_base;	
		char *screen_buffer;
	};
	unsigned long screen_size;	 
	void *pseudo_palette;		 
#define FBINFO_STATE_RUNNING	0
#define FBINFO_STATE_SUSPENDED	1
	u32 state;			
	void *fbcon_par;                
	
	void *par;
	
	struct apertures_struct {
		unsigned int count;
		struct aperture {
			resource_size_t base;
			resource_size_t size;
		} ranges[0];
	} *apertures;

	bool skip_vt_switch; 
};

static inline struct apertures_struct *alloc_apertures(unsigned int max_num) {
	struct apertures_struct *a = kzalloc(sizeof(struct apertures_struct)
			+ max_num * sizeof(struct aperture), GFP_KERNEL);
	if (!a)
		return NULL;
	a->count = max_num;
	return a;
}

#ifdef MODULE
#define FBINFO_DEFAULT	FBINFO_MODULE
#else
#define FBINFO_DEFAULT	0
#endif

// This will go away
#define FBINFO_FLAG_MODULE	FBINFO_MODULE
#define FBINFO_FLAG_DEFAULT	FBINFO_DEFAULT


#define STUPID_ACCELF_TEXT_SHIT

// This will go away
#if defined(__sparc__)



// This will go away
#define fb_readb sbus_readb
#define fb_readw sbus_readw
#define fb_readl sbus_readl
#define fb_readq sbus_readq
#define fb_writeb sbus_writeb
#define fb_writew sbus_writew
#define fb_writel sbus_writel
#define fb_writeq sbus_writeq
#define fb_memset sbus_memset_io
#define fb_memcpy_fromfb sbus_memcpy_fromio
#define fb_memcpy_tofb sbus_memcpy_toio

#elif defined(__i386__) || defined(__alpha__) || defined(__x86_64__) || defined(__hppa__) || defined(__sh__) || defined(__powerpc__) || defined(__avr32__) || defined(__bfin__) || defined(__arm__)

#define fb_readb __raw_readb
#define fb_readw __raw_readw
#define fb_readl __raw_readl
#define fb_readq __raw_readq
#define fb_writeb __raw_writeb
#define fb_writew __raw_writew
#define fb_writel __raw_writel
#define fb_writeq __raw_writeq
#define fb_memset memset_io
#define fb_memcpy_fromfb memcpy_fromio
#define fb_memcpy_tofb memcpy_toio

#else

#define fb_readb(addr) (*(volatile u8 *) (addr))
#define fb_readw(addr) (*(volatile u16 *) (addr))
#define fb_readl(addr) (*(volatile u32 *) (addr))
#define fb_readq(addr) (*(volatile u64 *) (addr))
#define fb_writeb(b,addr) (*(volatile u8 *) (addr) = (b))
#define fb_writew(b,addr) (*(volatile u16 *) (addr) = (b))
#define fb_writel(b,addr) (*(volatile u32 *) (addr) = (b))
#define fb_writeq(b,addr) (*(volatile u64 *) (addr) = (b))
#define fb_memset memset
#define fb_memcpy_fromfb memcpy
#define fb_memcpy_tofb memcpy

#endif

#define FB_LEFT_POS(p, bpp)          (fb_be_math(p) ? (32 - (bpp)) : 0)
#define FB_SHIFT_HIGH(p, val, bits)  (fb_be_math(p) ? (val) >> (bits) : \
						      (val) << (bits))
#define FB_SHIFT_LOW(p, val, bits)   (fb_be_math(p) ? (val) << (bits) : \
						      (val) >> (bits))

    

extern int fb_set_var(struct fb_info *info, struct fb_var_screeninfo *var); 
extern int fb_pan_display(struct fb_info *info, struct fb_var_screeninfo *var); 
extern int fb_blank(struct fb_info *info, int blank);
extern void cfb_fillrect(struct fb_info *info, const struct fb_fillrect *rect); 
extern void cfb_copyarea(struct fb_info *info, const struct fb_copyarea *area); 
extern void cfb_imageblit(struct fb_info *info, const struct fb_image *image);

extern void sys_fillrect(struct fb_info *info, const struct fb_fillrect *rect);
extern void sys_copyarea(struct fb_info *info, const struct fb_copyarea *area);
extern void sys_imageblit(struct fb_info *info, const struct fb_image *image);
extern ssize_t fb_sys_read(struct fb_info *info, char __user *buf,
			   size_t count, loff_t *ppos);
extern ssize_t fb_sys_write(struct fb_info *info, const char __user *buf,
			    size_t count, loff_t *ppos);


extern int register_framebuffer(struct fb_info *fb_info);
extern int unregister_framebuffer(struct fb_info *fb_info);
extern int unlink_framebuffer(struct fb_info *fb_info);
extern int remove_conflicting_framebuffers(struct apertures_struct *a,
					   const char *name, bool primary);
extern int fb_prepare_logo(struct fb_info *fb_info, int rotate);
extern int fb_show_logo(struct fb_info *fb_info, int rotate);
extern char* fb_get_buffer_offset(struct fb_info *info, struct fb_pixmap *buf, u32 size);
extern void fb_pad_unaligned_buffer(u8 *dst, u32 d_pitch, u8 *src, u32 idx,
				u32 height, u32 shift_high, u32 shift_low, u32 mod);
extern void fb_pad_aligned_buffer(u8 *dst, u32 d_pitch, u8 *src, u32 s_pitch, u32 height);
extern void fb_set_suspend(struct fb_info *info, int state);
extern int fb_get_color_depth(struct fb_var_screeninfo *var,
			      struct fb_fix_screeninfo *fix);
extern int fb_get_options(const char *name, char **option);
extern int fb_new_modelist(struct fb_info *info);

extern struct fb_info *registered_fb[FB_MAX];
extern int num_registered_fb;
extern struct class *fb_class;

extern int lock_fb_info(struct fb_info *info);

static inline void unlock_fb_info(struct fb_info *info)
{
	mutex_unlock(&info->lock);
}

static inline void __fb_pad_aligned_buffer(u8 *dst, u32 d_pitch,
					   u8 *src, u32 s_pitch, u32 height)
{
	u32 i, j;

	d_pitch -= s_pitch;

	for (i = height; i--; ) {
		
		for (j = 0; j < s_pitch; j++)
			*dst++ = *src++;
		dst += d_pitch;
	}
}


int fb_deferred_io_mmap(struct fb_info *info, struct vm_area_struct *vma);
extern void fb_deferred_io_init(struct fb_info *info);
extern void fb_deferred_io_open(struct fb_info *info,
				struct inode *inode,
				struct file *file);
extern void fb_deferred_io_cleanup(struct fb_info *info);
extern int fb_deferred_io_fsync(struct file *file, loff_t start,
				loff_t end, int datasync);

static inline bool fb_be_math(struct fb_info *info)
{
#ifdef CONFIG_FB_FOREIGN_ENDIAN
#if defined(CONFIG_FB_BOTH_ENDIAN)
	return info->flags & FBINFO_BE_MATH;
#elif defined(CONFIG_FB_BIG_ENDIAN)
	return true;
#elif defined(CONFIG_FB_LITTLE_ENDIAN)
	return false;
#endif 
#else
#ifdef __BIG_ENDIAN
	return true;
#else
	return false;
#endif 
#endif 
}


extern struct fb_info *framebuffer_alloc(size_t size, struct device *dev);
extern void framebuffer_release(struct fb_info *info);
extern int fb_init_device(struct fb_info *fb_info);
extern void fb_cleanup_device(struct fb_info *head);
extern void fb_bl_default_curve(struct fb_info *fb_info, u8 off, u8 min, u8 max);


#define FB_MAXTIMINGS		0
#define FB_VSYNCTIMINGS		1
#define FB_HSYNCTIMINGS		2
#define FB_DCLKTIMINGS		3
#define FB_IGNOREMON		0x100

#define FB_MODE_IS_UNKNOWN	0
#define FB_MODE_IS_DETAILED	1
#define FB_MODE_IS_STANDARD	2
#define FB_MODE_IS_VESA		4
#define FB_MODE_IS_CALCULATED	8
#define FB_MODE_IS_FIRST	16
#define FB_MODE_IS_FROM_VAR     32

extern int fbmon_dpms(const struct fb_info *fb_info);
extern int fb_get_mode(int flags, u32 val, struct fb_var_screeninfo *var,
		       struct fb_info *info);
extern int fb_validate_mode(const struct fb_var_screeninfo *var,
			    struct fb_info *info);
extern int fb_parse_edid(unsigned char *edid, struct fb_var_screeninfo *var);
extern const unsigned char *fb_firmware_edid(struct device *device);
extern void fb_edid_to_monspecs(unsigned char *edid,
				struct fb_monspecs *specs);
extern void fb_destroy_modedb(struct fb_videomode *modedb);
extern int fb_find_mode_cvt(struct fb_videomode *mode, int margins, int rb);
extern unsigned char *fb_ddc_read(struct i2c_adapter *adapter);

extern int of_get_fb_videomode(struct device_node *np,
			       struct fb_videomode *fb,
			       int index);
extern int fb_videomode_from_videomode(const struct videomode *vm,
				       struct fb_videomode *fbmode);


#define VESA_MODEDB_SIZE 43
#define DMT_SIZE 0x50

extern void fb_var_to_videomode(struct fb_videomode *mode,
				const struct fb_var_screeninfo *var);
extern void fb_videomode_to_var(struct fb_var_screeninfo *var,
				const struct fb_videomode *mode);
extern int fb_mode_is_equal(const struct fb_videomode *mode1,
			    const struct fb_videomode *mode2);
extern int fb_add_videomode(const struct fb_videomode *mode,
			    struct list_head *head);
extern void fb_delete_videomode(const struct fb_videomode *mode,
				struct list_head *head);
extern const struct fb_videomode *fb_match_mode(const struct fb_var_screeninfo *var,
						struct list_head *head);
extern const struct fb_videomode *fb_find_best_mode(const struct fb_var_screeninfo *var,
						    struct list_head *head);
extern const struct fb_videomode *fb_find_nearest_mode(const struct fb_videomode *mode,
						       struct list_head *head);
extern void fb_destroy_modelist(struct list_head *head);
extern void fb_videomode_to_modelist(const struct fb_videomode *modedb, int num,
				     struct list_head *head);
extern const struct fb_videomode *fb_find_best_display(const struct fb_monspecs *specs,
						       struct list_head *head);


extern int fb_alloc_cmap(struct fb_cmap *cmap, int len, int transp);
extern int fb_alloc_cmap_gfp(struct fb_cmap *cmap, int len, int transp, gfp_t flags);
extern void fb_dealloc_cmap(struct fb_cmap *cmap);
extern int fb_copy_cmap(const struct fb_cmap *from, struct fb_cmap *to);
extern int fb_cmap_to_user(const struct fb_cmap *from, struct fb_cmap_user *to);
extern int fb_set_cmap(struct fb_cmap *cmap, struct fb_info *fb_info);
extern int fb_set_user_cmap(struct fb_cmap_user *cmap, struct fb_info *fb_info);
extern const struct fb_cmap *fb_default_cmap(int len);
extern void fb_invert_cmaps(void);

struct fb_videomode {
	const char *name;	
	u32 refresh;		
	u32 xres;
	u32 yres;
	u32 pixclock;
	u32 left_margin;
	u32 right_margin;
	u32 upper_margin;
	u32 lower_margin;
	u32 hsync_len;
	u32 vsync_len;
	u32 sync;
	u32 vmode;
	u32 flag;
};

struct dmt_videomode {
	u32 dmt_id;
	u32 std_2byte_code;
	u32 cvt_3byte_code;
	const struct fb_videomode *mode;
};

extern const char *fb_mode_option;
extern const struct fb_videomode vesa_modes[];
extern const struct dmt_videomode dmt_modes[];

struct fb_modelist {
	struct list_head list;
	struct fb_videomode mode;
};

extern int fb_find_mode(struct fb_var_screeninfo *var,
			struct fb_info *info, const char *mode_option,
			const struct fb_videomode *db,
			unsigned int dbsize,
			const struct fb_videomode *default_mode,
			unsigned int default_bpp);


#define fb_err(fb_info, fmt, ...)					\
	pr_err("fb%d: " fmt, (fb_info)->node, ##__VA_ARGS__)
#define fb_notice(info, fmt, ...)					\
	pr_notice("fb%d: " fmt, (fb_info)->node, ##__VA_ARGS__)
#define fb_warn(fb_info, fmt, ...)					\
	pr_warn("fb%d: " fmt, (fb_info)->node, ##__VA_ARGS__)
#define fb_info(fb_info, fmt, ...)					\
	pr_info("fb%d: " fmt, (fb_info)->node, ##__VA_ARGS__)
#define fb_dbg(fb_info, fmt, ...)					\
	pr_debug("fb%d: " fmt, (fb_info)->node, ##__VA_ARGS__)

#endif 
