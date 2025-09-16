

#ifndef __LINUX_IIO_TRIGGER_CONSUMER_H__
#define __LINUX_IIO_TRIGGER_CONSUMER_H__

#include <linux/interrupt.h>
#include <linux/types.h>

struct iio_dev;
struct iio_trigger;


struct iio_poll_func {
	struct iio_dev *indio_dev;
	irqreturn_t (*h)(int irq, void *p);
	irqreturn_t (*thread)(int irq, void *p);
	int type;
	char *name;
	int irq;
	s64 timestamp;
};


struct iio_poll_func
*iio_alloc_pollfunc(irqreturn_t (*h)(int irq, void *p),
		    irqreturn_t (*thread)(int irq, void *p),
		    int type,
		    struct iio_dev *indio_dev,
		    const char *fmt,
		    ...);
void iio_dealloc_pollfunc(struct iio_poll_func *pf);
irqreturn_t iio_pollfunc_store_time(int irq, void *p);

void iio_trigger_notify_done(struct iio_trigger *trig);


int iio_triggered_buffer_postenable(struct iio_dev *indio_dev);
int iio_triggered_buffer_predisable(struct iio_dev *indio_dev);

#endif
