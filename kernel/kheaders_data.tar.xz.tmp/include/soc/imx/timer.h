

#ifndef __SOC_IMX_TIMER_H__
#define __SOC_IMX_TIMER_H__

enum imx_gpt_type {
	GPT_TYPE_IMX1,		
	GPT_TYPE_IMX21,		
	GPT_TYPE_IMX31,		
	GPT_TYPE_IMX6DL,	
};


void mxc_timer_init(unsigned long pbase, int irq, enum imx_gpt_type type);

#endif  
