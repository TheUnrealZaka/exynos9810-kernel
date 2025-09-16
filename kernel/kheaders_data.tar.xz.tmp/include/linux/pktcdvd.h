
#ifndef __PKTCDVD_H
#define __PKTCDVD_H

#include <linux/blkdev.h>
#include <linux/completion.h>
#include <linux/cdrom.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/mempool.h>
#include <uapi/linux/pktcdvd.h>


#define PKT_WRITE_CONGESTION_ON    10000
#define PKT_WRITE_CONGESTION_OFF   9000


struct packet_settings
{
	__u32			size;		
	__u8			fp;		
	__u8			link_loss;	
	__u8			write_type;
	__u8			track_mode;
	__u8			block_mode;
};


struct packet_stats
{
	unsigned long		pkt_started;
	unsigned long		pkt_ended;
	unsigned long		secs_w;
	unsigned long		secs_rg;
	unsigned long		secs_r;
};

struct packet_cdrw
{
	struct list_head	pkt_free_list;
	struct list_head	pkt_active_list;
	spinlock_t		active_list_lock; 
	struct task_struct	*thread;
	atomic_t		pending_bios;
};


#define HI_SPEED_SWITCH 512

struct packet_iosched
{
	atomic_t		attention;	
	int			writing;	
	spinlock_t		lock;		
	struct bio_list		read_queue;
	struct bio_list		write_queue;
	sector_t		last_write;	
	int			successive_reads;
};


#if (PAGE_SIZE % CD_FRAMESIZE) != 0
#error "PAGE_SIZE must be a multiple of CD_FRAMESIZE"
#endif
#define PACKET_MAX_SIZE		128
#define FRAMES_PER_PAGE		(PAGE_SIZE / CD_FRAMESIZE)
#define PACKET_MAX_SECTORS	(PACKET_MAX_SIZE * CD_FRAMESIZE >> 9)

enum packet_data_state {
	PACKET_IDLE_STATE,			
	PACKET_WAITING_STATE,			
						
						
	PACKET_READ_WAIT_STATE,			
	PACKET_WRITE_WAIT_STATE,		
	PACKET_RECOVERY_STATE,			
	PACKET_FINISHED_STATE,			

	PACKET_NUM_STATES			
};


struct pktcdvd_device;

struct packet_data
{
	struct list_head	list;

	spinlock_t		lock;		
						

	struct bio_list		orig_bios;	
						
	int			write_size;	
						

	struct bio		*w_bio;		
						
						
	sector_t		sector;		
	int			frames;		

	enum packet_data_state	state;		
	atomic_t		run_sm;		
						
	long			sleep_time;	
						

	atomic_t		io_wait;	
	atomic_t		io_errors;	

	struct bio		*r_bios[PACKET_MAX_SIZE]; 
	struct page		*pages[PACKET_MAX_SIZE / FRAMES_PER_PAGE];

	int			cache_valid;	
						
						

	int			id;		
	struct pktcdvd_device	*pd;
};

struct pkt_rb_node {
	struct rb_node		rb_node;
	struct bio		*bio;
};

struct packet_stacked_data
{
	struct bio		*bio;		
	struct pktcdvd_device	*pd;
};
#define PSD_POOL_SIZE		64

struct pktcdvd_kobj
{
	struct kobject		kobj;
	struct pktcdvd_device	*pd;
};
#define to_pktcdvdkobj(_k) \
  ((struct pktcdvd_kobj*)container_of(_k,struct pktcdvd_kobj,kobj))

struct pktcdvd_device
{
	struct block_device	*bdev;		
	dev_t			pkt_dev;	
	char			name[20];
	struct packet_settings	settings;
	struct packet_stats	stats;
	int			refcnt;		
	int			write_speed;	
	int			read_speed;	
	unsigned long		offset;		
	__u8			mode_offset;	
	__u8			type;
	unsigned long		flags;
	__u16			mmc3_profile;
	__u32			nwa;		
	__u32			lra;		
	struct packet_cdrw	cdrw;
	wait_queue_head_t	wqueue;

	spinlock_t		lock;		
	struct rb_root		bio_queue;	
	int			bio_queue_size;	
	sector_t		current_sector;	
	atomic_t		scan_queue;	
						
	mempool_t		*rb_pool;	

	struct packet_iosched   iosched;
	struct gendisk		*disk;

	int			write_congestion_off;
	int			write_congestion_on;

	struct device		*dev;		
	struct pktcdvd_kobj	*kobj_stat;	
	struct pktcdvd_kobj	*kobj_wqueue;	

	struct dentry		*dfs_d_root;	
	struct dentry		*dfs_f_info;	
};

#endif 
