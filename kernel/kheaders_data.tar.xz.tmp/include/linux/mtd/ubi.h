

#ifndef __LINUX_UBI_H__
#define __LINUX_UBI_H__

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/scatterlist.h>
#include <mtd/ubi-user.h>


#define UBI_ALL -1


#define UBI_MAX_SG_COUNT 64


enum {
	UBI_READONLY = 1,
	UBI_READWRITE,
	UBI_EXCLUSIVE,
	UBI_METAONLY
};


struct ubi_volume_info {
	int ubi_num;
	int vol_id;
	int size;
	long long used_bytes;
	int used_ebs;
	int vol_type;
	int corrupted;
	int upd_marker;
	int alignment;
	int usable_leb_size;
	int name_len;
	const char *name;
	dev_t cdev;
};


struct ubi_sgl {
	int list_pos;
	int page_pos;
	struct scatterlist sg[UBI_MAX_SG_COUNT];
};


static inline void ubi_sgl_init(struct ubi_sgl *usgl)
{
	usgl->list_pos = 0;
	usgl->page_pos = 0;
}


struct ubi_device_info {
	int ubi_num;
	int leb_size;
	int leb_start;
	int min_io_size;
	int max_write_size;
	int ro_mode;
	dev_t cdev;
};


enum {
	UBI_VOLUME_ADDED,
	UBI_VOLUME_REMOVED,
	UBI_VOLUME_RESIZED,
	UBI_VOLUME_RENAMED,
	UBI_VOLUME_UPDATED,
};


struct ubi_notification {
	struct ubi_device_info di;
	struct ubi_volume_info vi;
};


struct ubi_volume_desc;

int ubi_get_device_info(int ubi_num, struct ubi_device_info *di);
void ubi_get_volume_info(struct ubi_volume_desc *desc,
			 struct ubi_volume_info *vi);
struct ubi_volume_desc *ubi_open_volume(int ubi_num, int vol_id, int mode);
struct ubi_volume_desc *ubi_open_volume_nm(int ubi_num, const char *name,
					   int mode);
struct ubi_volume_desc *ubi_open_volume_path(const char *pathname, int mode);

int ubi_register_volume_notifier(struct notifier_block *nb,
				 int ignore_existing);
int ubi_unregister_volume_notifier(struct notifier_block *nb);

void ubi_close_volume(struct ubi_volume_desc *desc);
int ubi_leb_read(struct ubi_volume_desc *desc, int lnum, char *buf, int offset,
		 int len, int check);
int ubi_leb_read_sg(struct ubi_volume_desc *desc, int lnum, struct ubi_sgl *sgl,
		   int offset, int len, int check);
int ubi_leb_write(struct ubi_volume_desc *desc, int lnum, const void *buf,
		  int offset, int len);
int ubi_leb_change(struct ubi_volume_desc *desc, int lnum, const void *buf,
		   int len);
int ubi_leb_erase(struct ubi_volume_desc *desc, int lnum);
int ubi_leb_unmap(struct ubi_volume_desc *desc, int lnum);
int ubi_leb_map(struct ubi_volume_desc *desc, int lnum);
int ubi_is_mapped(struct ubi_volume_desc *desc, int lnum);
int ubi_sync(int ubi_num);
int ubi_flush(int ubi_num, int vol_id, int lnum);


static inline int ubi_read(struct ubi_volume_desc *desc, int lnum, char *buf,
			   int offset, int len)
{
	return ubi_leb_read(desc, lnum, buf, offset, len, 0);
}


static inline int ubi_read_sg(struct ubi_volume_desc *desc, int lnum,
			      struct ubi_sgl *sgl, int offset, int len)
{
	return ubi_leb_read_sg(desc, lnum, sgl, offset, len, 0);
}
#endif 
