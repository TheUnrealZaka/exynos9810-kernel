

#ifndef __BESTCOMM_GEN_BD_H__
#define __BESTCOMM_GEN_BD_H__

struct bcom_gen_bd {
	u32	status;
	u32	buf_pa;
};


extern struct bcom_task *
bcom_gen_bd_rx_init(int queue_len, phys_addr_t fifo,
			int initiator, int ipr, int maxbufsize);

extern int
bcom_gen_bd_rx_reset(struct bcom_task *tsk);

extern void
bcom_gen_bd_rx_release(struct bcom_task *tsk);


extern struct bcom_task *
bcom_gen_bd_tx_init(int queue_len, phys_addr_t fifo,
			int initiator, int ipr);

extern int
bcom_gen_bd_tx_reset(struct bcom_task *tsk);

extern void
bcom_gen_bd_tx_release(struct bcom_task *tsk);



struct bcom_task * bcom_psc_gen_bd_rx_init(unsigned psc_num, int queue_len,
					   phys_addr_t fifo, int maxbufsize);
struct bcom_task * bcom_psc_gen_bd_tx_init(unsigned psc_num, int queue_len,
					   phys_addr_t fifo);
#endif  

