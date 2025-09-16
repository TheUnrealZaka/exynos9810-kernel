#ifndef _LINUX_BADBLOCKS_H
#define _LINUX_BADBLOCKS_H

#include <linux/seqlock.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/types.h>

#define BB_LEN_MASK	(0x00000000000001FFULL)
#define BB_OFFSET_MASK	(0x7FFFFFFFFFFFFE00ULL)
#define BB_ACK_MASK	(0x8000000000000000ULL)
#define BB_MAX_LEN	512
#define BB_OFFSET(x)	(((x) & BB_OFFSET_MASK) >> 9)
#define BB_LEN(x)	(((x) & BB_LEN_MASK) + 1)
#define BB_ACK(x)	(!!((x) & BB_ACK_MASK))
#define BB_MAKE(a, l, ack) (((a)<<9) | ((l)-1) | ((u64)(!!(ack)) << 63))


#define MAX_BADBLOCKS	(PAGE_SIZE/8)

struct badblocks {
	struct device *dev;	
	int count;		
	int unacked_exist;	
	int shift;		
	u64 *page;		
	int changed;
	seqlock_t lock;
	sector_t sector;
	sector_t size;		
};

int badblocks_check(struct badblocks *bb, sector_t s, int sectors,
		   sector_t *first_bad, int *bad_sectors);
int badblocks_set(struct badblocks *bb, sector_t s, int sectors,
			int acknowledged);
int badblocks_clear(struct badblocks *bb, sector_t s, int sectors);
void ack_all_badblocks(struct badblocks *bb);
ssize_t badblocks_show(struct badblocks *bb, char *page, int unack);
ssize_t badblocks_store(struct badblocks *bb, const char *page, size_t len,
			int unack);
int badblocks_init(struct badblocks *bb, int enable);
void badblocks_exit(struct badblocks *bb);
struct device;
int devm_init_badblocks(struct device *dev, struct badblocks *bb);
static inline void devm_exit_badblocks(struct device *dev, struct badblocks *bb)
{
	if (bb->dev != dev) {
		dev_WARN_ONCE(dev, 1, "%s: badblocks instance not associated\n",
				__func__);
		return;
	}
	badblocks_exit(bb);
}
#endif
