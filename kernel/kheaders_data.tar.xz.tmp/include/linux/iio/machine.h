

#ifndef __LINUX_IIO_MACHINE_H__
#define __LINUX_IIO_MACHINE_H__


struct iio_map {
	const char *adc_channel_label;
	const char *consumer_dev_name;
	const char *consumer_channel;
	void *consumer_data;
};

#endif
