

#ifndef __ATMEL_H__
#define __ATMEL_H__

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/serial.h>

 
struct at91_cf_data {
	int	irq_pin;		
	int	det_pin;		
	int	vcc_pin;		
	int	rst_pin;		
	u8	chipselect;		
	u8	flags;
#define AT91_CF_TRUE_IDE	0x01
#define AT91_IDE_SWAP_A0_A2	0x02
};

 
struct atmel_nand_data {
	int		enable_pin;		
	int		det_pin;		
	int		rdy_pin;		
	u8		rdy_pin_active_low;	
	u8		ale;			
	u8		cle;			
	u8		bus_width_16;		
	u8		ecc_mode;		
	u8		on_flash_bbt;		
	struct mtd_partition *parts;
	unsigned int	num_parts;
	bool		has_dma;		

	
	bool		need_reset_workaround;
};

 
struct atmel_uart_data {
	int			num;		
	short			use_dma_tx;	
	short			use_dma_rx;	
	void __iomem		*regs;		
	struct serial_rs485	rs485;		
};


extern int at91_suspend_entering_slow_clock(void);

#endif 
