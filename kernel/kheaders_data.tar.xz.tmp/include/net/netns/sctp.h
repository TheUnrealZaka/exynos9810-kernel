#ifndef __NETNS_SCTP_H__
#define __NETNS_SCTP_H__

struct sock;
struct proc_dir_entry;
struct sctp_mib;
struct ctl_table_header;

struct netns_sctp {
	DEFINE_SNMP_STAT(struct sctp_mib, sctp_statistics);

#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *proc_net_sctp;
#endif
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *sysctl_header;
#endif
	
	struct sock *ctl_sock;

	
	struct list_head local_addr_list;
	struct list_head addr_waitq;
	struct timer_list addr_wq_timer;
	struct list_head auto_asconf_splist;
	
	spinlock_t addr_wq_lock;

	
	spinlock_t local_addr_lock;

	
	unsigned int rto_initial;
	unsigned int rto_min;
	unsigned int rto_max;

	
	int rto_alpha;
	int rto_beta;

	
	int max_burst;

	
	int cookie_preserve_enable;

	
	char *sctp_hmac_alg;

	
	unsigned int valid_cookie_life;

	
	unsigned int sack_timeout;

	
	unsigned int hb_interval;

	
	int max_retrans_association;
	int max_retrans_path;
	int max_retrans_init;
	
	int pf_retrans;

	
	int pf_enable;

	
	int sndbuf_policy;

	
	int rcvbuf_policy;

	int default_auto_asconf;

	
	int addip_enable;
	int addip_noauth;

	
	int prsctp_enable;

	
	int auth_enable;

	
	int scope_policy;

	
	int rwnd_upd_shift;

	
	unsigned long max_autoclose;
};

#endif 
