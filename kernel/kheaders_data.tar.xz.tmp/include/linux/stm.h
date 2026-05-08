

#ifndef _STM_H_
#define _STM_H_

#include <linux/device.h>


enum stp_packet_type {
	STP_PACKET_DATA = 0,
	STP_PACKET_FLAG,
	STP_PACKET_USER,
	STP_PACKET_MERR,
	STP_PACKET_GERR,
	STP_PACKET_TRIG,
	STP_PACKET_XSYNC,
};


enum stp_packet_flags {
	STP_PACKET_MARKED	= 0x1,
	STP_PACKET_TIMESTAMPED	= 0x2,
};

struct stp_policy;

struct stm_device;


struct stm_data {
	const char		*name;
	struct stm_device	*stm;
	unsigned int		sw_start;
	unsigned int		sw_end;
	unsigned int		sw_nchannels;
	unsigned int		sw_mmiosz;
	unsigned int		hw_override;
	ssize_t			(*packet)(struct stm_data *, unsigned int,
					  unsigned int, unsigned int,
					  unsigned int, unsigned int,
					  const unsigned char *);
	phys_addr_t		(*mmio_addr)(struct stm_data *, unsigned int,
					     unsigned int, unsigned int);
	int			(*link)(struct stm_data *, unsigned int,
					unsigned int);
	void			(*unlink)(struct stm_data *, unsigned int,
					  unsigned int);
	long			(*set_options)(struct stm_data *, unsigned int,
					       unsigned int, unsigned int,
					       unsigned long);
};

int stm_register_device(struct device *parent, struct stm_data *stm_data,
			struct module *owner);
void stm_unregister_device(struct stm_data *stm_data);

struct stm_source_device;


struct stm_source_data {
	const char		*name;
	struct stm_source_device *src;
	unsigned int		percpu;
	unsigned int		nr_chans;
	int			(*link)(struct stm_source_data *data);
	void			(*unlink)(struct stm_source_data *data);
};

int stm_source_register_device(struct device *parent,
			       struct stm_source_data *data);
void stm_source_unregister_device(struct stm_source_data *data);

int stm_source_write(struct stm_source_data *data, unsigned int chan,
		     const char *buf, size_t count);

#endif 
