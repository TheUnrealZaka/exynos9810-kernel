

#ifndef __VTS_MAILBOX_H
#define __VTS_MAILBOX_H


extern int mailbox_generate_interrupt(const struct platform_device *pdev, int hw_irq);


extern void mailbox_write_shared_register(const struct platform_device *pdev,
		const u32 *values, int start, int count);


extern void mailbox_read_shared_register(const struct platform_device *pdev,
		u32 *values, int start, int count);

#endif 