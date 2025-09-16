

#ifndef __MG_DISK_H__
#define __MG_DISK_H__


#define MG_DEV_NAME "mg_disk"


#define MG_RST_PIN	"mg_rst"

#define MG_RSTOUT_PIN	"mg_rstout"



#define MG_BOOT_DEV		(1 << 0)

#define MG_STORAGE_DEV		(1 << 1)

#define MG_STORAGE_DEV_SKIP_RST	(1 << 2)


struct mg_drv_data {
	
	u32 use_polling;

	
	u32 dev_attr;

	
	void *host;
};

#endif
