

#ifndef __LINUX_MFD_MAX77705_CC_H
#define __LINUX_MFD_MAX77705_CC_H
#include <linux/ccic/max77705.h>


#define MAX77705_CC_NAME	"max77705_CC"


#define MAX77865_IRQSRC_CHG		(1 << 0)
#define MAX77865_IRQSRC_FG      (1 << 2)

struct max77705_cc_data {

	
	int irq_vconncop;
	int irq_vsafe0v;
	int irq_detabrt;
	int irq_vconnsc;
	int irq_ccpinstat;
	int irq_ccistat;
	int irq_ccvcnstat;
	int irq_ccstat;

	u8 usbc_status1;
	u8 usbc_status2;
	u8 bc_status;
	u8 cc_status0;
	u8 cc_status1;
	u8 pd_status0;
	u8 pd_status1;

	u8 opcode_res;

	
	u8 vconnocp;
	
	u8 vconnsc;
	
	u8 vsafe0v;
	
	u8 detabrt;
	
	u8 ccpinstat;
	
	u8 ccistat;
	
	u8 ccvcnstat;
	
	u8 ccstat;

	enum max77705_vcon_role	current_vcon;
	enum max77705_vcon_role	previous_vcon;
	enum max77705_power_role current_pr;
	enum max77705_power_role previous_pr;

	
	struct wake_lock vconncop_wake_lock;
	struct wake_lock vsafe0v_wake_lock;
	struct wake_lock detabrt_wake_lock;
	struct wake_lock vconnsc_wake_lock;
	struct wake_lock ccpinstat_wake_lock;
	struct wake_lock ccistat_wake_lock;
	struct wake_lock ccvcnstat_wake_lock;
	struct wake_lock ccstat_wake_lock;
};
#endif
