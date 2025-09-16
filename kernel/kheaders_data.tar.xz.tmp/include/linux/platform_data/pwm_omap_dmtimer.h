

#ifndef __PWM_OMAP_DMTIMER_PDATA_H
#define __PWM_OMAP_DMTIMER_PDATA_H


#define PWM_OMAP_DMTIMER_SRC_SYS_CLK			0x00
#define PWM_OMAP_DMTIMER_SRC_32_KHZ			0x01
#define PWM_OMAP_DMTIMER_SRC_EXT_CLK			0x02


#define PWM_OMAP_DMTIMER_INT_CAPTURE			(1 << 2)
#define PWM_OMAP_DMTIMER_INT_OVERFLOW			(1 << 1)
#define PWM_OMAP_DMTIMER_INT_MATCH			(1 << 0)


#define PWM_OMAP_DMTIMER_TRIGGER_NONE			0x00
#define PWM_OMAP_DMTIMER_TRIGGER_OVERFLOW		0x01
#define PWM_OMAP_DMTIMER_TRIGGER_OVERFLOW_AND_COMPARE	0x02

struct omap_dm_timer;
typedef struct omap_dm_timer pwm_omap_dmtimer;

struct pwm_omap_dmtimer_pdata {
	pwm_omap_dmtimer *(*request_by_node)(struct device_node *np);
	pwm_omap_dmtimer *(*request_specific)(int timer_id);
	pwm_omap_dmtimer *(*request)(void);

	int	(*free)(pwm_omap_dmtimer *timer);

	void	(*enable)(pwm_omap_dmtimer *timer);
	void	(*disable)(pwm_omap_dmtimer *timer);

	int	(*get_irq)(pwm_omap_dmtimer *timer);
	int	(*set_int_enable)(pwm_omap_dmtimer *timer, unsigned int value);
	int	(*set_int_disable)(pwm_omap_dmtimer *timer, u32 mask);

	struct clk *(*get_fclk)(pwm_omap_dmtimer *timer);

	int	(*start)(pwm_omap_dmtimer *timer);
	int	(*stop)(pwm_omap_dmtimer *timer);
	int	(*set_source)(pwm_omap_dmtimer *timer, int source);

	int	(*set_load)(pwm_omap_dmtimer *timer, int autoreload,
			unsigned int value);
	int	(*set_match)(pwm_omap_dmtimer *timer, int enable,
			unsigned int match);
	int	(*set_pwm)(pwm_omap_dmtimer *timer, int def_on,
			int toggle, int trigger);
	int	(*set_prescaler)(pwm_omap_dmtimer *timer, int prescaler);

	unsigned int (*read_counter)(pwm_omap_dmtimer *timer);
	int	(*write_counter)(pwm_omap_dmtimer *timer, unsigned int value);
	unsigned int (*read_status)(pwm_omap_dmtimer *timer);
	int	(*write_status)(pwm_omap_dmtimer *timer, unsigned int value);
};

#endif 
