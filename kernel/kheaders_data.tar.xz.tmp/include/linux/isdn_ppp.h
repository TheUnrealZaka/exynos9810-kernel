
#ifndef _LINUX_ISDN_PPP_H
#define _LINUX_ISDN_PPP_H




#ifdef CONFIG_IPPP_FILTER
#include <linux/filter.h>
#endif
#include <uapi/linux/isdn_ppp.h>

#define DECOMP_ERR_NOMEM	(-10)

#define MP_END_FRAG    0x40
#define MP_BEGIN_FRAG  0x80

#define MP_MAX_QUEUE_LEN	16



#define IPPP_RESET_MAXDATABYTES	32

struct isdn_ppp_resetparams {
  unsigned char valid:1;	
  unsigned char rsend:1;	
  unsigned char idval:1;	
  unsigned char dtval:1;	
  unsigned char expra:1;	
  unsigned char id;		
  unsigned short maxdlen;	
  unsigned short dlen;		
  unsigned char *data;		
};


struct isdn_ppp_compressor {
  struct isdn_ppp_compressor *next, *prev;
  struct module *owner;
  int num; 
  
  void *(*alloc) (struct isdn_ppp_comp_data *);
  void (*free) (void *state);
  int  (*init) (void *state, struct isdn_ppp_comp_data *,
		int unit,int debug);
  
  
  
  void (*reset) (void *state, unsigned char code, unsigned char id,
		 unsigned char *data, unsigned len,
		 struct isdn_ppp_resetparams *rsparm);
  
  int  (*compress) (void *state, struct sk_buff *in,
		    struct sk_buff *skb_out, int proto);
  
	int  (*decompress) (void *state,struct sk_buff *in,
			    struct sk_buff *skb_out,
			    struct isdn_ppp_resetparams *rsparm);
  
  void (*incomp) (void *state, struct sk_buff *in,int proto);
  void (*stat) (void *state, struct compstat *stats);
};

extern int isdn_ppp_register_compressor(struct isdn_ppp_compressor *);
extern int isdn_ppp_unregister_compressor(struct isdn_ppp_compressor *);
extern int isdn_ppp_dial_slave(char *);
extern int isdn_ppp_hangup_slave(char *);

typedef struct {
  unsigned long seqerrs;
  unsigned long frame_drops;
  unsigned long overflows;
  unsigned long max_queue_len;
} isdn_mppp_stats;

typedef struct {
  int mp_mrru;                        
  struct sk_buff * frags;	
  long frames;			
  unsigned int seq;		
  spinlock_t lock;
  int ref_ct;				 
  
  isdn_mppp_stats stats;
} ippp_bundle;

#define NUM_RCV_BUFFS     64

struct ippp_buf_queue {
  struct ippp_buf_queue *next;
  struct ippp_buf_queue *last;
  char *buf;                 
  int len;
};


enum ippp_ccp_reset_states {
  CCPResetIdle,
  CCPResetSentReq,
  CCPResetRcvdReq,
  CCPResetSentAck,
  CCPResetRcvdAck
};

struct ippp_ccp_reset_state {
  enum ippp_ccp_reset_states state;	
  struct ippp_struct *is;		
  unsigned char id;			
  unsigned char ta:1;			
  unsigned char expra:1;		
  int dlen;				
  struct timer_list timer;		
  
  unsigned char data[IPPP_RESET_MAXDATABYTES];
};


struct ippp_ccp_reset {
  struct ippp_ccp_reset_state *rs[256];	
  unsigned char lastid;			
};

struct ippp_struct {
  struct ippp_struct *next_link;
  int state;
  spinlock_t buflock;
  struct ippp_buf_queue rq[NUM_RCV_BUFFS]; 
  struct ippp_buf_queue *first;  
  struct ippp_buf_queue *last;   
  wait_queue_head_t wq;
  struct task_struct *tk;
  unsigned int mpppcfg;
  unsigned int pppcfg;
  unsigned int mru;
  unsigned int mpmru;
  unsigned int mpmtu;
  unsigned int maxcid;
  struct isdn_net_local_s *lp;
  int unit;
  int minor;
  unsigned int last_link_seqno;
  long mp_seqno;
#ifdef CONFIG_ISDN_PPP_VJ
  unsigned char *cbuf;
  struct slcompress *slcomp;
#endif
#ifdef CONFIG_IPPP_FILTER
  struct bpf_prog *pass_filter;   
  struct bpf_prog *active_filter; 
#endif
  unsigned long debug;
  struct isdn_ppp_compressor *compressor,*decompressor;
  struct isdn_ppp_compressor *link_compressor,*link_decompressor;
  void *decomp_stat,*comp_stat,*link_decomp_stat,*link_comp_stat;
  struct ippp_ccp_reset *reset;	
  unsigned long compflags;
};

#endif 
