

#ifndef SEC_ABC_HUB_H
#define SEC_ABC_HUB_H

#include <linux/sti/abc_common.h>






#ifdef CONFIG_SEC_ABC_HUB_COND
#define DET_CONN_MAX_NUM_GPIOS 32

struct sub_cond_pdata {
	// common
	int init;
	int enabled;
	// custom
	const char *name[DET_CONN_MAX_NUM_GPIOS];
	int irq_gpio[DET_CONN_MAX_NUM_GPIOS];
	int irq_number[DET_CONN_MAX_NUM_GPIOS];
	unsigned int irq_type[DET_CONN_MAX_NUM_GPIOS];
	int gpio_cnt;
	struct mutex cond_lock;
};
#endif


#ifdef CONFIG_SEC_ABC_HUB_BOOTC
#define BOOTC_OFFSET_DATA_CNT 1
#define BOOTC_OFFSET_STR_MAX 100

struct abc_hub_bootc_offset_data {
	char module[BOOTC_OFFSET_STR_MAX];
	int offset;
};

struct sub_bootc_pdata {
	// common
	int init;
	int enabled;
	// custom
	int time_spec;
	int time_spec_offset;
	struct abc_hub_bootc_offset_data offset_data[BOOTC_OFFSET_DATA_CNT];
	struct workqueue_struct *workqueue;
	struct delayed_work bootc_work;
};
#endif




enum {
	ABC_HUB_DISABLED,
	ABC_HUB_ENABLED,
};
struct abc_hub_platform_data {
	unsigned int nSub;
#ifdef CONFIG_SEC_ABC_HUB_COND
	struct sub_cond_pdata cond_pdata;
#endif
#ifdef CONFIG_SEC_ABC_HUB_BOOTC
	struct sub_bootc_pdata bootc_pdata;
#endif
};

struct abc_hub_info {
	struct device *dev;
	struct abc_hub_platform_data *pdata;
	int enabled;
};






#ifdef CONFIG_SEC_ABC_HUB_COND
int parse_cond_data(struct device *dev,
			  struct abc_hub_platform_data *pdata,
			  struct device_node *np);
int abc_hub_cond_init(struct device *dev);
void abc_hub_cond_enable(struct device *dev, int enable);

int abc_hub_cond_suspend(struct device *dev);
int abc_hub_cond_resume(struct device *dev);
#endif


#ifdef CONFIG_SEC_ABC_HUB_BOOTC
int parse_bootc_data(struct device *dev,
			  struct abc_hub_platform_data *pdata,
			  struct device_node *np);
int abc_hub_bootc_init(struct device *dev);
void abc_hub_bootc_enable(struct device *dev, int enable);
#endif


void abc_hub_send_event(char *str);
int abc_hub_get_enabled(void);

#endif
