

#ifndef _GPMC_OMAP_H_
#define _GPMC_OMAP_H_


#define GPMC_CS_NUM		8


struct gpmc_bool_timings {
	bool cycle2cyclediffcsen;
	bool cycle2cyclesamecsen;
	bool we_extra_delay;
	bool oe_extra_delay;
	bool adv_extra_delay;
	bool cs_extra_delay;
	bool time_para_granularity;
};


struct gpmc_timings {
	
	u32 sync_clk;

	
	u32 cs_on;		
	u32 cs_rd_off;		
	u32 cs_wr_off;		

	
	u32 adv_on;		
	u32 adv_rd_off;		
	u32 adv_wr_off;		
	u32 adv_aad_mux_on;	
	u32 adv_aad_mux_rd_off;	
	u32 adv_aad_mux_wr_off;	

	
	u32 we_on;		
	u32 we_off;		

	
	u32 oe_on;		
	u32 oe_off;		
	u32 oe_aad_mux_on;	
	u32 oe_aad_mux_off;	

	
	u32 page_burst_access;	
	u32 access;		
	u32 rd_cycle;		
	u32 wr_cycle;		

	u32 bus_turnaround;
	u32 cycle2cycle_delay;

	u32 wait_monitoring;
	u32 clk_activation;

	
	u32 wr_access;		
	u32 wr_data_mux_bus;	

	struct gpmc_bool_timings bool_timings;
};


struct gpmc_device_timings {
	u32 t_ceasu;	
	u32 t_avdasu;	
	
	u32 t_avdp_r;	
	u32 t_avdp_w;
	u32 t_aavdh;	
	u32 t_oeasu;	
	u32 t_aa;	
	u32 t_iaa;	
	u32 t_oe;	
	u32 t_ce;	
	u32 t_rd_cycle;	
	u32 t_cez_r;	
	u32 t_cez_w;	
	u32 t_oez;	
	u32 t_weasu;	
	u32 t_wpl;	
	u32 t_wph;	
	u32 t_wr_cycle;	

	u32 clk;
	u32 t_bacc;	
	u32 t_ces;	
	u32 t_avds;	
	u32 t_avdh;	
	u32 t_ach;	
	u32 t_rdyo;	

	u32 t_ce_rdyz;	
	u32 t_ce_avd;	

	
	u8 cyc_aavdh_oe;
	u8 cyc_aavdh_we;
	u8 cyc_oe;	
	u8 cyc_wpl;	
	u32 cyc_iaa;	

	
	bool ce_xdelay;
	bool avd_xdelay;
	bool oe_xdelay;
	bool we_xdelay;
};

#define GPMC_BURST_4			4	
#define GPMC_BURST_8			8	
#define GPMC_BURST_16			16	
#define GPMC_DEVWIDTH_8BIT		1	
#define GPMC_DEVWIDTH_16BIT		2	
#define GPMC_MUX_AAD			1	
#define GPMC_MUX_AD			2	

struct gpmc_settings {
	bool burst_wrap;	
	bool burst_read;	
	bool burst_write;	
	bool device_nand;	
	bool sync_read;		
	bool sync_write;	
	bool wait_on_read;	
	bool wait_on_write;	
	u32 burst_len;		
	u32 device_width;	
	u32 mux_add_data;	
	u32 wait_pin;		
};


struct gpmc_omap_cs_data {
	bool valid;			
	bool is_nand;			
	struct gpmc_settings *settings;
	struct gpmc_device_timings *device_timings;
	struct gpmc_timings *gpmc_timings;
	struct platform_device *pdev;	
	unsigned int pdata_size;
};

struct gpmc_omap_platform_data {
	struct gpmc_omap_cs_data cs[GPMC_CS_NUM];
};

#endif 
