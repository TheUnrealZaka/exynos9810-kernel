

#ifndef TI_WILINK_ST_H
#define TI_WILINK_ST_H

#include <linux/skbuff.h>


enum proto_type {
	ST_BT,
	ST_FM,
	ST_GPS,
	ST_MAX_CHANNELS = 16,
};


struct st_proto_s {
	enum proto_type type;
	long (*recv) (void *, struct sk_buff *);
	unsigned char (*match_packet) (const unsigned char *data);
	void (*reg_complete_cb) (void *, int data);
	long (*write) (struct sk_buff *skb);
	void *priv_data;

	unsigned char chnl_id;
	unsigned short max_frame_size;
	unsigned char hdr_len;
	unsigned char offset_len_in_hdr;
	unsigned char len_size;
	unsigned char reserve;
};

extern long st_register(struct st_proto_s *);
extern long st_unregister(struct st_proto_s *);





#define ST_NOTEMPTY	1
#define ST_EMPTY	0


#define ST_INITIALIZING		1
#define ST_REG_IN_PROGRESS	2
#define ST_REG_PENDING		3
#define ST_WAITING_FOR_RESP	4


struct st_data_s {
	unsigned long st_state;
	struct sk_buff *tx_skb;
#define ST_TX_SENDING	1
#define ST_TX_WAKEUP	2
	unsigned long tx_state;
	struct st_proto_s *list[ST_MAX_CHANNELS];
	bool is_registered[ST_MAX_CHANNELS];
	unsigned long rx_state;
	unsigned long rx_count;
	struct sk_buff *rx_skb;
	unsigned char rx_chnl;
	struct sk_buff_head txq, tx_waitq;
	spinlock_t lock;
	unsigned char	protos_registered;
	unsigned long ll_state;
	void *kim_data;
	struct tty_struct *tty;
	struct work_struct work_write_wakeup;
};


int st_get_uart_wr_room(struct st_data_s *st_gdata);

int st_int_write(struct st_data_s*, const unsigned char*, int);


long st_write(struct sk_buff *);


void st_ll_send_frame(enum proto_type, struct sk_buff *);


void st_tx_wakeup(struct st_data_s *st_data);


int st_core_init(struct st_data_s **);
void st_core_exit(struct st_data_s *);


void st_kim_ref(struct st_data_s **, int);

#define GPS_STUB_TEST
#ifdef GPS_STUB_TEST
int gps_chrdrv_stub_write(const unsigned char*, int);
void gps_chrdrv_stub_init(void);
#endif




#define LDISC_TIME	1000
#define CMD_RESP_TIME	800
#define CMD_WR_TIME	5000
#define MAKEWORD(a, b)  ((unsigned short)(((unsigned char)(a)) \
	| ((unsigned short)((unsigned char)(b))) << 8))

#define GPIO_HIGH 1
#define GPIO_LOW  0


#define POR_RETRY_COUNT 5


struct chip_version {
	unsigned short full;
	unsigned short chip;
	unsigned short min_ver;
	unsigned short maj_ver;
};

#define UART_DEV_NAME_LEN 32

struct kim_data_s {
	long uim_pid;
	struct platform_device *kim_pdev;
	struct completion kim_rcvd, ldisc_installed;
	char resp_buffer[30];
	const struct firmware *fw_entry;
	unsigned nshutdown;
	unsigned long rx_state;
	unsigned long rx_count;
	struct sk_buff *rx_skb;
	struct st_data_s *core_data;
	struct chip_version version;
	unsigned char ldisc_install;
	unsigned char dev_name[UART_DEV_NAME_LEN + 1];
	unsigned flow_cntrl;
	unsigned baud_rate;
};


long st_kim_start(void *);
long st_kim_stop(void *);

void st_kim_complete(void *);
void kim_st_list_protocols(struct st_data_s *, void *);
void st_kim_recv(void *, const unsigned char *, long);



#define ACTION_SEND_COMMAND     1
#define ACTION_WAIT_EVENT       2
#define ACTION_SERIAL           3
#define ACTION_DELAY            4
#define ACTION_RUN_SCRIPT       5
#define ACTION_REMARKS          6


struct bts_header {
	u32 magic;
	u32 version;
	u8 future[24];
	u8 actions[0];
} __attribute__ ((packed));


struct bts_action {
	u16 type;
	u16 size;
	u8 data[0];
} __attribute__ ((packed));

struct bts_action_send {
	u8 data[0];
} __attribute__ ((packed));

struct bts_action_wait {
	u32 msec;
	u32 size;
	u8 data[0];
} __attribute__ ((packed));

struct bts_action_delay {
	u32 msec;
} __attribute__ ((packed));

struct bts_action_serial {
	u32 baud;
	u32 flow_control;
} __attribute__ ((packed));


struct hci_command {
	u8 prefix;
	u16 opcode;
	u8 plen;
	u32 speed;
} __attribute__ ((packed));




#define ST_W4_PACKET_TYPE       0
#define ST_W4_HEADER		1
#define ST_W4_DATA		2


#define ST_LL_ASLEEP               0
#define ST_LL_ASLEEP_TO_AWAKE      1
#define ST_LL_AWAKE                2
#define ST_LL_AWAKE_TO_ASLEEP      3
#define ST_LL_INVALID		   4


#define LL_SLEEP_IND	0x30
#define LL_SLEEP_ACK	0x31
#define LL_WAKE_UP_IND	0x32
#define LL_WAKE_UP_ACK	0x33


long st_ll_init(struct st_data_s *);
long st_ll_deinit(struct st_data_s *);


void st_ll_enable(struct st_data_s *);
void st_ll_disable(struct st_data_s *);


unsigned long st_ll_getstate(struct st_data_s *);
unsigned long st_ll_sleep_state(struct st_data_s *, unsigned char);
void st_ll_wakeup(struct st_data_s *);



struct fm_event_hdr {
	u8 plen;
} __attribute__ ((packed));

#define FM_MAX_FRAME_SIZE 0xFF	
#define FM_EVENT_HDR_SIZE 1	
#define ST_FM_CH8_PKT 0x8


struct gps_event_hdr {
	u8 opcode;
	u16 plen;
} __attribute__ ((packed));


struct ti_st_plat_data {
	u32 nshutdown_gpio;
	unsigned char dev_name[UART_DEV_NAME_LEN]; 
	u32 flow_cntrl; 
	u32 baud_rate;
	int (*suspend)(struct platform_device *, pm_message_t);
	int (*resume)(struct platform_device *);
	int (*chip_enable) (struct kim_data_s *);
	int (*chip_disable) (struct kim_data_s *);
	int (*chip_asleep) (struct kim_data_s *);
	int (*chip_awake) (struct kim_data_s *);
};

#endif 
