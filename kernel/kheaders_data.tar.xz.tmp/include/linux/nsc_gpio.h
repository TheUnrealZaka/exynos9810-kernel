

struct nsc_gpio_ops {
	struct module*	owner;
	u32	(*gpio_config)	(unsigned iminor, u32 mask, u32 bits);
	void	(*gpio_dump)	(struct nsc_gpio_ops *amp, unsigned iminor);
	int	(*gpio_get)	(unsigned iminor);
	void	(*gpio_set)	(unsigned iminor, int state);
	void	(*gpio_change)	(unsigned iminor);
	int	(*gpio_current)	(unsigned iminor);
	struct device*	dev;	
};

extern ssize_t nsc_gpio_write(struct file *file, const char __user *data,
			      size_t len, loff_t *ppos);

extern ssize_t nsc_gpio_read(struct file *file, char __user *buf,
			     size_t len, loff_t *ppos);

extern void nsc_gpio_dump(struct nsc_gpio_ops *amp, unsigned index);

