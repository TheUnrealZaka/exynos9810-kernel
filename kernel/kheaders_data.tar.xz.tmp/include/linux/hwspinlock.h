

#ifndef __LINUX_HWSPINLOCK_H
#define __LINUX_HWSPINLOCK_H

#include <linux/err.h>
#include <linux/sched.h>


#define HWLOCK_IRQSTATE	0x01	
#define HWLOCK_IRQ	0x02	

struct device;
struct device_node;
struct hwspinlock;
struct hwspinlock_device;
struct hwspinlock_ops;


struct hwspinlock_pdata {
	int base_id;
};

#if defined(CONFIG_HWSPINLOCK) || defined(CONFIG_HWSPINLOCK_MODULE)

int hwspin_lock_register(struct hwspinlock_device *bank, struct device *dev,
		const struct hwspinlock_ops *ops, int base_id, int num_locks);
int hwspin_lock_unregister(struct hwspinlock_device *bank);
struct hwspinlock *hwspin_lock_request(void);
struct hwspinlock *hwspin_lock_request_specific(unsigned int id);
int hwspin_lock_free(struct hwspinlock *hwlock);
int of_hwspin_lock_get_id(struct device_node *np, int index);
int hwspin_lock_get_id(struct hwspinlock *hwlock);
int __hwspin_lock_timeout(struct hwspinlock *, unsigned int, int,
							unsigned long *);
int __hwspin_trylock(struct hwspinlock *, int, unsigned long *);
void __hwspin_unlock(struct hwspinlock *, int, unsigned long *);

#else 


static inline struct hwspinlock *hwspin_lock_request(void)
{
	return ERR_PTR(-ENODEV);
}

static inline struct hwspinlock *hwspin_lock_request_specific(unsigned int id)
{
	return ERR_PTR(-ENODEV);
}

static inline int hwspin_lock_free(struct hwspinlock *hwlock)
{
	return 0;
}

static inline
int __hwspin_lock_timeout(struct hwspinlock *hwlock, unsigned int to,
					int mode, unsigned long *flags)
{
	return 0;
}

static inline
int __hwspin_trylock(struct hwspinlock *hwlock, int mode, unsigned long *flags)
{
	return 0;
}

static inline
void __hwspin_unlock(struct hwspinlock *hwlock, int mode, unsigned long *flags)
{
}

static inline int of_hwspin_lock_get_id(struct device_node *np, int index)
{
	return 0;
}

static inline int hwspin_lock_get_id(struct hwspinlock *hwlock)
{
	return 0;
}

#endif 


static inline
int hwspin_trylock_irqsave(struct hwspinlock *hwlock, unsigned long *flags)
{
	return __hwspin_trylock(hwlock, HWLOCK_IRQSTATE, flags);
}


static inline int hwspin_trylock_irq(struct hwspinlock *hwlock)
{
	return __hwspin_trylock(hwlock, HWLOCK_IRQ, NULL);
}


static inline int hwspin_trylock(struct hwspinlock *hwlock)
{
	return __hwspin_trylock(hwlock, 0, NULL);
}


static inline int hwspin_lock_timeout_irqsave(struct hwspinlock *hwlock,
				unsigned int to, unsigned long *flags)
{
	return __hwspin_lock_timeout(hwlock, to, HWLOCK_IRQSTATE, flags);
}


static inline
int hwspin_lock_timeout_irq(struct hwspinlock *hwlock, unsigned int to)
{
	return __hwspin_lock_timeout(hwlock, to, HWLOCK_IRQ, NULL);
}


static inline
int hwspin_lock_timeout(struct hwspinlock *hwlock, unsigned int to)
{
	return __hwspin_lock_timeout(hwlock, to, 0, NULL);
}


static inline void hwspin_unlock_irqrestore(struct hwspinlock *hwlock,
							unsigned long *flags)
{
	__hwspin_unlock(hwlock, HWLOCK_IRQSTATE, flags);
}


static inline void hwspin_unlock_irq(struct hwspinlock *hwlock)
{
	__hwspin_unlock(hwlock, HWLOCK_IRQ, NULL);
}


static inline void hwspin_unlock(struct hwspinlock *hwlock)
{
	__hwspin_unlock(hwlock, 0, NULL);
}

#endif 
