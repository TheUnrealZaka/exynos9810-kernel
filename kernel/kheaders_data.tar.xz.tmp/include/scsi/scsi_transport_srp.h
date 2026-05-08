#ifndef SCSI_TRANSPORT_SRP_H
#define SCSI_TRANSPORT_SRP_H

#include <linux/transport_class.h>
#include <linux/types.h>
#include <linux/mutex.h>

#define SRP_RPORT_ROLE_INITIATOR 0
#define SRP_RPORT_ROLE_TARGET 1

struct srp_rport_identifiers {
	u8 port_id[16];
	u8 roles;
};


enum srp_rport_state {
	SRP_RPORT_RUNNING,
	SRP_RPORT_BLOCKED,
	SRP_RPORT_FAIL_FAST,
	SRP_RPORT_LOST,
};


struct srp_rport {
	

	struct device dev;

	u8 port_id[16];
	u8 roles;

	

	void			*lld_data;

	struct mutex		mutex;
	enum srp_rport_state	state;
	int			reconnect_delay;
	int			failed_reconnects;
	struct delayed_work	reconnect_work;
	int			fast_io_fail_tmo;
	int			dev_loss_tmo;
	struct delayed_work	fast_io_fail_work;
	struct delayed_work	dev_loss_work;
};


struct srp_function_template {
	
	bool has_rport_state;
	bool reset_timer_if_blocked;
	int *reconnect_delay;
	int *fast_io_fail_tmo;
	int *dev_loss_tmo;
	int (*reconnect)(struct srp_rport *rport);
	void (*terminate_rport_io)(struct srp_rport *rport);
	void (*rport_delete)(struct srp_rport *rport);
	
	int (* tsk_mgmt_response)(struct Scsi_Host *, u64, u64, int);
	int (* it_nexus_response)(struct Scsi_Host *, u64, int);
};

extern struct scsi_transport_template *
srp_attach_transport(struct srp_function_template *);
extern void srp_release_transport(struct scsi_transport_template *);

extern void srp_rport_get(struct srp_rport *rport);
extern void srp_rport_put(struct srp_rport *rport);
extern struct srp_rport *srp_rport_add(struct Scsi_Host *,
				       struct srp_rport_identifiers *);
extern void srp_rport_del(struct srp_rport *);
extern int srp_tmo_valid(int reconnect_delay, int fast_io_fail_tmo,
			 int dev_loss_tmo);
int srp_parse_tmo(int *tmo, const char *buf);
extern int srp_reconnect_rport(struct srp_rport *rport);
extern void srp_start_tl_fail_timers(struct srp_rport *rport);
extern void srp_remove_host(struct Scsi_Host *);
extern void srp_stop_rport_timers(struct srp_rport *rport);


static inline int srp_chkready(struct srp_rport *rport)
{
	switch (rport->state) {
	case SRP_RPORT_RUNNING:
	case SRP_RPORT_BLOCKED:
	default:
		return 0;
	case SRP_RPORT_FAIL_FAST:
		return DID_TRANSPORT_FAILFAST << 16;
	case SRP_RPORT_LOST:
		return DID_NO_CONNECT << 16;
	}
}

#endif
