
#include <linux/mutex.h>
#include <linux/platform_device.h>

#ifndef _LINUX_FPGA_MGR_H
#define _LINUX_FPGA_MGR_H

struct fpga_manager;


enum fpga_mgr_states {
	
	FPGA_MGR_STATE_UNKNOWN,
	FPGA_MGR_STATE_POWER_OFF,
	FPGA_MGR_STATE_POWER_UP,
	FPGA_MGR_STATE_RESET,

	
	FPGA_MGR_STATE_FIRMWARE_REQ,
	FPGA_MGR_STATE_FIRMWARE_REQ_ERR,

	
	FPGA_MGR_STATE_WRITE_INIT,
	FPGA_MGR_STATE_WRITE_INIT_ERR,
	FPGA_MGR_STATE_WRITE,
	FPGA_MGR_STATE_WRITE_ERR,
	FPGA_MGR_STATE_WRITE_COMPLETE,
	FPGA_MGR_STATE_WRITE_COMPLETE_ERR,

	
	FPGA_MGR_STATE_OPERATING,
};


#define FPGA_MGR_PARTIAL_RECONFIG	BIT(0)


struct fpga_manager_ops {
	enum fpga_mgr_states (*state)(struct fpga_manager *mgr);
	int (*write_init)(struct fpga_manager *mgr, u32 flags,
			  const char *buf, size_t count);
	int (*write)(struct fpga_manager *mgr, const char *buf, size_t count);
	int (*write_complete)(struct fpga_manager *mgr, u32 flags);
	void (*fpga_remove)(struct fpga_manager *mgr);
};


struct fpga_manager {
	const char *name;
	struct device dev;
	struct mutex ref_mutex;
	enum fpga_mgr_states state;
	const struct fpga_manager_ops *mops;
	void *priv;
};

#define to_fpga_manager(d) container_of(d, struct fpga_manager, dev)

int fpga_mgr_buf_load(struct fpga_manager *mgr, u32 flags,
		      const char *buf, size_t count);

int fpga_mgr_firmware_load(struct fpga_manager *mgr, u32 flags,
			   const char *image_name);

struct fpga_manager *of_fpga_mgr_get(struct device_node *node);

void fpga_mgr_put(struct fpga_manager *mgr);

int fpga_mgr_register(struct device *dev, const char *name,
		      const struct fpga_manager_ops *mops, void *priv);

void fpga_mgr_unregister(struct device *dev);

#endif 
