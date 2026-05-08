

#ifndef CW1200_PLAT_H_INCLUDED
#define CW1200_PLAT_H_INCLUDED

struct cw1200_platform_data_spi {
	u8 spi_bits_per_word;           
	u16 ref_clk;                    

	
	bool have_5ghz;
	int reset;                     
	int powerup;                   
	int (*power_ctrl)(const struct cw1200_platform_data_spi *pdata,
			  bool enable); 
	int (*clk_ctrl)(const struct cw1200_platform_data_spi *pdata,
			bool enable); 
	const u8 *macaddr;  
	const char *sdd_file;  
};

struct cw1200_platform_data_sdio {
	u16 ref_clk;                    

	
	bool have_5ghz;
	bool no_nptb;       
	int reset;          
	int powerup;        
	int irq;            
	int (*power_ctrl)(const struct cw1200_platform_data_sdio *pdata,
			  bool enable); 
	int (*clk_ctrl)(const struct cw1200_platform_data_sdio *pdata,
			bool enable); 
	const u8 *macaddr;  
	const char *sdd_file;  
};






void __init cw1200_sdio_set_platform_data(struct cw1200_platform_data_sdio *pdata);

#endif 
