



#ifndef __ACBUFFER_H__
#define __ACBUFFER_H__







struct acpi_fde_info {
	u32 floppy0;
	u32 floppy1;
	u32 floppy2;
	u32 floppy3;
	u32 tape;
};


struct acpi_grt_info {
	u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	u8 valid;
	u16 milliseconds;
	u16 timezone;
	u8 daylight;
	u8 reserved[3];
};



struct acpi_gtm_info {
	u32 pio_speed0;
	u32 dma_speed0;
	u32 pio_speed1;
	u32 dma_speed1;
	u32 flags;
};


struct acpi_pld_info {
	u8 revision;
	u8 ignore_color;
	u8 red;
	u8 green;
	u8 blue;
	u16 width;
	u16 height;
	u8 user_visible;
	u8 dock;
	u8 lid;
	u8 panel;
	u8 vertical_position;
	u8 horizontal_position;
	u8 shape;
	u8 group_orientation;
	u8 group_token;
	u8 group_position;
	u8 bay;
	u8 ejectable;
	u8 ospm_eject_required;
	u8 cabinet_number;
	u8 card_cage_number;
	u8 reference;
	u8 rotation;
	u8 order;
	u8 reserved;
	u16 vertical_offset;
	u16 horizontal_offset;
};


#define ACPI_PLD_REV1_BUFFER_SIZE               16	
#define ACPI_PLD_REV2_BUFFER_SIZE               20	
#define ACPI_PLD_BUFFER_SIZE                    20	



#define ACPI_PLD_GET_REVISION(dword)            ACPI_GET_BITS (dword, 0, ACPI_7BIT_MASK)
#define ACPI_PLD_SET_REVISION(dword,value)      ACPI_SET_BITS (dword, 0, ACPI_7BIT_MASK, value)	

#define ACPI_PLD_GET_IGNORE_COLOR(dword)        ACPI_GET_BITS (dword, 7, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_IGNORE_COLOR(dword,value)  ACPI_SET_BITS (dword, 7, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_RED(dword)                 ACPI_GET_BITS (dword, 8, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_RED(dword,value)           ACPI_SET_BITS (dword, 8, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_GREEN(dword)               ACPI_GET_BITS (dword, 16, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_GREEN(dword,value)         ACPI_SET_BITS (dword, 16, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_BLUE(dword)                ACPI_GET_BITS (dword, 24, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_BLUE(dword,value)          ACPI_SET_BITS (dword, 24, ACPI_8BIT_MASK, value)	



#define ACPI_PLD_GET_WIDTH(dword)               ACPI_GET_BITS (dword, 0, ACPI_16BIT_MASK)
#define ACPI_PLD_SET_WIDTH(dword,value)         ACPI_SET_BITS (dword, 0, ACPI_16BIT_MASK, value)	

#define ACPI_PLD_GET_HEIGHT(dword)              ACPI_GET_BITS (dword, 16, ACPI_16BIT_MASK)
#define ACPI_PLD_SET_HEIGHT(dword,value)        ACPI_SET_BITS (dword, 16, ACPI_16BIT_MASK, value)	



#define ACPI_PLD_GET_USER_VISIBLE(dword)        ACPI_GET_BITS (dword, 0, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_USER_VISIBLE(dword,value)  ACPI_SET_BITS (dword, 0, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_DOCK(dword)                ACPI_GET_BITS (dword, 1, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_DOCK(dword,value)          ACPI_SET_BITS (dword, 1, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_LID(dword)                 ACPI_GET_BITS (dword, 2, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_LID(dword,value)           ACPI_SET_BITS (dword, 2, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_PANEL(dword)               ACPI_GET_BITS (dword, 3, ACPI_3BIT_MASK)
#define ACPI_PLD_SET_PANEL(dword,value)         ACPI_SET_BITS (dword, 3, ACPI_3BIT_MASK, value)	

#define ACPI_PLD_GET_VERTICAL(dword)            ACPI_GET_BITS (dword, 6, ACPI_2BIT_MASK)
#define ACPI_PLD_SET_VERTICAL(dword,value)      ACPI_SET_BITS (dword, 6, ACPI_2BIT_MASK, value)	

#define ACPI_PLD_GET_HORIZONTAL(dword)          ACPI_GET_BITS (dword, 8, ACPI_2BIT_MASK)
#define ACPI_PLD_SET_HORIZONTAL(dword,value)    ACPI_SET_BITS (dword, 8, ACPI_2BIT_MASK, value)	

#define ACPI_PLD_GET_SHAPE(dword)               ACPI_GET_BITS (dword, 10, ACPI_4BIT_MASK)
#define ACPI_PLD_SET_SHAPE(dword,value)         ACPI_SET_BITS (dword, 10, ACPI_4BIT_MASK, value)	

#define ACPI_PLD_GET_ORIENTATION(dword)         ACPI_GET_BITS (dword, 14, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_ORIENTATION(dword,value)   ACPI_SET_BITS (dword, 14, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_TOKEN(dword)               ACPI_GET_BITS (dword, 15, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_TOKEN(dword,value)         ACPI_SET_BITS (dword, 15, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_POSITION(dword)            ACPI_GET_BITS (dword, 23, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_POSITION(dword,value)      ACPI_SET_BITS (dword, 23, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_BAY(dword)                 ACPI_GET_BITS (dword, 31, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_BAY(dword,value)           ACPI_SET_BITS (dword, 31, ACPI_1BIT_MASK, value)	



#define ACPI_PLD_GET_EJECTABLE(dword)           ACPI_GET_BITS (dword, 0, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_EJECTABLE(dword,value)     ACPI_SET_BITS (dword, 0, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_OSPM_EJECT(dword)          ACPI_GET_BITS (dword, 1, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_OSPM_EJECT(dword,value)    ACPI_SET_BITS (dword, 1, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_CABINET(dword)             ACPI_GET_BITS (dword, 2, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_CABINET(dword,value)       ACPI_SET_BITS (dword, 2, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_CARD_CAGE(dword)           ACPI_GET_BITS (dword, 10, ACPI_8BIT_MASK)
#define ACPI_PLD_SET_CARD_CAGE(dword,value)     ACPI_SET_BITS (dword, 10, ACPI_8BIT_MASK, value)	

#define ACPI_PLD_GET_REFERENCE(dword)           ACPI_GET_BITS (dword, 18, ACPI_1BIT_MASK)
#define ACPI_PLD_SET_REFERENCE(dword,value)     ACPI_SET_BITS (dword, 18, ACPI_1BIT_MASK, value)	

#define ACPI_PLD_GET_ROTATION(dword)            ACPI_GET_BITS (dword, 19, ACPI_4BIT_MASK)
#define ACPI_PLD_SET_ROTATION(dword,value)      ACPI_SET_BITS (dword, 19, ACPI_4BIT_MASK, value)	

#define ACPI_PLD_GET_ORDER(dword)               ACPI_GET_BITS (dword, 23, ACPI_5BIT_MASK)
#define ACPI_PLD_SET_ORDER(dword,value)         ACPI_SET_BITS (dword, 23, ACPI_5BIT_MASK, value)	



#define ACPI_PLD_GET_VERT_OFFSET(dword)         ACPI_GET_BITS (dword, 0, ACPI_16BIT_MASK)
#define ACPI_PLD_SET_VERT_OFFSET(dword,value)   ACPI_SET_BITS (dword, 0, ACPI_16BIT_MASK, value)	

#define ACPI_PLD_GET_HORIZ_OFFSET(dword)        ACPI_GET_BITS (dword, 16, ACPI_16BIT_MASK)
#define ACPI_PLD_SET_HORIZ_OFFSET(dword,value)  ACPI_SET_BITS (dword, 16, ACPI_16BIT_MASK, value)	

#endif				
