

#ifndef __POWERCAP_H__
#define __POWERCAP_H__

#include <linux/device.h>
#include <linux/idr.h>



struct powercap_control_type;
struct powercap_zone;
struct powercap_zone_constraint;


struct powercap_control_type_ops {
	int (*set_enable) (struct powercap_control_type *, bool mode);
	int (*get_enable) (struct powercap_control_type *, bool *mode);
	int (*release) (struct powercap_control_type *);
};


struct powercap_control_type {
	struct device dev;
	struct idr idr;
	int nr_zones;
	const struct powercap_control_type_ops *ops;
	struct mutex lock;
	bool allocated;
	struct list_head node;
};


struct powercap_zone_ops {
	int (*get_max_energy_range_uj) (struct powercap_zone *, u64 *);
	int (*get_energy_uj) (struct powercap_zone *, u64 *);
	int (*reset_energy_uj) (struct powercap_zone *);
	int (*get_max_power_range_uw) (struct powercap_zone *, u64 *);
	int (*get_power_uw) (struct powercap_zone *, u64 *);
	int (*set_enable) (struct powercap_zone *, bool mode);
	int (*get_enable) (struct powercap_zone *, bool *mode);
	int (*release) (struct powercap_zone *);
};

#define	POWERCAP_ZONE_MAX_ATTRS		6
#define	POWERCAP_CONSTRAINTS_ATTRS	8
#define MAX_CONSTRAINTS_PER_ZONE	10

struct powercap_zone {
	int id;
	char *name;
	void *control_type_inst;
	const struct powercap_zone_ops *ops;
	struct device dev;
	int const_id_cnt;
	struct idr idr;
	struct idr *parent_idr;
	void *private_data;
	struct attribute **zone_dev_attrs;
	int zone_attr_count;
	struct attribute_group dev_zone_attr_group;
	const struct attribute_group *dev_attr_groups[2]; 
	bool allocated;
	struct powercap_zone_constraint *constraints;
};


struct powercap_zone_constraint_ops {
	int (*set_power_limit_uw) (struct powercap_zone *, int, u64);
	int (*get_power_limit_uw) (struct powercap_zone *, int, u64 *);
	int (*set_time_window_us) (struct powercap_zone *, int, u64);
	int (*get_time_window_us) (struct powercap_zone *, int, u64 *);
	int (*get_max_power_uw) (struct powercap_zone *, int, u64 *);
	int (*get_min_power_uw) (struct powercap_zone *, int, u64 *);
	int (*get_max_time_window_us) (struct powercap_zone *, int, u64 *);
	int (*get_min_time_window_us) (struct powercap_zone *, int, u64 *);
	const char *(*get_name) (struct powercap_zone *, int);
};


struct powercap_zone_constraint {
	int id;
	struct powercap_zone *power_zone;
	const struct powercap_zone_constraint_ops *ops;
};



#define POWERCAP_GET_DEV(power_zone)	(&power_zone->dev)


static inline void powercap_set_zone_data(struct powercap_zone *power_zone,
						void *pdata)
{
	if (power_zone)
		power_zone->private_data = pdata;
}


static inline void *powercap_get_zone_data(struct powercap_zone *power_zone)
{
	if (power_zone)
		return power_zone->private_data;
	return NULL;
}


struct powercap_control_type *powercap_register_control_type(
				struct powercap_control_type *control_type,
				const char *name,
				const struct powercap_control_type_ops *ops);


int powercap_unregister_control_type(struct powercap_control_type *instance);




struct powercap_zone *powercap_register_zone(
			struct powercap_zone *power_zone,
			struct powercap_control_type *control_type,
			const char *name,
			struct powercap_zone *parent,
			const struct powercap_zone_ops *ops,
			int nr_constraints,
			const struct powercap_zone_constraint_ops *const_ops);


int powercap_unregister_zone(struct powercap_control_type *control_type,
				struct powercap_zone *power_zone);

#endif
