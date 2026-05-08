

#ifndef __LINUX_IRQCHIP_XTENSA_PIC_H
#define __LINUX_IRQCHIP_XTENSA_PIC_H

struct device_node;
int xtensa_pic_init_legacy(struct device_node *interrupt_parent);

#endif 
