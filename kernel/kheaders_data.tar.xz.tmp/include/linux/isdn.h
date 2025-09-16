
#ifndef __ISDN_H__
#define __ISDN_H__


#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_reg.h>
#include <linux/fcntl.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/mutex.h>
#include <uapi/linux/isdn.h>

#define ISDN_TTY_MAJOR    43
#define ISDN_TTYAUX_MAJOR 44
#define ISDN_MAJOR        45



#define ISDN_MINOR_B        0
#define ISDN_MINOR_BMAX     (ISDN_MAX_CHANNELS-1)
#define ISDN_MINOR_CTRL     64
#define ISDN_MINOR_CTRLMAX  (64 + (ISDN_MAX_CHANNELS-1))
#define ISDN_MINOR_PPP      128
#define ISDN_MINOR_PPPMAX   (128 + (ISDN_MAX_CHANNELS-1))
#define ISDN_MINOR_STATUS   255

#ifdef CONFIG_ISDN_PPP

#ifdef CONFIG_ISDN_PPP_VJ
#  include <net/slhc_vj.h>
#endif

#include <linux/ppp_defs.h>
#include <linux/ppp-ioctl.h>

#include <linux/isdn_ppp.h>
#endif

#ifdef CONFIG_ISDN_X25
#  include <linux/concap.h>
#endif

#include <linux/isdnif.h>

#define ISDN_DRVIOCTL_MASK       0x7f  


#define ISDN_SERVICE_VOICE 1
#define ISDN_SERVICE_AB    1<<1 
#define ISDN_SERVICE_X21   1<<2
#define ISDN_SERVICE_G4    1<<3
#define ISDN_SERVICE_BTX   1<<4
#define ISDN_SERVICE_DFUE  1<<5
#define ISDN_SERVICE_X25   1<<6
#define ISDN_SERVICE_TTX   1<<7
#define ISDN_SERVICE_MIXED 1<<8
#define ISDN_SERVICE_FW    1<<9
#define ISDN_SERVICE_GTEL  1<<10
#define ISDN_SERVICE_BTXN  1<<11
#define ISDN_SERVICE_BTEL  1<<12


#define USG_NONE(x)         ((x & ISDN_USAGE_MASK)==ISDN_USAGE_NONE)
#define USG_RAW(x)          ((x & ISDN_USAGE_MASK)==ISDN_USAGE_RAW)
#define USG_MODEM(x)        ((x & ISDN_USAGE_MASK)==ISDN_USAGE_MODEM)
#define USG_VOICE(x)        ((x & ISDN_USAGE_MASK)==ISDN_USAGE_VOICE)
#define USG_NET(x)          ((x & ISDN_USAGE_MASK)==ISDN_USAGE_NET)
#define USG_FAX(x)          ((x & ISDN_USAGE_MASK)==ISDN_USAGE_FAX)
#define USG_OUTGOING(x)     ((x & ISDN_USAGE_OUTGOING)==ISDN_USAGE_OUTGOING)
#define USG_MODEMORVOICE(x) (((x & ISDN_USAGE_MASK)==ISDN_USAGE_MODEM) || \
                             ((x & ISDN_USAGE_MASK)==ISDN_USAGE_VOICE)     )


#define ISDN_TIMER_RES         4                         
#define ISDN_TIMER_02SEC       (HZ/ISDN_TIMER_RES/5)     
#define ISDN_TIMER_1SEC        (HZ/ISDN_TIMER_RES)       
#define ISDN_TIMER_RINGING     5 
#define ISDN_TIMER_KEEPINT    10 
#define ISDN_TIMER_MODEMREAD   1
#define ISDN_TIMER_MODEMPLUS   2
#define ISDN_TIMER_MODEMRING   4
#define ISDN_TIMER_MODEMXMIT   8
#define ISDN_TIMER_NETDIAL    16 
#define ISDN_TIMER_NETHANGUP  32
#define ISDN_TIMER_CARRIER   256 
#define ISDN_TIMER_FAST      (ISDN_TIMER_MODEMREAD | ISDN_TIMER_MODEMPLUS | \
                              ISDN_TIMER_MODEMXMIT)
#define ISDN_TIMER_SLOW      (ISDN_TIMER_MODEMRING | ISDN_TIMER_NETHANGUP | \
                              ISDN_TIMER_NETDIAL | ISDN_TIMER_CARRIER)


#define ISDN_TIMER_DTIMEOUT10 (10*HZ/(ISDN_TIMER_02SEC*(ISDN_TIMER_RES+1)))
#define ISDN_TIMER_DTIMEOUT15 (15*HZ/(ISDN_TIMER_02SEC*(ISDN_TIMER_RES+1)))
#define ISDN_TIMER_DTIMEOUT60 (60*HZ/(ISDN_TIMER_02SEC*(ISDN_TIMER_RES+1)))


#define ISDN_GLOBAL_STOPPED 1




#define ISDN_NET_CONNECTED  0x01       
#define ISDN_NET_SECURE     0x02       
#define ISDN_NET_CALLBACK   0x04       
#define ISDN_NET_CBHUP      0x08       
#define ISDN_NET_CBOUT      0x10       

#define ISDN_NET_MAGIC      0x49344C02 


typedef struct {
  void *next;
  char num[ISDN_MSNLEN];
} isdn_net_phone;




typedef struct isdn_net_local_s {
  ulong                  magic;
  struct net_device_stats stats;       
  int                    isdn_device;  
  int                    isdn_channel; 
  int			 ppp_slot;     
  int                    pre_device;   
  int                    pre_channel;  
  int                    exclusive;    
  int                    flags;        
  int                    dialretry;    
  int                    dialmax;      
  int                    cbdelay;      
  int                    dtimer;       
  char                   msn[ISDN_MSNLEN]; 
  u_char                 cbhup;        
  u_char                 dialstate;    
  u_char                 p_encap;      
                                       
				       
                                       
  u_char                 l2_proto;     
				       
                                       
                                       
				       
				       
				       
  u_char                 l3_proto;     
				       
                                       
                                       
  int                    huptimer;     
  int                    charge;       
  ulong                  chargetime;   
  int                    hupflags;     
				       
				       
                                       
                                       
  int                    outgoing;     
  int                    onhtime;      
  int                    chargeint;    
  int                    onum;         
  int                    cps;          
  int                    transcount;   
  int                    sqfull;       
  ulong                  sqfull_stamp; 
  ulong                  slavedelay;   
  int                    triggercps;   
  isdn_net_phone         *phone[2];    
				       
				       
  isdn_net_phone         *dial;        
  struct net_device      *master;      
  struct net_device      *slave;       
  struct isdn_net_local_s *next;       
  struct isdn_net_local_s *last;       
  struct isdn_net_dev_s  *netdev;      
  struct sk_buff_head    super_tx_queue; 
	                               
  atomic_t frame_cnt;                  
                        	           
                                       
  spinlock_t             xmit_lock;    
                                       
                                       

  int  pppbind;                        
  int					dialtimeout;	
  int					dialwait;		
  ulong					dialstarted;	
  ulong					dialwait_timer;	
  int					huptimeout;		
#ifdef CONFIG_ISDN_X25
  struct concap_device_ops *dops;      
#endif
  
  ulong cisco_myseq;                   
  ulong cisco_mineseen;                
  ulong cisco_yourseq;                 
  int cisco_keepalive_period;		
  ulong cisco_last_slarp_in;		
  char cisco_line_state;		
  char cisco_debserint;			
  struct timer_list cisco_timer;
  struct work_struct tqueue;
} isdn_net_local;


typedef struct isdn_net_dev_s {
  isdn_net_local *local;
  isdn_net_local *queue;               
  spinlock_t queue_lock;               
  void *next;                          
  struct net_device *dev;              
#ifdef CONFIG_ISDN_PPP
  ippp_bundle * pb;		
#endif
#ifdef CONFIG_ISDN_X25
  struct concap_proto  *cprot; 
#endif

} isdn_net_dev;





#define ISDN_ASYNC_MAGIC          0x49344C01 
#define ISDN_SERIAL_XMIT_SIZE           1024 
#define ISDN_SERIAL_XMIT_MAX            4000 

#ifdef CONFIG_ISDN_AUDIO

typedef struct _isdn_audio_data {
  unsigned short dle_count;
  unsigned char  lock;
} isdn_audio_data_t;

#define ISDN_AUDIO_SKB_DLECOUNT(skb)	(((isdn_audio_data_t *)&skb->cb[0])->dle_count)
#define ISDN_AUDIO_SKB_LOCK(skb)	(((isdn_audio_data_t *)&skb->cb[0])->lock)
#endif


typedef struct atemu {
	u_char       profile[ISDN_MODEM_NUMREG]; 
	u_char       mdmreg[ISDN_MODEM_NUMREG];  
	char         pmsn[ISDN_MSNLEN];          
	char         msn[ISDN_MSNLEN];           
	char         plmsn[ISDN_LMSNLEN];        
	char         lmsn[ISDN_LMSNLEN];         
	char         cpn[ISDN_MSNLEN];           
	char         connmsg[ISDN_CMSGLEN];	 
#ifdef CONFIG_ISDN_AUDIO
	u_char       vpar[10];                   
	int          lastDLE;                    
#endif
	int          mdmcmdl;                    
	int          pluscount;                  
	u_long       lastplus;                   
	int	     carrierwait;                
	char         mdmcmd[255];                
	unsigned int charge;                     
} atemu;


typedef struct modem_info {
  int			magic;
  struct tty_port	port;
  int			x_char;		 
  int			mcr;		 
  int                   msr;             
  int                   lsr;             
  int			line;
  int                   online;          
					 
  int                   dialing;         
  int                   closing;
  int                   rcvsched;        
  int                   isdn_driver;	 
  int                   isdn_channel;    
  int                   drv_index;       
  int                   ncarrier;        
  unsigned char         last_cause[8];   
  unsigned char         last_num[ISDN_MSNLEN];
	                                 
  unsigned char         last_l2;         
  unsigned char         last_si;         
  unsigned char         last_lhup;       
  unsigned char         last_dir;        
  struct timer_list     nc_timer;        
  int                   send_outstanding;
  int                   xmit_size;       
  int                   xmit_count;      
  struct sk_buff_head   xmit_queue;      
  atomic_t              xmit_lock;       
#ifdef CONFIG_ISDN_AUDIO
  int                   vonline;         
					 
					 
					 
  struct sk_buff_head   dtmf_queue;      
  void                  *adpcms;         
  void                  *adpcmr;         
  void                  *dtmf_state;     
  void                  *silence_state;  
#endif
#ifdef CONFIG_ISDN_TTY_FAX
  struct T30_s		*fax;		 
  int			faxonline;	 
#endif
  atemu                 emu;             
  spinlock_t	        readlock;
} modem_info;

#define ISDN_MODEM_WINSIZE 8


typedef struct _isdn_modem {
  int                refcount;				
  struct tty_driver  *tty_modem;			
  struct tty_struct  *modem_table[ISDN_MAX_CHANNELS];	
  struct ktermios     *modem_termios[ISDN_MAX_CHANNELS];
  struct ktermios     *modem_termios_locked[ISDN_MAX_CHANNELS];
  modem_info         info[ISDN_MAX_CHANNELS];	   
} isdn_modem_t;




#define V110_BUFSIZE 1024

typedef struct {
	int nbytes;                    
	int nbits;                     
	unsigned char key;             
	int decodelen;                 
	int SyncInit;                  
	unsigned char *OnlineFrame;    
	unsigned char *OfflineFrame;   
	int framelen;                  
	int skbuser;                   
	int skbidle;                   
	int introducer;                
	int dbit;
	unsigned char b;
	int skbres;                    
	int maxsize;                   
	unsigned char *encodebuf;      
	unsigned char decodebuf[V110_BUFSIZE]; 
} isdn_v110_stream;





typedef struct {
	char *next;
	char *private;
} infostruct;

#define DRV_FLAG_RUNNING 1
#define DRV_FLAG_REJBUS  2
#define DRV_FLAG_LOADED  4


typedef struct _isdn_driver {
	ulong               online;           
	ulong               flags;            
	int                 locks;            
	int                 channels;         
	wait_queue_head_t   st_waitq;         
	int                 maxbufsize;       
	unsigned long       pktcount;         
	int                 stavail;          
	isdn_if            *interface;        
	int                *rcverr;           
	int                *rcvcount;         
#ifdef CONFIG_ISDN_AUDIO
	unsigned long      DLEflag;           
#endif
	struct sk_buff_head *rpqueue;         
	wait_queue_head_t  *rcv_waitq;       
	wait_queue_head_t  *snd_waitq;       
	char               msn2eaz[10][ISDN_MSNLEN];  
} isdn_driver_t;


typedef struct isdn_devt {
	struct module     *owner;
	spinlock_t	  lock;
	unsigned short    flags;		      
	int               drivers;		      
	int               channels;		      
	int               net_verbose;                
	int               modempoll;		      
	spinlock_t	  timerlock;
	int               tflags;                     
	
	int               global_flags;
	infostruct        *infochain;                 
	wait_queue_head_t info_waitq;                 
	struct timer_list timer;		      
	int               chanmap[ISDN_MAX_CHANNELS]; 
	int               drvmap[ISDN_MAX_CHANNELS];  
	int               usage[ISDN_MAX_CHANNELS];   
	char              num[ISDN_MAX_CHANNELS][ISDN_MSNLEN];
	
	int               m_idx[ISDN_MAX_CHANNELS];   
	isdn_driver_t     *drv[ISDN_MAX_DRIVERS];     
	isdn_net_dev      *netdev;		      
	char              drvid[ISDN_MAX_DRIVERS][20];
	struct task_struct *profd;                    
	isdn_modem_t      mdm;			      
	isdn_net_dev      *rx_netdev[ISDN_MAX_CHANNELS]; 
	isdn_net_dev      *st_netdev[ISDN_MAX_CHANNELS]; 
	ulong             ibytes[ISDN_MAX_CHANNELS];  
	ulong             obytes[ISDN_MAX_CHANNELS];  
	int               v110emu[ISDN_MAX_CHANNELS]; 
	atomic_t          v110use[ISDN_MAX_CHANNELS]; 
	isdn_v110_stream  *v110[ISDN_MAX_CHANNELS];   
	struct mutex      mtx;                        
	unsigned long     global_features;
} isdn_dev;

extern isdn_dev *dev;


#endif 
