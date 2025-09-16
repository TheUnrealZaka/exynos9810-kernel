


#ifndef __ISP1704_CHARGER_H
#define __ISP1704_CHARGER_H

struct isp1704_charger_data {
	void		(*set_power)(bool on);
	int		enable_gpio;
};

#endif
