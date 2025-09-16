

#ifndef __LINUX_DEVFREQ_H__
#define __LINUX_DEVFREQ_H__

#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/pm_opp.h>
#include <linux/kthread.h>
#include <linux/timer.h>

#define DEVFREQ_NAME_LEN 16


#define DEVFREQ_TRANSITION_NOTIFIER	(0)


#define	DEVFREQ_PRECHANGE		(0)
#define DEVFREQ_POSTCHANGE		(1)

struct devfreq;


struct devfreq_dev_status {
	
	unsigned long long total_time;
	unsigned long long busy_time;
	unsigned long long delta_time;
	unsigned long current_frequency;
	void *private_data;
};


#define DEVFREQ_FLAG_LEAST_UPPER_BOUND		0x1


struct devfreq_dev_profile {
	unsigned long initial_freq;
	unsigned long suspend_freq;
	unsigned int polling_ms;

	int (*target)(struct device *dev, unsigned long *freq, u32 flags);
	int (*get_dev_status)(struct device *dev,
			      struct devfreq_dev_status *stat);
	int (*get_cur_freq)(struct device *dev, unsigned long *freq);
	void (*exit)(struct device *dev);

	unsigned long *freq_table;
	unsigned int max_state;
};


struct devfreq_governor {
	struct list_head node;

	const char name[DEVFREQ_NAME_LEN];
	const unsigned int immutable;
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq);
	int (*event_handler)(struct devfreq *devfreq,
				unsigned int event, void *data);
};


struct devfreq {
	struct list_head node;

	struct mutex lock;
	struct device dev;
	struct devfreq_dev_profile *profile;
	const struct devfreq_governor *governor;
	char governor_name[DEVFREQ_NAME_LEN];
	struct notifier_block nb;
	struct delayed_work work;

	unsigned long previous_freq;
	struct devfreq_dev_status last_status;

	void *data; 

	unsigned long min_freq;
	unsigned long max_freq;
	unsigned long str_freq;
	bool stop_polling;

	
	unsigned int total_trans;
	unsigned int *trans_table;
	unsigned long *time_in_state;
	unsigned long last_stat_updated;

	struct srcu_notifier_head transition_notifier_list;

	bool disabled_pm_qos;
};

struct devfreq_freqs {
	unsigned long old;
	unsigned long new;
};

#if defined(CONFIG_PM_DEVFREQ)
extern struct devfreq *devfreq_add_device(struct device *dev,
				  struct devfreq_dev_profile *profile,
				  const char *governor_name,
				  void *data);
extern int devfreq_remove_device(struct devfreq *devfreq);
extern struct devfreq *devm_devfreq_add_device(struct device *dev,
				  struct devfreq_dev_profile *profile,
				  const char *governor_name,
				  void *data);
extern void devm_devfreq_remove_device(struct device *dev,
				  struct devfreq *devfreq);


extern int devfreq_suspend_device(struct devfreq *devfreq);
extern int devfreq_resume_device(struct devfreq *devfreq);


extern struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags);
extern int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq);
extern int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq);
extern int devm_devfreq_register_opp_notifier(struct device *dev,
					      struct devfreq *devfreq);
extern void devm_devfreq_unregister_opp_notifier(struct device *dev,
						struct devfreq *devfreq);
extern int devfreq_register_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list);
extern int devfreq_unregister_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list);
extern int devm_devfreq_register_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
extern void devm_devfreq_unregister_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
extern struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
						int index);


static inline int devfreq_update_stats(struct devfreq *df)
{
	return df->profile->get_dev_status(df->dev.parent, &df->last_status);
}
#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND) || IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_USAGE)\
	|| IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_INTERACTIVE)
struct devfreq_notifier_block {
	struct notifier_block nb;
	struct devfreq *df;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND)

struct devfreq_simple_ondemand_data {
	unsigned int multiplication_weight;
	unsigned int upthreshold;
	unsigned int downdifferential;
	unsigned long cal_qos_max;
	int pm_qos_class;
	struct devfreq_notifier_block nb;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_USAGE)
struct devfreq_simple_usage_data {
	unsigned int multiplication_weight;
	unsigned int proportional;
	unsigned int upthreshold;
	unsigned int target_percentage;
	int pm_qos_class;
	unsigned long cal_qos_max;
	bool en_monitoring;
	struct devfreq_notifier_block nb;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_EXYNOS)
struct devfreq_simple_exynos_data {
	unsigned int urgentthreshold;
	unsigned int upthreshold;
	unsigned int downthreshold;
	unsigned int idlethreshold;
	unsigned long above_freq;
	unsigned long below_freq;
	int pm_qos_class;
	int pm_qos_class_max;
	unsigned long cal_qos_max;
	bool en_monitoring;
	struct devfreq_notifier_block nb;
	struct devfreq_notifier_block nb_max;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_SIMPLE_INTERACTIVE)
#define DEFAULT_DELAY_TIME		10 
#define DEFAULT_NDELAY_TIME		1
#define DELAY_TIME_RANGE		10
#define BOUND_CPU_NUM			0

#ifdef CONFIG_EXYNOS_WD_DVFS
#define SIMPLE_LOAD_MAX				10
struct devfreq_simple_load {
	unsigned long long delta;
	unsigned int load;
};
#endif
struct devfreq_simple_interactive_data {
#ifdef CONFIG_EXYNOS_WD_DVFS
	struct devfreq_simple_load buffer[SIMPLE_LOAD_MAX];
	struct devfreq_simple_load *front;
	struct devfreq_simple_load *rear;
	unsigned long long busy;
	unsigned long long total;
	unsigned int min_load;
	unsigned int max_load;
	unsigned long long max_spent;
	
#define INTERACTIVE_MIN_SAMPLE_TIME	15
	unsigned int min_sample_time;
#define INTERACTIVE_HOLD_SAMPLE_TIME	100
	unsigned int hold_sample_time;
#define INTERACTIVE_TARGET_LOAD		75
#define INTERACTIVE_NTARGET_LOAD	1
	unsigned int *target_load;
	unsigned int ntarget_load;
#define INTERACTIVE_GO_HISPEED_LOAD	99
	unsigned int go_hispeed_load;
#define INTERACTIVE_HISPEED_FREQ	1000000
	unsigned int hispeed_freq;
#define INTERACTIVE_TOLERANCE		1
	unsigned int tolerance;
	
#endif
	bool use_delay_time;
	int *delay_time;
	int ndelay_time;
	unsigned long prev_freq;
	u64 changed_time;
	struct timer_list freq_timer;
	struct timer_list freq_slack_timer;
	struct task_struct *change_freq_task;
	int pm_qos_class;
	int pm_qos_class_max;
	struct devfreq_notifier_block nb;
	struct devfreq_notifier_block nb_max;
};
#endif

#if IS_ENABLED(CONFIG_DEVFREQ_GOV_PASSIVE)

struct devfreq_passive_data {
	
	struct devfreq *parent;

	
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq);

	
	struct devfreq *this;
	struct notifier_block nb;
};
#endif

#else 
static inline struct devfreq *devfreq_add_device(struct device *dev,
					  struct devfreq_dev_profile *profile,
					  const char *governor_name,
					  void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline int devfreq_remove_device(struct devfreq *devfreq)
{
	return 0;
}

static inline struct devfreq *devm_devfreq_add_device(struct device *dev,
					struct devfreq_dev_profile *profile,
					const char *governor_name,
					void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline void devm_devfreq_remove_device(struct device *dev,
					struct devfreq *devfreq)
{
}

static inline int devfreq_suspend_device(struct devfreq *devfreq)
{
	return 0;
}

static inline int devfreq_resume_device(struct devfreq *devfreq)
{
	return 0;
}

static inline struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags)
{
	return ERR_PTR(-EINVAL);
}

static inline int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline int devm_devfreq_register_opp_notifier(struct device *dev,
						     struct devfreq *devfreq)
{
	return -EINVAL;
}

static inline void devm_devfreq_unregister_opp_notifier(struct device *dev,
							struct devfreq *devfreq)
{
}

static inline int devfreq_register_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devfreq_unregister_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devm_devfreq_register_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list)
{
	return 0;
}

static inline void devm_devfreq_unregister_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list)
{
}

static inline struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
							int index)
{
	return ERR_PTR(-ENODEV);
}

static inline int devfreq_update_stats(struct devfreq *df)
{
	return -EINVAL;
}
#endif 

#endif 
