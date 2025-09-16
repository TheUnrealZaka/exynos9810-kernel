
#ifndef __SDB_H__
#define __SDB_H__
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif




struct sdb_product {
	uint64_t		vendor_id;	
	uint32_t		device_id;	
	uint32_t		version;	
	uint32_t		date;		
	uint8_t			name[19];	
	uint8_t			record_type;	
};


struct sdb_component {
	uint64_t		addr_first;	
	uint64_t		addr_last;	
	struct sdb_product	product;	
};


enum sdb_record_type {
	sdb_type_interconnect	= 0x00,
	sdb_type_device		= 0x01,
	sdb_type_bridge		= 0x02,
	sdb_type_integration	= 0x80,
	sdb_type_repo_url	= 0x81,
	sdb_type_synthesis	= 0x82,
	sdb_type_empty		= 0xFF,
};


#define				SDB_MAGIC	0x5344422d 
struct sdb_interconnect {
	uint32_t		sdb_magic;	
	uint16_t		sdb_records;	
	uint8_t			sdb_version;	
	uint8_t			sdb_bus_type;	
	struct sdb_component	sdb_component;	
};


struct sdb_device {
	uint16_t		abi_class;	
	uint8_t			abi_ver_major;	
	uint8_t			abi_ver_minor;	
	uint32_t		bus_specific;	
	struct sdb_component	sdb_component;	
};


struct sdb_bridge {
	uint64_t		sdb_child;	
	struct sdb_component	sdb_component;	
};


struct sdb_integration {
	uint8_t			reserved[24];	
	struct sdb_product	product;	
};


struct sdb_repo_url {
	uint8_t			repo_url[63];	
	uint8_t			record_type;	
};


struct sdb_synthesis {
	uint8_t			syn_name[16];	
	uint8_t			commit_id[16];	
	uint8_t			tool_name[8];	
	uint32_t		tool_version;	
	uint32_t		date;		
	uint8_t			user_name[15];	
	uint8_t			record_type;	
};


struct sdb_empty {
	uint8_t			reserved[63];	
	uint8_t			record_type;	
};


enum sdb_bus_type {
	sdb_wishbone = 0x00,
	sdb_data     = 0x01,
};

#define SDB_WB_WIDTH_MASK	0x0f
#define SDB_WB_ACCESS8			0x01
#define SDB_WB_ACCESS16			0x02
#define SDB_WB_ACCESS32			0x04
#define SDB_WB_ACCESS64			0x08
#define SDB_WB_LITTLE_ENDIAN	0x80

#define SDB_DATA_READ		0x04
#define SDB_DATA_WRITE		0x02
#define SDB_DATA_EXEC		0x01

#endif 
