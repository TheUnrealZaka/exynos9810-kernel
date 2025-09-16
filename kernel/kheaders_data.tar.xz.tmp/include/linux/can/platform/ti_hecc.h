#ifndef _CAN_PLATFORM_TI_HECC_H
#define _CAN_PLATFORM_TI_HECC_H




struct ti_hecc_platform_data {
	u32 scc_hecc_offset;
	u32 scc_ram_offset;
	u32 hecc_ram_offset;
	u32 mbx_offset;
	u32 int_line;
	u32 version;
	void (*transceiver_switch) (int);
};
#endif 
