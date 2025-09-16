

#ifndef __LINUX_EXTCON_H__
#define __LINUX_EXTCON_H__

#include <linux/device.h>


#define EXTCON_TYPE_USB		BIT(0)	
#define EXTCON_TYPE_CHG		BIT(1)	
#define EXTCON_TYPE_JACK	BIT(2)	
#define EXTCON_TYPE_DISP	BIT(3)	
#define EXTCON_TYPE_MISC	BIT(4)	


#define EXTCON_NONE		0


#define EXTCON_USB		1
#define EXTCON_USB_HOST		2


#define EXTCON_CHG_USB_SDP	5	
#define EXTCON_CHG_USB_DCP	6	
#define EXTCON_CHG_USB_CDP	7	
#define EXTCON_CHG_USB_ACA	8	
#define EXTCON_CHG_USB_FAST	9
#define EXTCON_CHG_USB_SLOW	10
#define EXTCON_CHG_WPT		11	


#define EXTCON_JACK_MICROPHONE	20
#define EXTCON_JACK_HEADPHONE	21
#define EXTCON_JACK_LINE_IN	22
#define EXTCON_JACK_LINE_OUT	23
#define EXTCON_JACK_VIDEO_IN	24
#define EXTCON_JACK_VIDEO_OUT	25
#define EXTCON_JACK_SPDIF_IN	26	
#define EXTCON_JACK_SPDIF_OUT	27


#define EXTCON_DISP_HDMI	40	
#define EXTCON_DISP_MHL		41	
#define EXTCON_DISP_DVI		42	
#define EXTCON_DISP_VGA		43	
#define EXTCON_DISP_DP		44	
#define EXTCON_DISP_HMD		45	


#define EXTCON_DOCK		60
#define EXTCON_JIG		61
#define EXTCON_MECHANICAL	62

#define EXTCON_NUM		63




#define EXTCON_PROP_USB_VBUS		0
#define EXTCON_PROP_USB_TYPEC_POLARITY	1
#define EXTCON_PROP_USB_SS		2

#define EXTCON_PROP_USB_MIN		0
#define EXTCON_PROP_USB_MAX		2
#define EXTCON_PROP_USB_CNT	(EXTCON_PROP_USB_MAX - EXTCON_PROP_USB_MIN + 1)


#define EXTCON_PROP_CHG_MIN		50
#define EXTCON_PROP_CHG_MAX		50
#define EXTCON_PROP_CHG_CNT	(EXTCON_PROP_CHG_MAX - EXTCON_PROP_CHG_MIN + 1)


#define EXTCON_PROP_JACK_MIN		100
#define EXTCON_PROP_JACK_MAX		100
#define EXTCON_PROP_JACK_CNT (EXTCON_PROP_JACK_MAX - EXTCON_PROP_JACK_MIN + 1)


#define EXTCON_PROP_DISP_HPD		150


#define EXTCON_PROP_DISP_MIN		150
#define EXTCON_PROP_DISP_MAX		151
#define EXTCON_PROP_DISP_CNT (EXTCON_PROP_DISP_MAX - EXTCON_PROP_DISP_MIN + 1)


union extcon_property_value {
	int intval;	
};

struct extcon_cable;


struct extcon_dev {
	
	const char *name;
	const unsigned int *supported_cable;
	const u32 *mutually_exclusive;

	
	struct device dev;
	struct raw_notifier_head *nh;
	struct list_head entry;
	int max_supported;
	spinlock_t lock;	
	u32 state;

	
	struct device_type extcon_dev_type;
	struct extcon_cable *cables;

	
	struct attribute_group attr_g_muex;
	struct attribute **attrs_muex;
	struct device_attribute *d_attrs_muex;
};

#if IS_ENABLED(CONFIG_EXTCON)


extern int extcon_dev_register(struct extcon_dev *edev);
extern void extcon_dev_unregister(struct extcon_dev *edev);
extern int devm_extcon_dev_register(struct device *dev,
				    struct extcon_dev *edev);
extern void devm_extcon_dev_unregister(struct device *dev,
				       struct extcon_dev *edev);
extern struct extcon_dev *extcon_get_extcon_dev(const char *extcon_name);


extern struct extcon_dev *extcon_dev_allocate(const unsigned int *cable);
extern void extcon_dev_free(struct extcon_dev *edev);
extern struct extcon_dev *devm_extcon_dev_allocate(struct device *dev,
						   const unsigned int *cable);
extern void devm_extcon_dev_free(struct device *dev, struct extcon_dev *edev);


extern int extcon_get_state(struct extcon_dev *edev, unsigned int id);
extern int extcon_set_state(struct extcon_dev *edev, unsigned int id,
				   bool cable_state);
extern int extcon_set_state_sync(struct extcon_dev *edev, unsigned int id,
				bool cable_state);

extern int extcon_sync(struct extcon_dev *edev, unsigned int id);


extern int extcon_get_property(struct extcon_dev *edev, unsigned int id,
				unsigned int prop,
				union extcon_property_value *prop_val);
extern int extcon_set_property(struct extcon_dev *edev, unsigned int id,
				unsigned int prop,
				union extcon_property_value prop_val);
extern int extcon_set_property_sync(struct extcon_dev *edev, unsigned int id,
				unsigned int prop,
				union extcon_property_value prop_val);


extern int extcon_get_property_capability(struct extcon_dev *edev,
				unsigned int id, unsigned int prop);
extern int extcon_set_property_capability(struct extcon_dev *edev,
				unsigned int id, unsigned int prop);


extern int extcon_register_notifier(struct extcon_dev *edev, unsigned int id,
				    struct notifier_block *nb);
extern int extcon_unregister_notifier(struct extcon_dev *edev, unsigned int id,
				    struct notifier_block *nb);
extern int devm_extcon_register_notifier(struct device *dev,
				struct extcon_dev *edev, unsigned int id,
				struct notifier_block *nb);
extern void devm_extcon_unregister_notifier(struct device *dev,
				struct extcon_dev *edev, unsigned int id,
				struct notifier_block *nb);


extern struct extcon_dev *extcon_get_edev_by_phandle(struct device *dev,
						     int index);


extern const char *extcon_get_edev_name(struct extcon_dev *edev);


#else 
static inline int extcon_dev_register(struct extcon_dev *edev)
{
	return 0;
}

static inline void extcon_dev_unregister(struct extcon_dev *edev) { }

static inline int devm_extcon_dev_register(struct device *dev,
					   struct extcon_dev *edev)
{
	return -EINVAL;
}

static inline void devm_extcon_dev_unregister(struct device *dev,
					      struct extcon_dev *edev) { }

static inline struct extcon_dev *extcon_dev_allocate(const unsigned int *cable)
{
	return ERR_PTR(-ENOSYS);
}

static inline void extcon_dev_free(struct extcon_dev *edev) { }

static inline struct extcon_dev *devm_extcon_dev_allocate(struct device *dev,
						const unsigned int *cable)
{
	return ERR_PTR(-ENOSYS);
}

static inline void devm_extcon_dev_free(struct extcon_dev *edev) { }


static inline int extcon_get_state(struct extcon_dev *edev, unsigned int id)
{
	return 0;
}

static inline int extcon_set_state(struct extcon_dev *edev, unsigned int id,
				bool cable_state)
{
	return 0;
}

static inline int extcon_set_state_sync(struct extcon_dev *edev, unsigned int id,
				bool cable_state)
{
	return 0;
}

static inline int extcon_sync(struct extcon_dev *edev, unsigned int id)
{
	return 0;
}

static inline int extcon_get_property(struct extcon_dev *edev, unsigned int id,
					unsigned int prop,
					union extcon_property_value *prop_val)
{
	return 0;
}
static inline int extcon_set_property(struct extcon_dev *edev, unsigned int id,
					unsigned int prop,
					union extcon_property_value prop_val)
{
	return 0;
}

static inline int extcon_set_property_sync(struct extcon_dev *edev,
					unsigned int id, unsigned int prop,
					union extcon_property_value prop_val)
{
	return 0;
}

static inline int extcon_get_property_capability(struct extcon_dev *edev,
					unsigned int id, unsigned int prop)
{
	return 0;
}

static inline int extcon_set_property_capability(struct extcon_dev *edev,
					unsigned int id, unsigned int prop)
{
	return 0;
}

static inline struct extcon_dev *extcon_get_extcon_dev(const char *extcon_name)
{
	return NULL;
}

static inline int extcon_register_notifier(struct extcon_dev *edev,
					unsigned int id,
					struct notifier_block *nb)
{
	return 0;
}

static inline int extcon_unregister_notifier(struct extcon_dev *edev,
					unsigned int id,
					struct notifier_block *nb)
{
	return 0;
}

static inline int devm_extcon_register_notifier(struct device *dev,
				struct extcon_dev *edev, unsigned int id,
				struct notifier_block *nb)
{
	return -ENOSYS;
}

static inline  void devm_extcon_unregister_notifier(struct device *dev,
				struct extcon_dev *edev, unsigned int id,
				struct notifier_block *nb) { }

static inline struct extcon_dev *extcon_get_edev_by_phandle(struct device *dev,
							    int index)
{
	return ERR_PTR(-ENODEV);
}
#endif 


struct extcon_specific_cable_nb {
       struct notifier_block *user_nb;
       int cable_index;
       struct extcon_dev *edev;
       unsigned long previous_value;
};

static inline int extcon_register_interest(struct extcon_specific_cable_nb *obj,
			const char *extcon_name, const char *cable_name,
			struct notifier_block *nb)
{
	return -EINVAL;
}

static inline int extcon_unregister_interest(struct extcon_specific_cable_nb
						    *obj)
{
	return -EINVAL;
}

static inline int extcon_get_cable_state_(struct extcon_dev *edev, unsigned int id)
{
	return extcon_get_state(edev, id);
}

static inline int extcon_set_cable_state_(struct extcon_dev *edev, unsigned int id,
				   bool cable_state)
{
	return extcon_set_state_sync(edev, id, cable_state);
}
#endif 
