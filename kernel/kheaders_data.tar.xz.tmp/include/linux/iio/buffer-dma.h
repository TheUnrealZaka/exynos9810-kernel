

#ifndef __INDUSTRIALIO_DMA_BUFFER_H__
#define __INDUSTRIALIO_DMA_BUFFER_H__

#include <linux/list.h>
#include <linux/kref.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/iio/buffer.h>

struct iio_dma_buffer_queue;
struct iio_dma_buffer_ops;
struct device;

struct iio_buffer_block {
	u32 size;
	u32 bytes_used;
};


enum iio_block_state {
	IIO_BLOCK_STATE_DEQUEUED,
	IIO_BLOCK_STATE_QUEUED,
	IIO_BLOCK_STATE_ACTIVE,
	IIO_BLOCK_STATE_DONE,
	IIO_BLOCK_STATE_DEAD,
};


struct iio_dma_buffer_block {
	
	struct list_head head;
	size_t bytes_used;

	
	void *vaddr;
	dma_addr_t phys_addr;
	size_t size;
	struct iio_dma_buffer_queue *queue;

	
	struct kref kref;
	
	enum iio_block_state state;
};


struct iio_dma_buffer_queue_fileio {
	struct iio_dma_buffer_block *blocks[2];
	struct iio_dma_buffer_block *active_block;
	size_t pos;
	size_t block_size;
};


struct iio_dma_buffer_queue {
	struct iio_buffer buffer;
	struct device *dev;
	const struct iio_dma_buffer_ops *ops;

	struct mutex lock;
	spinlock_t list_lock;
	struct list_head incoming;
	struct list_head outgoing;

	bool active;

	struct iio_dma_buffer_queue_fileio fileio;
};


struct iio_dma_buffer_ops {
	int (*submit)(struct iio_dma_buffer_queue *queue,
		struct iio_dma_buffer_block *block);
	void (*abort)(struct iio_dma_buffer_queue *queue);
};

void iio_dma_buffer_block_done(struct iio_dma_buffer_block *block);
void iio_dma_buffer_block_list_abort(struct iio_dma_buffer_queue *queue,
	struct list_head *list);

int iio_dma_buffer_enable(struct iio_buffer *buffer,
	struct iio_dev *indio_dev);
int iio_dma_buffer_disable(struct iio_buffer *buffer,
	struct iio_dev *indio_dev);
int iio_dma_buffer_read(struct iio_buffer *buffer, size_t n,
	char __user *user_buffer);
size_t iio_dma_buffer_data_available(struct iio_buffer *buffer);
int iio_dma_buffer_set_bytes_per_datum(struct iio_buffer *buffer, size_t bpd);
int iio_dma_buffer_set_length(struct iio_buffer *buffer, unsigned int length);
int iio_dma_buffer_request_update(struct iio_buffer *buffer);

int iio_dma_buffer_init(struct iio_dma_buffer_queue *queue,
	struct device *dma_dev, const struct iio_dma_buffer_ops *ops);
void iio_dma_buffer_exit(struct iio_dma_buffer_queue *queue);
void iio_dma_buffer_release(struct iio_dma_buffer_queue *queue);

#endif
