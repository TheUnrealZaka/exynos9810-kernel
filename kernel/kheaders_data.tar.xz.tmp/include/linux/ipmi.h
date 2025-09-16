
#ifndef __LINUX_IPMI_H
#define __LINUX_IPMI_H

#include <uapi/linux/ipmi.h>

#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/acpi.h> 

struct module;
struct device;


typedef struct ipmi_user *ipmi_user_t;


struct ipmi_recv_msg {
	struct list_head link;

	
	int              recv_type;

	ipmi_user_t      user;
	struct ipmi_addr addr;
	long             msgid;
	struct kernel_ipmi_msg  msg;

	
	void             *user_msg_data;

	
	void (*done)(struct ipmi_recv_msg *msg);

	
	unsigned char   msg_data[IPMI_MAX_MSG_LENGTH];
};


void ipmi_free_recv_msg(struct ipmi_recv_msg *msg);

struct ipmi_user_hndl {
	
	void (*ipmi_recv_hndl)(struct ipmi_recv_msg *msg,
			       void                 *user_msg_data);

	
	void (*ipmi_watchdog_pretimeout)(void *handler_data);
};


int ipmi_create_user(unsigned int          if_num,
		     struct ipmi_user_hndl *handler,
		     void                  *handler_data,
		     ipmi_user_t           *user);


int ipmi_destroy_user(ipmi_user_t user);


void ipmi_get_version(ipmi_user_t   user,
		      unsigned char *major,
		      unsigned char *minor);


int ipmi_set_my_address(ipmi_user_t   user,
			unsigned int  channel,
			unsigned char address);
int ipmi_get_my_address(ipmi_user_t   user,
			unsigned int  channel,
			unsigned char *address);
int ipmi_set_my_LUN(ipmi_user_t   user,
		    unsigned int  channel,
		    unsigned char LUN);
int ipmi_get_my_LUN(ipmi_user_t   user,
		    unsigned int  channel,
		    unsigned char *LUN);


int ipmi_request_settime(ipmi_user_t      user,
			 struct ipmi_addr *addr,
			 long             msgid,
			 struct kernel_ipmi_msg  *msg,
			 void             *user_msg_data,
			 int              priority,
			 int              max_retries,
			 unsigned int     retry_time_ms);


int ipmi_request_supply_msgs(ipmi_user_t          user,
			     struct ipmi_addr     *addr,
			     long                 msgid,
			     struct kernel_ipmi_msg *msg,
			     void                 *user_msg_data,
			     void                 *supplied_smi,
			     struct ipmi_recv_msg *supplied_recv,
			     int                  priority);


void ipmi_poll_interface(ipmi_user_t user);


int ipmi_register_for_cmd(ipmi_user_t   user,
			  unsigned char netfn,
			  unsigned char cmd,
			  unsigned int  chans);
int ipmi_unregister_for_cmd(ipmi_user_t   user,
			    unsigned char netfn,
			    unsigned char cmd,
			    unsigned int  chans);


int ipmi_get_maintenance_mode(ipmi_user_t user);
int ipmi_set_maintenance_mode(ipmi_user_t user, int mode);


int ipmi_set_gets_events(ipmi_user_t user, bool val);


struct ipmi_smi_watcher {
	struct list_head link;

	
	struct module *owner;

	
	void (*new_smi)(int if_num, struct device *dev);
	void (*smi_gone)(int if_num);
};

int ipmi_smi_watcher_register(struct ipmi_smi_watcher *watcher);
int ipmi_smi_watcher_unregister(struct ipmi_smi_watcher *watcher);




unsigned int ipmi_addr_length(int addr_type);


int ipmi_validate_addr(struct ipmi_addr *addr, int len);


enum ipmi_addr_src {
	SI_INVALID = 0, SI_HOTMOD, SI_HARDCODED, SI_SPMI, SI_ACPI, SI_SMBIOS,
	SI_PCI,	SI_DEVICETREE, SI_LAST
};
const char *ipmi_addr_src_to_str(enum ipmi_addr_src src);

union ipmi_smi_info_union {
#ifdef CONFIG_ACPI
	
	struct {
		acpi_handle acpi_handle;
	} acpi_info;
#endif
};

struct ipmi_smi_info {
	enum ipmi_addr_src addr_src;

	
	struct device *dev;

	
	union ipmi_smi_info_union addr_info;
};


extern int ipmi_get_smi_info(int if_num, struct ipmi_smi_info *data);

#endif 
