

#ifndef __S2MPS18_MFD_H__
#define __S2MPS18_MFD_H__
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define MFD_DEV_NAME "s2mps18"

struct s2mps18_regulator_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *reg_node;
};


struct sec_opmode_data {
	int id;
	unsigned int mode;
};


enum sec_opmode {
	SEC_OPMODE_OFF,
	SEC_OPMODE_SUSPEND,
	SEC_OPMODE_LOWPOWER,
	SEC_OPMODE_ON,
	SEC_OPMODE_TCXO = 0x2,
	SEC_OPMODE_MIF = 0x2,
};


struct sec_wtsr_smpl {
	bool wtsr_en;
	bool smpl_en;
	int wtsr_timer_val;
	int smpl_timer_val;
	bool check_jigon;
};

struct s2mps18_platform_data {
	
	int irq_base;
	int irq_gpio;
	bool wakeup;

	int num_regulators;
	struct	s2mps18_regulator_data *regulators;
	struct	sec_opmode_data		*opmode;
	struct	mfd_cell *sub_devices;
	int 	num_subdevs;

	int	(*cfg_pmic_irq)(void);
	int	device_type;
	int	ono;
	int	buck_ramp_delay;

	int				smpl_warn;
	bool				smpl_warn_en;
	unsigned int                    smpl_warn_vth;
	unsigned int                    smpl_warn_hys;
	bool				cpu_ocp_warn_en;
	bool				gpu_ocp_warn_en;
	int				cpu_ocp_warn_reset;
	int				gpu_ocp_warn_reset;
	int				cpu_ocp_warn_lv;
	int				gpu_ocp_warn_lv;
	bool				g3d_en;

	
	int		adc_mode;
	
	int		adc_sync_mode;

	
	struct sec_wtsr_smpl *wtsr_smpl;

	struct rtc_time *init_time;
	int	osc_bias_up;
	int	cap_sel;
	int	osc_xin;
	int	osc_xout;

	bool	use_i2c_speedy;
};

struct s2mps18 {
	struct regmap *regmap;
};

#endif 

