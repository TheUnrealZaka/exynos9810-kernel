
#ifndef __LINUX_IPMI_FRU_H__
#define __LINUX_IPMI_FRU_H__
#ifdef __KERNEL__
#  include <linux/types.h>
#  include <linux/string.h>
#else
#  include <stdint.h>
#  include <string.h>
#endif




struct fru_common_header {
	uint8_t format;			
	uint8_t internal_use_off;	
	uint8_t chassis_info_off;	
	uint8_t board_area_off;		
	uint8_t product_area_off;	
	uint8_t multirecord_off;	
	uint8_t pad;			
	uint8_t checksum;		
};






struct fru_type_length {
	uint8_t type_length;
	uint8_t data[0];
};


struct fru_board_info_area {
	uint8_t format;			
	uint8_t area_len;		
	uint8_t language;		
	uint8_t mfg_date[3];		
	struct fru_type_length tl[0];	

	
};

enum fru_type {
	FRU_TYPE_BINARY		= 0x00,
	FRU_TYPE_BCDPLUS	= 0x40,
	FRU_TYPE_ASCII6		= 0x80,
	FRU_TYPE_ASCII		= 0xc0, 
};


static inline struct fru_board_info_area *fru_get_board_area(
	const struct fru_common_header *header)
{
	
	return (struct fru_board_info_area *)(header + header->board_area_off);
}

static inline int fru_type(struct fru_type_length *tl)
{
	return tl->type_length & 0xc0;
}

static inline int fru_length(struct fru_type_length *tl)
{
	return (tl->type_length & 0x3f) + 1; 
}


static inline int fru_strlen(struct fru_type_length *tl)
{
	return fru_length(tl) - 1;
}

static inline char *fru_strcpy(char *dest, struct fru_type_length *tl)
{
	int len = fru_strlen(tl);
	memcpy(dest, tl->data, len);
	dest[len] = '\0';
	return dest;
}

static inline struct fru_type_length *fru_next_tl(struct fru_type_length *tl)
{
	return tl + fru_length(tl);
}

static inline int fru_is_eof(struct fru_type_length *tl)
{
	return tl->type_length == 0xc1;
}


extern int fru_header_cksum_ok(struct fru_common_header *header);
extern int fru_bia_cksum_ok(struct fru_board_info_area *bia);


extern char *fru_get_board_manufacturer(struct fru_common_header *header);
extern char *fru_get_product_name(struct fru_common_header *header);
extern char *fru_get_serial_number(struct fru_common_header *header);
extern char *fru_get_part_number(struct fru_common_header *header);


extern void *fru_alloc(size_t size);

#endif 
