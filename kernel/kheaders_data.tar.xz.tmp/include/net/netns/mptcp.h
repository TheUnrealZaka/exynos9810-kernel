

#ifndef __NETNS_MPTCP_H__
#define __NETNS_MPTCP_H__

#include <linux/compiler.h>

enum {
	MPTCP_PM_FULLMESH = 0,
	MPTCP_PM_MAX
};

struct mptcp_mib;

struct netns_mptcp {
	DEFINE_SNMP_STAT(struct mptcp_mib, mptcp_statistics);

#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *proc_net_mptcp;
#endif

	void *path_managers[MPTCP_PM_MAX];
};

#endif 
