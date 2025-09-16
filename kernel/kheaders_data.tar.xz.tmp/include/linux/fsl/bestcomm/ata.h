

#ifndef __BESTCOMM_ATA_H__
#define __BESTCOMM_ATA_H__


struct bcom_ata_bd {
	u32	status;
	u32	src_pa;
	u32	dst_pa;
};

extern struct bcom_task * bcom_ata_init(int queue_len, int maxbufsize);
extern void bcom_ata_rx_prepare(struct bcom_task *tsk);
extern void bcom_ata_tx_prepare(struct bcom_task *tsk);
extern void bcom_ata_reset_bd(struct bcom_task *tsk);
extern void bcom_ata_release(struct bcom_task *tsk);

#endif 

