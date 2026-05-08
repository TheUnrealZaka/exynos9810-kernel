



struct hci_h4p_platform_data {
	int chip_type;
	int bt_sysclk;
	unsigned int bt_wakeup_gpio;
	unsigned int host_wakeup_gpio;
	unsigned int reset_gpio;
	int reset_gpio_shared;
	unsigned int uart_irq;
	phys_addr_t uart_base;
	const char *uart_iclk;
	const char *uart_fclk;
	void (*set_pm_limits)(struct device *dev, bool set);
};
