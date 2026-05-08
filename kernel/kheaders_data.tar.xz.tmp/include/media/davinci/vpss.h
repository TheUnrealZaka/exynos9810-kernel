
#ifndef _VPSS_H
#define _VPSS_H


enum vpss_ccdc_source_sel {
	VPSS_CCDCIN,
	VPSS_HSSIIN,
	VPSS_PGLPBK,	
	VPSS_CCDCPG	
};

struct vpss_sync_pol {
	unsigned int ccdpg_hdpol:1;
	unsigned int ccdpg_vdpol:1;
};

struct vpss_pg_frame_size {
	short hlpfr;
	short pplen;
};


enum vpss_clock_sel {
	
	VPSS_CCDC_CLOCK,
	VPSS_IPIPE_CLOCK,
	VPSS_H3A_CLOCK,
	VPSS_CFALD_CLOCK,
	
	VPSS_VENC_CLOCK_SEL,
	VPSS_VPBE_CLOCK,
	
	VPSS_IPIPEIF_CLOCK,
	VPSS_RSZ_CLOCK,
	VPSS_BL_CLOCK,
	
	VPSS_PCLK_INTERNAL,
	
	VPSS_PSYNC_CLOCK_SEL,
	VPSS_LDC_CLOCK_SEL,
	VPSS_OSD_CLOCK_SEL,
	VPSS_FDIF_CLOCK,
	VPSS_LDC_CLOCK
};


int vpss_select_ccdc_source(enum vpss_ccdc_source_sel src_sel);

int vpss_enable_clock(enum vpss_clock_sel clock_sel, int en);

void dm365_vpss_set_sync_pol(struct vpss_sync_pol);

void dm365_vpss_set_pg_frame_size(struct vpss_pg_frame_size);


enum vpss_wbl_sel {
	VPSS_PCR_AEW_WBL_0 = 16,
	VPSS_PCR_AF_WBL_0,
	VPSS_PCR_RSZ4_WBL_0,
	VPSS_PCR_RSZ3_WBL_0,
	VPSS_PCR_RSZ2_WBL_0,
	VPSS_PCR_RSZ1_WBL_0,
	VPSS_PCR_PREV_WBL_0,
	VPSS_PCR_CCDC_WBL_O,
};

int vpss_clear_wbl_overflow(enum vpss_wbl_sel wbl_sel);


void vpss_set_sync_pol(struct vpss_sync_pol sync);

void vpss_set_pg_frame_size(struct vpss_pg_frame_size frame_size);

int vpss_dma_complete_interrupt(void);

#endif
