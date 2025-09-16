

#ifndef LINUX_HWRANDOM_H_
#define LINUX_HWRANDOM_H_

#include <linux/completion.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/kref.h>


struct hwrng {
	const char *name;
	int (*init)(struct hwrng *rng);
	void (*cleanup)(struct hwrng *rng);
	int (*data_present)(struct hwrng *rng, int wait);
	int (*data_read)(struct hwrng *rng, u32 *data);
	int (*read)(struct hwrng *rng, void *data, size_t max, bool wait);
	unsigned long priv;
	unsigned short quality;

	
	struct list_head list;
	struct kref ref;
	struct completion cleanup_done;
};

struct device;


extern int hwrng_register(struct hwrng *rng);
extern int devm_hwrng_register(struct device *dev, struct hwrng *rng);

extern void hwrng_unregister(struct hwrng *rng);
extern void devm_hwrng_unregister(struct device *dve, struct hwrng *rng);

#endif 
