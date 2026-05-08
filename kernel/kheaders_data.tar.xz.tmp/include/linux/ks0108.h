

#ifndef _KS0108_H_
#define _KS0108_H_


extern void ks0108_writedata(unsigned char byte);


extern void ks0108_writecontrol(unsigned char byte);


extern void ks0108_displaystate(unsigned char state);


extern void ks0108_startline(unsigned char startline);


extern void ks0108_address(unsigned char address);


extern void ks0108_page(unsigned char page);


extern unsigned char ks0108_isinited(void);

#endif 
