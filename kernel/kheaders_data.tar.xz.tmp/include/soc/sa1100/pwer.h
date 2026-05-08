#ifndef SOC_SA1100_PWER_H
#define SOC_SA1100_PWER_H



int sa11x0_gpio_set_wake(unsigned int gpio, unsigned int on);
int sa11x0_sc_set_wake(unsigned int irq, unsigned int on);

#endif
