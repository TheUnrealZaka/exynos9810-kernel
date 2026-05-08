

#ifndef __LINUX_USB_OTG_FSM_H
#define __LINUX_USB_OTG_FSM_H

#include <linux/mutex.h>
#include <linux/errno.h>

#undef VERBOSE

#ifdef VERBOSE
#define VDBG(fmt, args...) pr_debug("[%s]  " fmt , \
				 __func__, ## args)
//#else
//#define VDBG(stuff...)	do {} while (0)
#endif

#ifdef VERBOSE
#define MPC_LOC printk("Current Location [%s]:[%d]\n", __FILE__, __LINE__)
#else
#define MPC_LOC do {} while (0)
#endif

#define PROTO_UNDEF	(0)
#define PROTO_HOST	(1)
#define PROTO_GADGET	(2)

#define OTG_STS_SELECTOR	0xF000	

#define HOST_REQUEST_FLAG	1	

#define T_HOST_REQ_POLL		(1500)	

enum otg_fsm_timer {
	
	A_WAIT_VRISE,
	A_WAIT_VFALL,
	A_WAIT_BCON,
	A_AIDL_BDIS,
	B_ASE0_BRST,
	A_BIDL_ADIS,
	B_AIDL_BDIS,

	
	B_SE0_SRP,
	B_SRP_FAIL,
	A_WAIT_ENUM,
	B_DATA_PLS,
	B_SSEND_SRP,

	NUM_OTG_FSM_TIMERS,
};


struct otg_fsm {
	int reset;

	
	int id;
	int adp_change;
	int power_up;
	int a_srp_det;
	int a_vbus_vld;
	int b_conn;
	int a_bus_resume;
	int a_bus_suspend;
	int a_conn;
	int b_se0_srp;
	int b_ssend_srp;
	int b_sess_vld;
	int test_device;
	int a_bus_drop;
	int a_bus_req;
	int b_bus_req;

	
	int a_sess_vld;
	int b_bus_resume;
	int b_bus_suspend;

	
	int drv_vbus;
	int loc_conn;
	int loc_sof;
	int adp_prb;
	int adp_sns;
	int data_pulse;

	
	int a_set_b_hnp_en;
	int b_srp_done;
	int b_hnp_enable;
	int a_clr_err;

	
	int a_bus_drop_inf;
	int a_bus_req_inf;
	int a_clr_err_inf;
	int b_bus_req_inf;
	
	int a_suspend_req_inf;

	
	int a_wait_vrise_tmout;
	int a_wait_vfall_tmout;
	int a_wait_bcon_tmout;
	int a_aidl_bdis_tmout;
	int b_ase0_brst_tmout;
	int a_bidl_adis_tmout;

	struct otg_fsm_ops *ops;
	struct usb_otg *otg;

	
	int protocol;
	struct mutex lock;
	u8 *host_req_flag;
	struct delayed_work hnp_polling_work;
	bool hnp_work_inited;
	bool state_changed;
};

struct otg_fsm_ops {
	void	(*chrg_vbus)(struct otg_fsm *fsm, int on);
	void	(*drv_vbus)(struct otg_fsm *fsm, int on);
	void	(*loc_conn)(struct otg_fsm *fsm, int on);
	void	(*loc_sof)(struct otg_fsm *fsm, int on);
	void	(*start_pulse)(struct otg_fsm *fsm);
	void	(*start_adp_prb)(struct otg_fsm *fsm);
	void	(*start_adp_sns)(struct otg_fsm *fsm);
	void	(*add_timer)(struct otg_fsm *fsm, enum otg_fsm_timer timer);
	void	(*del_timer)(struct otg_fsm *fsm, enum otg_fsm_timer timer);
	int	(*start_host)(struct otg_fsm *fsm, int on);
	int	(*start_gadget)(struct otg_fsm *fsm, int on);
};


static inline int otg_chrg_vbus(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->chrg_vbus)
		return -EOPNOTSUPP;
	fsm->ops->chrg_vbus(fsm, on);
	return 0;
}

static inline int otg_drv_vbus(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->drv_vbus)
		return -EOPNOTSUPP;
	if (fsm->drv_vbus != on) {
		fsm->drv_vbus = on;
		fsm->ops->drv_vbus(fsm, on);
	}
	return 0;
}

static inline int otg_loc_conn(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->loc_conn)
		return -EOPNOTSUPP;
	if (fsm->loc_conn != on) {
		fsm->loc_conn = on;
		fsm->ops->loc_conn(fsm, on);
	}
	return 0;
}

static inline int otg_loc_sof(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->loc_sof)
		return -EOPNOTSUPP;
	if (fsm->loc_sof != on) {
		fsm->loc_sof = on;
		fsm->ops->loc_sof(fsm, on);
	}
	return 0;
}

static inline int otg_start_pulse(struct otg_fsm *fsm)
{
	if (!fsm->ops->start_pulse)
		return -EOPNOTSUPP;
	if (!fsm->data_pulse) {
		fsm->data_pulse = 1;
		fsm->ops->start_pulse(fsm);
	}
	return 0;
}

static inline int otg_start_adp_prb(struct otg_fsm *fsm)
{
	if (!fsm->ops->start_adp_prb)
		return -EOPNOTSUPP;
	if (!fsm->adp_prb) {
		fsm->adp_sns = 0;
		fsm->adp_prb = 1;
		fsm->ops->start_adp_prb(fsm);
	}
	return 0;
}

static inline int otg_start_adp_sns(struct otg_fsm *fsm)
{
	if (!fsm->ops->start_adp_sns)
		return -EOPNOTSUPP;
	if (!fsm->adp_sns) {
		fsm->adp_sns = 1;
		fsm->ops->start_adp_sns(fsm);
	}
	return 0;
}

static inline int otg_add_timer(struct otg_fsm *fsm, enum otg_fsm_timer timer)
{
	if (!fsm->ops->add_timer)
		return -EOPNOTSUPP;
	fsm->ops->add_timer(fsm, timer);
	return 0;
}

static inline int otg_del_timer(struct otg_fsm *fsm, enum otg_fsm_timer timer)
{
	if (!fsm->ops->del_timer)
		return -EOPNOTSUPP;
	fsm->ops->del_timer(fsm, timer);
	return 0;
}

static inline int otg_start_host(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->start_host)
		return -EOPNOTSUPP;
	return fsm->ops->start_host(fsm, on);
}

static inline int otg_start_gadget(struct otg_fsm *fsm, int on)
{
	if (!fsm->ops->start_gadget)
		return -EOPNOTSUPP;
	return fsm->ops->start_gadget(fsm, on);
}

int otg_statemachine(struct otg_fsm *fsm);

#endif 
