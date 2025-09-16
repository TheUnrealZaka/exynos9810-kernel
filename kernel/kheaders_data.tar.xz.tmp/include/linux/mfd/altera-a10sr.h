

#ifndef __MFD_ALTERA_A10SR_H
#define __MFD_ALTERA_A10SR_H

#include <linux/completion.h>
#include <linux/list.h>
#include <linux/mfd/core.h>
#include <linux/regmap.h>
#include <linux/slab.h>


#define  WRITE_REG_MASK              0xFE

#define  READ_REG_MASK               0x01

#define ALTR_A10SR_BITS_PER_REGISTER  8

#define ALTR_A10SR_REG_OFFSET(X)     (((X) / ALTR_A10SR_BITS_PER_REGISTER) << 1)
#define ALTR_A10SR_REG_BIT(X)        ((X) % ALTR_A10SR_BITS_PER_REGISTER)
#define ALTR_A10SR_REG_BIT_CHG(X, Y) ((X) << ALTR_A10SR_REG_BIT(Y))
#define ALTR_A10SR_REG_BIT_MASK(X)   (1 << ALTR_A10SR_REG_BIT(X))


#define ALTR_A10SR_NOP                0x00    
#define ALTR_A10SR_VERSION_READ       0x00    

#define ALTR_A10SR_LED_REG            0x02    

#define ALTR_A10SR_LED_VALID_SHIFT        4       
#define ALTR_A10SR_OUT_VALID_RANGE_LO     ALTR_A10SR_LED_VALID_SHIFT
#define ALTR_A10SR_OUT_VALID_RANGE_HI     7

#define ALTR_A10SR_PBDSW_REG          0x04    
#define ALTR_A10SR_PBDSW_IRQ_REG      0x06    

#define ALTR_A10SR_IN_VALID_RANGE_LO      8
#define ALTR_A10SR_IN_VALID_RANGE_HI      15

#define ALTR_A10SR_PWR_GOOD1_REG      0x08    
#define ALTR_A10SR_PWR_GOOD2_REG      0x0A    
#define ALTR_A10SR_PWR_GOOD3_REG      0x0C    
#define ALTR_A10SR_FMCAB_REG          0x0E    
#define ALTR_A10SR_HPS_RST_REG        0x10    
#define ALTR_A10SR_USB_QSPI_REG       0x12    
#define ALTR_A10SR_SFPA_REG           0x14    
#define ALTR_A10SR_SFPB_REG           0x16    
#define ALTR_A10SR_I2C_M_REG          0x18    
#define ALTR_A10SR_WARM_RST_REG       0x1A    
#define ALTR_A10SR_WR_KEY_REG         0x1C    
#define ALTR_A10SR_PMBUS_REG          0x1E    


struct altr_a10sr {
	struct device *dev;
	struct regmap *regmap;
};

#endif 
