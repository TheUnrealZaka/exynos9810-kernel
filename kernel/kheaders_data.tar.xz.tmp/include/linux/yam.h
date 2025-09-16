





#define SIOCYAMRESERVED	(0)
#define SIOCYAMSCFG 	(1)	
#define SIOCYAMGCFG 	(2)	
#define SIOCYAMSMCS 	(3)	

#define YAM_IOBASE   (1 << 0)
#define YAM_IRQ      (1 << 1)
#define YAM_BITRATE  (1 << 2) 
#define YAM_MODE     (1 << 3) 
#define YAM_HOLDDLY  (1 << 4) 
#define YAM_TXDELAY  (1 << 5) 
#define YAM_TXTAIL   (1 << 6) 
#define YAM_PERSIST  (1 << 7) 
#define YAM_SLOTTIME (1 << 8) 
#define YAM_BAUDRATE (1 << 9) 

#define YAM_MAXBITRATE  57600
#define YAM_MAXBAUDRATE 115200
#define YAM_MAXMODE     2
#define YAM_MAXHOLDDLY  99
#define YAM_MAXTXDELAY  999
#define YAM_MAXTXTAIL   999
#define YAM_MAXPERSIST  255
#define YAM_MAXSLOTTIME 999

#define YAM_FPGA_SIZE	5302

struct yamcfg {
	unsigned int mask;		
	unsigned int iobase;	
	unsigned int irq;		
	unsigned int bitrate;	
	unsigned int baudrate;	
	unsigned int txdelay;	
	unsigned int txtail;	
	unsigned int persist;	
	unsigned int slottime;	
	unsigned int mode;		
	unsigned int holddly;	
};

struct yamdrv_ioctl_cfg {
	int cmd;
	struct yamcfg cfg;
};

struct yamdrv_ioctl_mcs {
	int cmd;
	unsigned int bitrate;
	unsigned char bits[YAM_FPGA_SIZE];
};
