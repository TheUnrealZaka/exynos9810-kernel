
#ifndef CMDLINEPARSEH
#define CMDLINEPARSEH

#include <linux/blkdev.h>
#include <linux/fs.h>
#include <linux/slab.h>


#define PF_RDONLY                   0x01 
#define PF_POWERUP_LOCK             0x02 

struct cmdline_subpart {
	char name[BDEVNAME_SIZE]; 
	sector_t from;
	sector_t size;
	int flags;
	struct cmdline_subpart *next_subpart;
};

struct cmdline_parts {
	char name[BDEVNAME_SIZE]; 
	unsigned int nr_subparts;
	struct cmdline_subpart *subpart;
	struct cmdline_parts *next_parts;
};

void cmdline_parts_free(struct cmdline_parts **parts);

int cmdline_parts_parse(struct cmdline_parts **parts, const char *cmdline);

struct cmdline_parts *cmdline_parts_find(struct cmdline_parts *parts,
					 const char *bdev);

int cmdline_parts_set(struct cmdline_parts *parts, sector_t disk_size,
		      int slot,
		      int (*add_part)(int, struct cmdline_subpart *, void *),
		      void *param);

#endif 
