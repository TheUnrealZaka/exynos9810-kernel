



#ifndef NCM_COMMON_H__
#define NCM_COMMON_H__


#define INET6_ADDRSTRLEN_NAP 48

#define NCM_VERSION 10

#define INIT_UID_NAP 0
#define INIT_PID_NAP 1

#define DNS_PORT_NAP 53

#define IPV4_FAMILY_NAP 2
#define IPV6_FAMILY_NAP 10

#define NCM_FLOW_TYPE_DEFAULT -1
#define NCM_FLOW_TYPE_ALL 0
#define NCM_FLOW_TYPE_OPEN 1
#define NCM_FLOW_TYPE_CLOSE 2
#define NCM_FLOW_TYPE_INTERMEDIATE 3

#include <linux/kernel.h>
#include <linux/inet.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <net/netfilter/nf_conntrack.h>

#define isIpv4AddressEqualsNull(srcaddr, dstaddr) ((((strcmp(srcaddr, "0.0.0.0")) || (strcmp(dstaddr, "0.0.0.0"))) == 0) ? 1 : 0)
#define isIpv6AddressEqualsNull(srcaddr, dstaddr) ((((strcmp(srcaddr, "0000:0000:0000:0000:0000:0000:0000:0000")) || (strcmp(dstaddr, "0000:0000:0000:0000:0000:0000:0000:0000"))) == 0) ? 1 : 0)


struct knox_socket_metadata {

	__u16   srcport;

	__u16   dstport;

	__u16   trans_proto;

	__u64   knox_sent;

	__u64   knox_recv;

	uid_t   knox_uid;

	pid_t   knox_pid;

	uid_t   knox_puid;

	__u64   open_time;

	__u64   close_time;

	char srcaddr[INET6_ADDRSTRLEN_NAP];

	char dstaddr[INET6_ADDRSTRLEN_NAP];

	char process_name[PROCESS_NAME_LEN_NAP];

	char parent_process_name[PROCESS_NAME_LEN_NAP];

	char domain_name[DOMAIN_NAME_LEN_NAP];

	uid_t   knox_uid_dns;

	pid_t   knox_ppid;

	char interface_name[IFNAMSIZ];

	int flow_type;

	struct work_struct work_kfifo;
};


struct knox_user_socket_metadata {

	__u16   srcport;

	__u16   dstport;

	__u16   trans_proto;

	__u64   knox_sent;

	__u64   knox_recv;

	uid_t   knox_uid;

	pid_t   knox_pid;

	uid_t   knox_puid;

	__u64   open_time;

	__u64   close_time;

	char srcaddr[INET6_ADDRSTRLEN_NAP];

	char dstaddr[INET6_ADDRSTRLEN_NAP];

	char process_name[PROCESS_NAME_LEN_NAP];

	char parent_process_name[PROCESS_NAME_LEN_NAP];

	char domain_name[DOMAIN_NAME_LEN_NAP];

	uid_t   knox_uid_dns;

	pid_t   knox_ppid;

	char interface_name[IFNAMSIZ];

	int flow_type;
};


extern unsigned int check_ncm_flag(void);
extern void knox_collect_conntrack_data(struct nf_conn *ct, int startStop, int where);

extern bool kfifo_status(void);
extern void insert_data_kfifo_kthread(struct knox_socket_metadata* knox_socket_metadata);
extern unsigned int check_intermediate_flag(void);
extern unsigned int get_intermediate_timeout(void);


#define NCM_DEBUG        1
#if NCM_DEBUG
#define NCM_LOGD(...) printk("ncm: "__VA_ARGS__)
#else
#define NCM_LOGD(...)
#endif 
#define NCM_LOGE(...) printk("ncm: "__VA_ARGS__)




#define __NCMIOC    0x120
#define NCM_ACTIVATED_OPEN       _IO(__NCMIOC, 2)
#define NCM_DEACTIVATED          _IO(__NCMIOC, 4)
#define NCM_ACTIVATED_CLOSE      _IO(__NCMIOC, 8)
#define NCM_ACTIVATED_ALL        _IO(__NCMIOC, 16)
#define NCM_GETVERSION           _IO(__NCMIOC, 32)
#define NCM_MATCH_VERSION        _IO(__NCMIOC, 64)

#endif

