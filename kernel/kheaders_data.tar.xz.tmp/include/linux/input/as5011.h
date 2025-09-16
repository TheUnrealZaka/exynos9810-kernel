#ifndef _AS5011_H
#define _AS5011_H



struct as5011_platform_data {
	unsigned int button_gpio;
	unsigned int axis_irq; 
	unsigned long axis_irqflags;
	char xp, xn; 
	char yp, yn; 
};

#endif 
