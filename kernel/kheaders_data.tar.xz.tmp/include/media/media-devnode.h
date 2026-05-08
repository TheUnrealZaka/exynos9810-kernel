

#ifndef _MEDIA_DEVNODE_H
#define _MEDIA_DEVNODE_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

struct media_device;


#define MEDIA_FLAG_REGISTERED	0


struct media_file_operations {
	struct module *owner;
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*open) (struct file *);
	int (*release) (struct file *);
};


struct media_devnode {
	struct media_device *media_dev;

	
	const struct media_file_operations *fops;

	
	struct device dev;		
	struct cdev cdev;		
	struct device *parent;		

	
	int minor;
	unsigned long flags;		

	
	void (*release)(struct media_devnode *devnode);
};


#define to_media_devnode(cd) container_of(cd, struct media_devnode, dev)


int __must_check media_devnode_register(struct media_device *mdev,
					struct media_devnode *devnode,
					struct module *owner);


void media_devnode_unregister_prepare(struct media_devnode *devnode);


void media_devnode_unregister(struct media_devnode *devnode);


static inline struct media_devnode *media_devnode_data(struct file *filp)
{
	return filp->private_data;
}


static inline int media_devnode_is_registered(struct media_devnode *devnode)
{
	if (!devnode)
		return false;

	return test_bit(MEDIA_FLAG_REGISTERED, &devnode->flags);
}

#endif 
