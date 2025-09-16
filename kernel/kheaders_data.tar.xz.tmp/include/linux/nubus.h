
#ifndef LINUX_NUBUS_H
#define LINUX_NUBUS_H

#include <asm/nubus.h>
#include <uapi/linux/nubus.h>

struct nubus_board {
	struct nubus_board* next;
	struct nubus_dev* first_dev;
	
	
	int slot;
	
	char name[64];

	
	unsigned char* fblock;
	
	unsigned char* directory;
	
	unsigned long slot_addr;
	
	unsigned long doffset;
	
	unsigned long rom_length;
	
	unsigned long crc;
	unsigned char rev;
	unsigned char format;
	unsigned char lanes;
};

struct nubus_dev {
	
	struct nubus_dev* next;
	
	struct proc_dir_entry* procdir;

	
	unsigned char resid;
	
	unsigned short category;
	unsigned short type;
	unsigned short dr_sw;
	unsigned short dr_hw;
	
	char name[64];
	
	unsigned char* driver;
	
	unsigned long iobase;
	unsigned long iosize;
	unsigned char flags, hwdevid;
	
	
	unsigned char* directory;
	
	struct nubus_board* board;
};


extern struct nubus_dev* nubus_devices;

extern struct nubus_board* nubus_boards;


void nubus_scan_bus(void);
#ifdef CONFIG_PROC_FS
extern void nubus_proc_init(void);
#else
static inline void nubus_proc_init(void) {}
#endif
int get_nubus_list(char *buf);
int nubus_proc_attach_device(struct nubus_dev *dev);

struct nubus_dev* nubus_find_device(unsigned short category,
				    unsigned short type,
				    unsigned short dr_hw,
				    unsigned short dr_sw,
				    const struct nubus_dev* from);
struct nubus_dev* nubus_find_type(unsigned short category,
				  unsigned short type,
				  const struct nubus_dev* from);

struct nubus_dev* nubus_find_slot(unsigned int slot,
				  const struct nubus_dev* from);




int nubus_get_root_dir(const struct nubus_board* board,
		       struct nubus_dir* dir);

int nubus_get_board_dir(const struct nubus_board* board,
			struct nubus_dir* dir);

int nubus_get_func_dir(const struct nubus_dev* dev,
		       struct nubus_dir* dir);


int nubus_readdir(struct nubus_dir* dir,
		  struct nubus_dirent* ent);
int nubus_find_rsrc(struct nubus_dir* dir,
		    unsigned char rsrc_type,
		    struct nubus_dirent* ent);
int nubus_rewinddir(struct nubus_dir* dir);


int nubus_get_subdir(const struct nubus_dirent* ent,
		     struct nubus_dir* dir);
void nubus_get_rsrc_mem(void* dest,
			const struct nubus_dirent *dirent,
			int len);
void nubus_get_rsrc_str(void* dest,
			const struct nubus_dirent *dirent,
			int maxlen);
#endif 
