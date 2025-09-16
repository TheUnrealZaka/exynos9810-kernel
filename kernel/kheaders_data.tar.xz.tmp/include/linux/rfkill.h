
#ifndef __RFKILL_H
#define __RFKILL_H

#include <uapi/linux/rfkill.h>


enum rfkill_user_states {
	RFKILL_USER_STATE_SOFT_BLOCKED	= RFKILL_STATE_SOFT_BLOCKED,
	RFKILL_USER_STATE_UNBLOCKED	= RFKILL_STATE_UNBLOCKED,
	RFKILL_USER_STATE_HARD_BLOCKED	= RFKILL_STATE_HARD_BLOCKED,
};
#undef RFKILL_STATE_SOFT_BLOCKED
#undef RFKILL_STATE_UNBLOCKED
#undef RFKILL_STATE_HARD_BLOCKED

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/leds.h>
#include <linux/err.h>

struct device;

struct rfkill;


struct rfkill_ops {
	void	(*poll)(struct rfkill *rfkill, void *data);
	void	(*query)(struct rfkill *rfkill, void *data);
	int	(*set_block)(void *data, bool blocked);
};

#if defined(CONFIG_RFKILL) || defined(CONFIG_RFKILL_MODULE)

struct rfkill * __must_check rfkill_alloc(const char *name,
					  struct device *parent,
					  const enum rfkill_type type,
					  const struct rfkill_ops *ops,
					  void *ops_data);


int __must_check rfkill_register(struct rfkill *rfkill);


void rfkill_pause_polling(struct rfkill *rfkill);


void rfkill_resume_polling(struct rfkill *rfkill);



void rfkill_unregister(struct rfkill *rfkill);


void rfkill_destroy(struct rfkill *rfkill);


bool rfkill_set_hw_state(struct rfkill *rfkill, bool blocked);


bool rfkill_set_sw_state(struct rfkill *rfkill, bool blocked);


void rfkill_init_sw_state(struct rfkill *rfkill, bool blocked);


void rfkill_set_states(struct rfkill *rfkill, bool sw, bool hw);


bool rfkill_blocked(struct rfkill *rfkill);


enum rfkill_type rfkill_find_type(const char *name);

#else 
static inline struct rfkill * __must_check
rfkill_alloc(const char *name,
	     struct device *parent,
	     const enum rfkill_type type,
	     const struct rfkill_ops *ops,
	     void *ops_data)
{
	return ERR_PTR(-ENODEV);
}

static inline int __must_check rfkill_register(struct rfkill *rfkill)
{
	if (rfkill == ERR_PTR(-ENODEV))
		return 0;
	return -EINVAL;
}

static inline void rfkill_pause_polling(struct rfkill *rfkill)
{
}

static inline void rfkill_resume_polling(struct rfkill *rfkill)
{
}

static inline void rfkill_unregister(struct rfkill *rfkill)
{
}

static inline void rfkill_destroy(struct rfkill *rfkill)
{
}

static inline bool rfkill_set_hw_state(struct rfkill *rfkill, bool blocked)
{
	return blocked;
}

static inline bool rfkill_set_sw_state(struct rfkill *rfkill, bool blocked)
{
	return blocked;
}

static inline void rfkill_init_sw_state(struct rfkill *rfkill, bool blocked)
{
}

static inline void rfkill_set_states(struct rfkill *rfkill, bool sw, bool hw)
{
}

static inline bool rfkill_blocked(struct rfkill *rfkill)
{
	return false;
}

static inline enum rfkill_type rfkill_find_type(const char *name)
{
	return RFKILL_TYPE_ALL;
}

#endif 


#ifdef CONFIG_RFKILL_LEDS

const char *rfkill_get_led_trigger_name(struct rfkill *rfkill);


void rfkill_set_led_trigger_name(struct rfkill *rfkill, const char *name);
#else
static inline const char *rfkill_get_led_trigger_name(struct rfkill *rfkill)
{
	return NULL;
}

static inline void
rfkill_set_led_trigger_name(struct rfkill *rfkill, const char *name)
{
}
#endif

#endif 
