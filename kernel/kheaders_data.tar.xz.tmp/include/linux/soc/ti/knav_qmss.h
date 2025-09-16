

#ifndef __SOC_TI_KNAV_QMSS_H__
#define __SOC_TI_KNAV_QMSS_H__

#include <linux/err.h>
#include <linux/time.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/fcntl.h>
#include <linux/dma-mapping.h>


#define KNAV_QUEUE_QPEND	((unsigned)-2) 
#define KNAV_QUEUE_ACC		((unsigned)-3) 
#define KNAV_QUEUE_GP		((unsigned)-4) 


#define KNAV_QUEUE_SHARED	0x0001		


enum knav_queue_ctrl_cmd {
	KNAV_QUEUE_GET_ID,
	KNAV_QUEUE_FLUSH,
	KNAV_QUEUE_SET_NOTIFIER,
	KNAV_QUEUE_ENABLE_NOTIFY,
	KNAV_QUEUE_DISABLE_NOTIFY,
	KNAV_QUEUE_GET_COUNT
};


typedef void (*knav_queue_notify_fn)(void *arg);


struct knav_queue_notify_config {
	knav_queue_notify_fn fn;
	void *fn_arg;
};

void *knav_queue_open(const char *name, unsigned id,
					unsigned flags);
void knav_queue_close(void *qhandle);
int knav_queue_device_control(void *qhandle,
				enum knav_queue_ctrl_cmd cmd,
				unsigned long arg);
dma_addr_t knav_queue_pop(void *qhandle, unsigned *size);
int knav_queue_push(void *qhandle, dma_addr_t dma,
				unsigned size, unsigned flags);

void *knav_pool_create(const char *name,
				int num_desc, int region_id);
void knav_pool_destroy(void *ph);
int knav_pool_count(void *ph);
void *knav_pool_desc_get(void *ph);
void knav_pool_desc_put(void *ph, void *desc);
int knav_pool_desc_map(void *ph, void *desc, unsigned size,
					dma_addr_t *dma, unsigned *dma_sz);
void *knav_pool_desc_unmap(void *ph, dma_addr_t dma, unsigned dma_sz);
dma_addr_t knav_pool_desc_virt_to_dma(void *ph, void *virt);
void *knav_pool_desc_dma_to_virt(void *ph, dma_addr_t dma);

#endif 
