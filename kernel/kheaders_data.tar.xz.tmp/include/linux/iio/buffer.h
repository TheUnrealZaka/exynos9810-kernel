

#ifndef _IIO_BUFFER_GENERIC_H_
#define _IIO_BUFFER_GENERIC_H_
#include <linux/sysfs.h>
#include <linux/iio/iio.h>
#include <linux/kref.h>

#ifdef CONFIG_IIO_BUFFER

struct iio_buffer;


#define INDIO_BUFFER_FLAG_FIXED_WATERMARK BIT(0)


struct iio_buffer_access_funcs {
	int (*store_to)(struct iio_buffer *buffer, const void *data);
	int (*read_first_n)(struct iio_buffer *buffer,
			    size_t n,
			    char __user *buf);
	size_t (*data_available)(struct iio_buffer *buffer);

	int (*request_update)(struct iio_buffer *buffer);

	int (*set_bytes_per_datum)(struct iio_buffer *buffer, size_t bpd);
	int (*set_length)(struct iio_buffer *buffer, unsigned int length);

	int (*enable)(struct iio_buffer *buffer, struct iio_dev *indio_dev);
	int (*disable)(struct iio_buffer *buffer, struct iio_dev *indio_dev);

	void (*release)(struct iio_buffer *buffer);

	unsigned int modes;
	unsigned int flags;
};


struct iio_buffer {
	unsigned int				length;
	size_t					bytes_per_datum;
	struct attribute_group			*scan_el_attrs;
	long					*scan_mask;
	bool					scan_timestamp;
	const struct iio_buffer_access_funcs	*access;
	struct list_head			scan_el_dev_attr_list;
	struct attribute_group			buffer_group;
	struct attribute_group			scan_el_group;
	wait_queue_head_t			pollq;
	bool					stufftoread;
	const struct attribute			**attrs;
	struct list_head			demux_list;
	void					*demux_bounce;
	struct list_head			buffer_list;
	struct kref				ref;
	unsigned int				watermark;
};


int iio_update_buffers(struct iio_dev *indio_dev,
		       struct iio_buffer *insert_buffer,
		       struct iio_buffer *remove_buffer);


void iio_buffer_init(struct iio_buffer *buffer);

int iio_scan_mask_query(struct iio_dev *indio_dev,
			struct iio_buffer *buffer, int bit);


int iio_push_to_buffers(struct iio_dev *indio_dev, const void *data);


static inline int iio_push_to_buffers_with_timestamp(struct iio_dev *indio_dev,
	void *data, int64_t timestamp)
{
	if (indio_dev->scan_timestamp) {
		size_t ts_offset = indio_dev->scan_bytes / sizeof(int64_t) - 1;
		((int64_t *)data)[ts_offset] = timestamp;
	}

	return iio_push_to_buffers(indio_dev, data);
}

int iio_update_demux(struct iio_dev *indio_dev);

bool iio_validate_scan_mask_onehot(struct iio_dev *indio_dev,
	const unsigned long *mask);

struct iio_buffer *iio_buffer_get(struct iio_buffer *buffer);
void iio_buffer_put(struct iio_buffer *buffer);


static inline void iio_device_attach_buffer(struct iio_dev *indio_dev,
	struct iio_buffer *buffer)
{
	indio_dev->buffer = iio_buffer_get(buffer);
}

#else 

static inline void iio_buffer_get(struct iio_buffer *buffer) {}
static inline void iio_buffer_put(struct iio_buffer *buffer) {}

#endif 

#endif 
