

#ifndef __MTD_TRANS_H__
#define __MTD_TRANS_H__

#include <linux/mutex.h>
#include <linux/kref.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>

struct hd_geometry;
struct mtd_info;
struct mtd_blktrans_ops;
struct file;
struct inode;

struct mtd_blktrans_dev {
	struct mtd_blktrans_ops *tr;
	struct list_head list;
	struct mtd_info *mtd;
	struct mutex lock;
	int devnum;
	bool bg_stop;
	unsigned long size;
	int readonly;
	int open;
	struct kref ref;
	struct gendisk *disk;
	struct attribute_group *disk_attributes;
	struct workqueue_struct *wq;
	struct work_struct work;
	struct request_queue *rq;
	spinlock_t queue_lock;
	void *priv;
	fmode_t file_mode;
};

struct mtd_blktrans_ops {
	char *name;
	int major;
	int part_bits;
	int blksize;
	int blkshift;

	
	int (*readsect)(struct mtd_blktrans_dev *dev,
		    unsigned long block, char *buffer);
	int (*writesect)(struct mtd_blktrans_dev *dev,
		     unsigned long block, char *buffer);
	int (*discard)(struct mtd_blktrans_dev *dev,
		       unsigned long block, unsigned nr_blocks);
	void (*background)(struct mtd_blktrans_dev *dev);

	
	int (*getgeo)(struct mtd_blktrans_dev *dev, struct hd_geometry *geo);
	int (*flush)(struct mtd_blktrans_dev *dev);

	
	int (*open)(struct mtd_blktrans_dev *dev);
	void (*release)(struct mtd_blktrans_dev *dev);

	
	void (*add_mtd)(struct mtd_blktrans_ops *tr, struct mtd_info *mtd);
	void (*remove_dev)(struct mtd_blktrans_dev *dev);

	struct list_head devs;
	struct list_head list;
	struct module *owner;
};

extern int register_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int deregister_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int add_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);
extern int del_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);
extern int mtd_blktrans_cease_background(struct mtd_blktrans_dev *dev);


#endif 
