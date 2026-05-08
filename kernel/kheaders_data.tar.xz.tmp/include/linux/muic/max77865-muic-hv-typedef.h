

#ifndef __MAX77865_MUIC_HV_TYPEDEF_H__
#define __MAX77865_MUIC_HV_TYPEDEF_H__


typedef enum {
	MUIC_AFC_IRQ_VDNMON = 0,
	MUIC_AFC_IRQ_MRXRDY,
	MUIC_AFC_IRQ_VBADC,
	MUIC_AFC_IRQ_MPNACK,

	MUIC_AFC_IRQ_DONTCARE = 0xff,
} muic_afc_irq_t;


typedef struct max77865_muic_afc_data {
	muic_attached_dev_t		new_dev;
	const char			*afc_name;
	muic_afc_irq_t			afc_irq;
	u8				hvcontrol1_dpdnvden;
	u8				status3_vbadc;
	u8				status3_vdnmon;
	int				function_num;
	struct max77865_muic_afc_data	*next;
} muic_afc_data_t;

#endif 
