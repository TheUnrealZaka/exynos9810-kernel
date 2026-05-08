

#ifndef _ALTERA_H_
#define _ALTERA_H_

struct altera_config {
	void *dev;
	u8 *action;
	int (*jtag_io) (void *dev, int tms, int tdi, int tdo);
};

#if defined(CONFIG_ALTERA_STAPL) || \
		(defined(CONFIG_ALTERA_STAPL_MODULE) && defined(MODULE))

extern int altera_init(struct altera_config *config, const struct firmware *fw);
#else

static inline int altera_init(struct altera_config *config,
						const struct firmware *fw)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return 0;
}
#endif 

#endif 
