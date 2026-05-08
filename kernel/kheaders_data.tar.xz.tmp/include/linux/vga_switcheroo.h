

#ifndef _LINUX_VGA_SWITCHEROO_H_
#define _LINUX_VGA_SWITCHEROO_H_

#include <linux/fb.h>

struct pci_dev;


enum vga_switcheroo_handler_flags_t {
	VGA_SWITCHEROO_CAN_SWITCH_DDC	= (1 << 0),
	VGA_SWITCHEROO_NEEDS_EDP_CONFIG	= (1 << 1),
};


enum vga_switcheroo_state {
	VGA_SWITCHEROO_OFF,
	VGA_SWITCHEROO_ON,
	
	VGA_SWITCHEROO_NOT_FOUND,
};


enum vga_switcheroo_client_id {
	VGA_SWITCHEROO_UNKNOWN_ID = -1,
	VGA_SWITCHEROO_IGD,
	VGA_SWITCHEROO_DIS,
	VGA_SWITCHEROO_MAX_CLIENTS,
};


struct vga_switcheroo_handler {
	int (*init)(void);
	int (*switchto)(enum vga_switcheroo_client_id id);
	int (*switch_ddc)(enum vga_switcheroo_client_id id);
	int (*power_state)(enum vga_switcheroo_client_id id,
			   enum vga_switcheroo_state state);
	enum vga_switcheroo_client_id (*get_client_id)(struct pci_dev *pdev);
};


struct vga_switcheroo_client_ops {
	void (*set_gpu_state)(struct pci_dev *dev, enum vga_switcheroo_state);
	void (*reprobe)(struct pci_dev *dev);
	bool (*can_switch)(struct pci_dev *dev);
};

#if defined(CONFIG_VGA_SWITCHEROO)
void vga_switcheroo_unregister_client(struct pci_dev *dev);
int vga_switcheroo_register_client(struct pci_dev *dev,
				   const struct vga_switcheroo_client_ops *ops,
				   bool driver_power_control);
int vga_switcheroo_register_audio_client(struct pci_dev *pdev,
					 const struct vga_switcheroo_client_ops *ops,
					 enum vga_switcheroo_client_id id);

void vga_switcheroo_client_fb_set(struct pci_dev *dev,
				  struct fb_info *info);

int vga_switcheroo_register_handler(const struct vga_switcheroo_handler *handler,
				    enum vga_switcheroo_handler_flags_t handler_flags);
void vga_switcheroo_unregister_handler(void);
enum vga_switcheroo_handler_flags_t vga_switcheroo_handler_flags(void);
int vga_switcheroo_lock_ddc(struct pci_dev *pdev);
int vga_switcheroo_unlock_ddc(struct pci_dev *pdev);

int vga_switcheroo_process_delayed_switch(void);

bool vga_switcheroo_client_probe_defer(struct pci_dev *pdev);
enum vga_switcheroo_state vga_switcheroo_get_client_state(struct pci_dev *dev);

void vga_switcheroo_set_dynamic_switch(struct pci_dev *pdev, enum vga_switcheroo_state dynamic);

int vga_switcheroo_init_domain_pm_ops(struct device *dev, struct dev_pm_domain *domain);
void vga_switcheroo_fini_domain_pm_ops(struct device *dev);
int vga_switcheroo_init_domain_pm_optimus_hdmi_audio(struct device *dev, struct dev_pm_domain *domain);
#else

static inline void vga_switcheroo_unregister_client(struct pci_dev *dev) {}
static inline int vga_switcheroo_register_client(struct pci_dev *dev,
		const struct vga_switcheroo_client_ops *ops, bool driver_power_control) { return 0; }
static inline void vga_switcheroo_client_fb_set(struct pci_dev *dev, struct fb_info *info) {}
static inline int vga_switcheroo_register_handler(const struct vga_switcheroo_handler *handler,
		enum vga_switcheroo_handler_flags_t handler_flags) { return 0; }
static inline int vga_switcheroo_register_audio_client(struct pci_dev *pdev,
	const struct vga_switcheroo_client_ops *ops,
	enum vga_switcheroo_client_id id) { return 0; }
static inline void vga_switcheroo_unregister_handler(void) {}
static inline enum vga_switcheroo_handler_flags_t vga_switcheroo_handler_flags(void) { return 0; }
static inline int vga_switcheroo_lock_ddc(struct pci_dev *pdev) { return -ENODEV; }
static inline int vga_switcheroo_unlock_ddc(struct pci_dev *pdev) { return -ENODEV; }
static inline int vga_switcheroo_process_delayed_switch(void) { return 0; }
static inline bool vga_switcheroo_client_probe_defer(struct pci_dev *pdev) { return false; }
static inline enum vga_switcheroo_state vga_switcheroo_get_client_state(struct pci_dev *dev) { return VGA_SWITCHEROO_ON; }

static inline void vga_switcheroo_set_dynamic_switch(struct pci_dev *pdev, enum vga_switcheroo_state dynamic) {}

static inline int vga_switcheroo_init_domain_pm_ops(struct device *dev, struct dev_pm_domain *domain) { return -EINVAL; }
static inline void vga_switcheroo_fini_domain_pm_ops(struct device *dev) {}
static inline int vga_switcheroo_init_domain_pm_optimus_hdmi_audio(struct device *dev, struct dev_pm_domain *domain) { return -EINVAL; }

#endif
#endif 
