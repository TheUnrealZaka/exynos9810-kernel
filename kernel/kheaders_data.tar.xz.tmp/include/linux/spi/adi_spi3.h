

#ifndef _ADI_SPI3_H_
#define _ADI_SPI3_H_

#include <linux/types.h>


#define SPI_CTL_EN                  0x00000001    
#define SPI_CTL_MSTR                0x00000002    
#define SPI_CTL_PSSE                0x00000004    
#define SPI_CTL_ODM                 0x00000008    
#define SPI_CTL_CPHA                0x00000010    
#define SPI_CTL_CPOL                0x00000020    
#define SPI_CTL_ASSEL               0x00000040    
#define SPI_CTL_SELST               0x00000080    
#define SPI_CTL_EMISO               0x00000100    
#define SPI_CTL_SIZE                0x00000600    
#define SPI_CTL_SIZE08              0x00000000    
#define SPI_CTL_SIZE16              0x00000200    
#define SPI_CTL_SIZE32              0x00000400    
#define SPI_CTL_LSBF                0x00001000    
#define SPI_CTL_FCEN                0x00002000    
#define SPI_CTL_FCCH                0x00004000    
#define SPI_CTL_FCPL                0x00008000    
#define SPI_CTL_FCWM                0x00030000    
#define SPI_CTL_FIFO0               0x00000000    
#define SPI_CTL_FIFO1               0x00010000    
#define SPI_CTL_FIFO2               0x00020000    
#define SPI_CTL_FMODE               0x00040000    
#define SPI_CTL_MIOM                0x00300000    
#define SPI_CTL_MIO_DIS             0x00000000    
#define SPI_CTL_MIO_DUAL            0x00100000    
#define SPI_CTL_MIO_QUAD            0x00200000    
#define SPI_CTL_SOSI                0x00400000    

#define SPI_RXCTL_REN               0x00000001    
#define SPI_RXCTL_RTI               0x00000004    
#define SPI_RXCTL_RWCEN             0x00000008    
#define SPI_RXCTL_RDR               0x00000070    
#define SPI_RXCTL_RDR_DIS           0x00000000    
#define SPI_RXCTL_RDR_NE            0x00000010    
#define SPI_RXCTL_RDR_25            0x00000020    
#define SPI_RXCTL_RDR_50            0x00000030    
#define SPI_RXCTL_RDR_75            0x00000040    
#define SPI_RXCTL_RDR_FULL          0x00000050    
#define SPI_RXCTL_RDO               0x00000100    
#define SPI_RXCTL_RRWM              0x00003000    
#define SPI_RXCTL_RWM_0             0x00000000    
#define SPI_RXCTL_RWM_25            0x00001000    
#define SPI_RXCTL_RWM_50            0x00002000    
#define SPI_RXCTL_RWM_75            0x00003000    
#define SPI_RXCTL_RUWM              0x00070000    
#define SPI_RXCTL_UWM_DIS           0x00000000    
#define SPI_RXCTL_UWM_25            0x00010000    
#define SPI_RXCTL_UWM_50            0x00020000    
#define SPI_RXCTL_UWM_75            0x00030000    
#define SPI_RXCTL_UWM_FULL          0x00040000    

#define SPI_TXCTL_TEN               0x00000001    
#define SPI_TXCTL_TTI               0x00000004    
#define SPI_TXCTL_TWCEN             0x00000008    
#define SPI_TXCTL_TDR               0x00000070    
#define SPI_TXCTL_TDR_DIS           0x00000000    
#define SPI_TXCTL_TDR_NF            0x00000010    
#define SPI_TXCTL_TDR_25            0x00000020    
#define SPI_TXCTL_TDR_50            0x00000030    
#define SPI_TXCTL_TDR_75            0x00000040    
#define SPI_TXCTL_TDR_EMPTY         0x00000050    
#define SPI_TXCTL_TDU               0x00000100    
#define SPI_TXCTL_TRWM              0x00003000    
#define SPI_TXCTL_RWM_FULL          0x00000000    
#define SPI_TXCTL_RWM_25            0x00001000    
#define SPI_TXCTL_RWM_50            0x00002000    
#define SPI_TXCTL_RWM_75            0x00003000    
#define SPI_TXCTL_TUWM              0x00070000    
#define SPI_TXCTL_UWM_DIS           0x00000000    
#define SPI_TXCTL_UWM_25            0x00010000    
#define SPI_TXCTL_UWM_50            0x00020000    
#define SPI_TXCTL_UWM_75            0x00030000    
#define SPI_TXCTL_UWM_EMPTY         0x00040000    

#define SPI_CLK_BAUD                0x0000FFFF    

#define SPI_DLY_STOP                0x000000FF    
#define SPI_DLY_LEADX               0x00000100    
#define SPI_DLY_LAGX                0x00000200    

#define SPI_SLVSEL_SSE1             0x00000002    
#define SPI_SLVSEL_SSE2             0x00000004    
#define SPI_SLVSEL_SSE3             0x00000008    
#define SPI_SLVSEL_SSE4             0x00000010    
#define SPI_SLVSEL_SSE5             0x00000020    
#define SPI_SLVSEL_SSE6             0x00000040    
#define SPI_SLVSEL_SSE7             0x00000080    
#define SPI_SLVSEL_SSEL1            0x00000200    
#define SPI_SLVSEL_SSEL2            0x00000400    
#define SPI_SLVSEL_SSEL3            0x00000800    
#define SPI_SLVSEL_SSEL4            0x00001000    
#define SPI_SLVSEL_SSEL5            0x00002000    
#define SPI_SLVSEL_SSEL6            0x00004000    
#define SPI_SLVSEL_SSEL7            0x00008000    

#define SPI_RWC_VALUE               0x0000FFFF    

#define SPI_RWCR_VALUE              0x0000FFFF    

#define SPI_TWC_VALUE               0x0000FFFF    

#define SPI_TWCR_VALUE              0x0000FFFF    

#define SPI_IMSK_RUWM               0x00000002    
#define SPI_IMSK_TUWM               0x00000004    
#define SPI_IMSK_ROM                0x00000010    
#define SPI_IMSK_TUM                0x00000020    
#define SPI_IMSK_TCM                0x00000040    
#define SPI_IMSK_MFM                0x00000080    
#define SPI_IMSK_RSM                0x00000100    
#define SPI_IMSK_TSM                0x00000200    
#define SPI_IMSK_RFM                0x00000400    
#define SPI_IMSK_TFM                0x00000800    

#define SPI_IMSK_CLR_RUW            0x00000002    
#define SPI_IMSK_CLR_TUWM           0x00000004    
#define SPI_IMSK_CLR_ROM            0x00000010    
#define SPI_IMSK_CLR_TUM            0x00000020    
#define SPI_IMSK_CLR_TCM            0x00000040    
#define SPI_IMSK_CLR_MFM            0x00000080    
#define SPI_IMSK_CLR_RSM            0x00000100    
#define SPI_IMSK_CLR_TSM            0x00000200    
#define SPI_IMSK_CLR_RFM            0x00000400    
#define SPI_IMSK_CLR_TFM            0x00000800    

#define SPI_IMSK_SET_RUWM           0x00000002    
#define SPI_IMSK_SET_TUWM           0x00000004    
#define SPI_IMSK_SET_ROM            0x00000010    
#define SPI_IMSK_SET_TUM            0x00000020    
#define SPI_IMSK_SET_TCM            0x00000040    
#define SPI_IMSK_SET_MFM            0x00000080    
#define SPI_IMSK_SET_RSM            0x00000100    
#define SPI_IMSK_SET_TSM            0x00000200    
#define SPI_IMSK_SET_RFM            0x00000400    
#define SPI_IMSK_SET_TFM            0x00000800    

#define SPI_STAT_SPIF               0x00000001    
#define SPI_STAT_RUWM               0x00000002    
#define SPI_STAT_TUWM               0x00000004    
#define SPI_STAT_ROE                0x00000010    
#define SPI_STAT_TUE                0x00000020    
#define SPI_STAT_TCE                0x00000040    
#define SPI_STAT_MODF               0x00000080    
#define SPI_STAT_RS                 0x00000100    
#define SPI_STAT_TS                 0x00000200    
#define SPI_STAT_RF                 0x00000400    
#define SPI_STAT_TF                 0x00000800    
#define SPI_STAT_RFS                0x00007000    
#define SPI_STAT_RFIFO_EMPTY        0x00000000    
#define SPI_STAT_RFIFO_25           0x00001000    
#define SPI_STAT_RFIFO_50           0x00002000    
#define SPI_STAT_RFIFO_75           0x00003000    
#define SPI_STAT_RFIFO_FULL         0x00004000    
#define SPI_STAT_TFS                0x00070000    
#define SPI_STAT_TFIFO_FULL         0x00000000    
#define SPI_STAT_TFIFO_25           0x00010000    
#define SPI_STAT_TFIFO_50           0x00020000    
#define SPI_STAT_TFIFO_75           0x00030000    
#define SPI_STAT_TFIFO_EMPTY        0x00040000    
#define SPI_STAT_FCS                0x00100000    
#define SPI_STAT_RFE                0x00400000    
#define SPI_STAT_TFF                0x00800000    

#define SPI_ILAT_RUWMI              0x00000002    
#define SPI_ILAT_TUWMI              0x00000004    
#define SPI_ILAT_ROI                0x00000010    
#define SPI_ILAT_TUI                0x00000020    
#define SPI_ILAT_TCI                0x00000040    
#define SPI_ILAT_MFI                0x00000080    
#define SPI_ILAT_RSI                0x00000100    
#define SPI_ILAT_TSI                0x00000200    
#define SPI_ILAT_RFI                0x00000400    
#define SPI_ILAT_TFI                0x00000800    

#define SPI_ILAT_CLR_RUWMI          0x00000002    
#define SPI_ILAT_CLR_TUWMI          0x00000004    
#define SPI_ILAT_CLR_ROI            0x00000010    
#define SPI_ILAT_CLR_TUI            0x00000020    
#define SPI_ILAT_CLR_TCI            0x00000040    
#define SPI_ILAT_CLR_MFI            0x00000080    
#define SPI_ILAT_CLR_RSI            0x00000100    
#define SPI_ILAT_CLR_TSI            0x00000200    
#define SPI_ILAT_CLR_RFI            0x00000400    
#define SPI_ILAT_CLR_TFI            0x00000800    


struct adi_spi_regs {
	u32 revid;
	u32 control;
	u32 rx_control;
	u32 tx_control;
	u32 clock;
	u32 delay;
	u32 ssel;
	u32 rwc;
	u32 rwcr;
	u32 twc;
	u32 twcr;
	u32 reserved0;
	u32 emask;
	u32 emaskcl;
	u32 emaskst;
	u32 reserved1;
	u32 status;
	u32 elat;
	u32 elatcl;
	u32 reserved2;
	u32 rfifo;
	u32 reserved3;
	u32 tfifo;
};

#define MAX_CTRL_CS          8  


struct adi_spi3_master {
	u16 num_chipselect;
	u16 pin_req[7];
};


struct adi_spi3_chip {
	u32 control;
	u16 cs_chg_udelay; 
	u32 tx_dummy_val; 
	bool enable_dma;
};

#endif 
