
#ifndef _INPUT_H
#define _INPUT_H

#include <linux/time.h>
#include <linux/list.h>
#include <uapi/linux/input.h>

#define ABS_MT_FIRST		ABS_MT_TOUCH_MAJOR
#define ABS_MT_LAST		ABS_MT_GRIP



#include <linux/device.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/mod_devicetable.h>


struct input_value {
	__u16 type;
	__u16 code;
	__s32 value;
};


struct input_dev {
	const char *name;
	const char *phys;
	const char *uniq;
	struct input_id id;

	unsigned long propbit[BITS_TO_LONGS(INPUT_PROP_CNT)];

	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];

	unsigned int hint_events_per_packet;

	unsigned int keycodemax;
	unsigned int keycodesize;
	void *keycode;

	int (*setkeycode)(struct input_dev *dev,
			  const struct input_keymap_entry *ke,
			  unsigned int *old_keycode);
	int (*getkeycode)(struct input_dev *dev,
			  struct input_keymap_entry *ke);

	struct ff_device *ff;

	unsigned int repeat_key;
	struct timer_list timer;

	int rep[REP_CNT];

	struct input_mt *mt;

	struct input_absinfo *absinfo;

	unsigned long key[BITS_TO_LONGS(KEY_CNT)];
	unsigned long led[BITS_TO_LONGS(LED_CNT)];
	unsigned long snd[BITS_TO_LONGS(SND_CNT)];
	unsigned long sw[BITS_TO_LONGS(SW_CNT)];

	int (*open)(struct input_dev *dev);
	void (*close)(struct input_dev *dev);
	int (*flush)(struct input_dev *dev, struct file *file);
	int (*event)(struct input_dev *dev, unsigned int type, unsigned int code, int value);

	struct input_handle __rcu *grab;

	spinlock_t event_lock;
	struct mutex mutex;

	unsigned int users;
	unsigned int users_private;
	bool going_away;
	bool disabled;

	struct device dev;

	struct list_head	h_list;
	struct list_head	node;

	unsigned int num_vals;
	unsigned int max_vals;
	struct input_value *vals;

	bool devres_managed;
};
#define to_input_dev(d) container_of(d, struct input_dev, dev)



#if EV_MAX != INPUT_DEVICE_ID_EV_MAX
#error "EV_MAX and INPUT_DEVICE_ID_EV_MAX do not match"
#endif

#if KEY_MIN_INTERESTING != INPUT_DEVICE_ID_KEY_MIN_INTERESTING
#error "KEY_MIN_INTERESTING and INPUT_DEVICE_ID_KEY_MIN_INTERESTING do not match"
#endif

#if KEY_MAX != INPUT_DEVICE_ID_KEY_MAX
#error "KEY_MAX and INPUT_DEVICE_ID_KEY_MAX do not match"
#endif

#if REL_MAX != INPUT_DEVICE_ID_REL_MAX
#error "REL_MAX and INPUT_DEVICE_ID_REL_MAX do not match"
#endif

#if ABS_MAX != INPUT_DEVICE_ID_ABS_MAX
#error "ABS_MAX and INPUT_DEVICE_ID_ABS_MAX do not match"
#endif

#if MSC_MAX != INPUT_DEVICE_ID_MSC_MAX
#error "MSC_MAX and INPUT_DEVICE_ID_MSC_MAX do not match"
#endif

#if LED_MAX != INPUT_DEVICE_ID_LED_MAX
#error "LED_MAX and INPUT_DEVICE_ID_LED_MAX do not match"
#endif

#if SND_MAX != INPUT_DEVICE_ID_SND_MAX
#error "SND_MAX and INPUT_DEVICE_ID_SND_MAX do not match"
#endif

#if FF_MAX != INPUT_DEVICE_ID_FF_MAX
#error "FF_MAX and INPUT_DEVICE_ID_FF_MAX do not match"
#endif

#if SW_MAX != INPUT_DEVICE_ID_SW_MAX
#error "SW_MAX and INPUT_DEVICE_ID_SW_MAX do not match"
#endif

#define INPUT_DEVICE_ID_MATCH_DEVICE \
	(INPUT_DEVICE_ID_MATCH_BUS | INPUT_DEVICE_ID_MATCH_VENDOR | INPUT_DEVICE_ID_MATCH_PRODUCT)
#define INPUT_DEVICE_ID_MATCH_DEVICE_AND_VERSION \
	(INPUT_DEVICE_ID_MATCH_DEVICE | INPUT_DEVICE_ID_MATCH_VERSION)

struct input_handle;


struct input_handler {

	void *private;

	void (*event)(struct input_handle *handle, unsigned int type, unsigned int code, int value);
	void (*events)(struct input_handle *handle,
		       const struct input_value *vals, unsigned int count);
	bool (*filter)(struct input_handle *handle, unsigned int type, unsigned int code, int value);
	bool (*match)(struct input_handler *handler, struct input_dev *dev);
	int (*connect)(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id);
	void (*disconnect)(struct input_handle *handle);
	void (*start)(struct input_handle *handle);

	bool legacy_minors;
	int minor;
	const char *name;

	const struct input_device_id *id_table;

	struct list_head	h_list;
	struct list_head	node;
};


struct input_handle {

	void *private;

	int open;
	const char *name;

	struct input_dev *dev;
	struct input_handler *handler;

	struct list_head	d_node;
	struct list_head	h_node;
};

struct input_dev __must_check *input_allocate_device(void);
struct input_dev __must_check *devm_input_allocate_device(struct device *);
void input_free_device(struct input_dev *dev);

static inline struct input_dev *input_get_device(struct input_dev *dev)
{
	return dev ? to_input_dev(get_device(&dev->dev)) : NULL;
}

static inline void input_put_device(struct input_dev *dev)
{
	if (dev)
		put_device(&dev->dev);
}

static inline void *input_get_drvdata(struct input_dev *dev)
{
	return dev_get_drvdata(&dev->dev);
}

static inline void input_set_drvdata(struct input_dev *dev, void *data)
{
	dev_set_drvdata(&dev->dev, data);
}

int __must_check input_register_device(struct input_dev *);
void input_unregister_device(struct input_dev *);

void input_reset_device(struct input_dev *);

int __must_check input_register_handler(struct input_handler *);
void input_unregister_handler(struct input_handler *);

int __must_check input_get_new_minor(int legacy_base, unsigned int legacy_num,
				     bool allow_dynamic);
void input_free_minor(unsigned int minor);

int input_handler_for_each_handle(struct input_handler *, void *data,
				  int (*fn)(struct input_handle *, void *));

int input_register_handle(struct input_handle *);
void input_unregister_handle(struct input_handle *);

int input_grab_device(struct input_handle *);
void input_release_device(struct input_handle *);

int input_open_device(struct input_handle *);
void input_close_device(struct input_handle *);

int input_flush_device(struct input_handle *handle, struct file *file);

void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value);
void input_inject_event(struct input_handle *handle, unsigned int type, unsigned int code, int value);

static inline void input_report_key(struct input_dev *dev, unsigned int code, int value)
{
	input_event(dev, EV_KEY, code, !!value);
}

static inline void input_report_rel(struct input_dev *dev, unsigned int code, int value)
{
	input_event(dev, EV_REL, code, value);
}

static inline void input_report_abs(struct input_dev *dev, unsigned int code, int value)
{
	input_event(dev, EV_ABS, code, value);
}

static inline void input_report_ff_status(struct input_dev *dev, unsigned int code, int value)
{
	input_event(dev, EV_FF_STATUS, code, value);
}

static inline void input_report_switch(struct input_dev *dev, unsigned int code, int value)
{
	input_event(dev, EV_SW, code, !!value);
}

static inline void input_sync(struct input_dev *dev)
{
	input_event(dev, EV_SYN, SYN_REPORT, 0);
}

static inline void input_mt_sync(struct input_dev *dev)
{
	input_event(dev, EV_SYN, SYN_MT_REPORT, 0);
}

void input_set_capability(struct input_dev *dev, unsigned int type, unsigned int code);


static inline void input_set_events_per_packet(struct input_dev *dev, int n_events)
{
	dev->hint_events_per_packet = n_events;
}

void input_alloc_absinfo(struct input_dev *dev);
void input_set_abs_params(struct input_dev *dev, unsigned int axis,
			  int min, int max, int fuzz, int flat);

#define INPUT_GENERATE_ABS_ACCESSORS(_suffix, _item)			\
static inline int input_abs_get_##_suffix(struct input_dev *dev,	\
					  unsigned int axis)		\
{									\
	return dev->absinfo ? dev->absinfo[axis]._item : 0;		\
}									\
									\
static inline void input_abs_set_##_suffix(struct input_dev *dev,	\
					   unsigned int axis, int val)	\
{									\
	input_alloc_absinfo(dev);					\
	if (dev->absinfo)						\
		dev->absinfo[axis]._item = val;				\
}

INPUT_GENERATE_ABS_ACCESSORS(val, value)
INPUT_GENERATE_ABS_ACCESSORS(min, minimum)
INPUT_GENERATE_ABS_ACCESSORS(max, maximum)
INPUT_GENERATE_ABS_ACCESSORS(fuzz, fuzz)
INPUT_GENERATE_ABS_ACCESSORS(flat, flat)
INPUT_GENERATE_ABS_ACCESSORS(res, resolution)

int input_scancode_to_scalar(const struct input_keymap_entry *ke,
			     unsigned int *scancode);

int input_get_keycode(struct input_dev *dev, struct input_keymap_entry *ke);
int input_set_keycode(struct input_dev *dev,
		      const struct input_keymap_entry *ke);

void input_enable_softrepeat(struct input_dev *dev, int delay, int period);

extern struct class input_class;


struct ff_device {
	int (*upload)(struct input_dev *dev, struct ff_effect *effect,
		      struct ff_effect *old);
	int (*erase)(struct input_dev *dev, int effect_id);

	int (*playback)(struct input_dev *dev, int effect_id, int value);
	void (*set_gain)(struct input_dev *dev, u16 gain);
	void (*set_autocenter)(struct input_dev *dev, u16 magnitude);

	void (*destroy)(struct ff_device *);

	void *private;

	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];

	struct mutex mutex;

	int max_effects;
	struct ff_effect *effects;
	struct file *effect_owners[];
};

int input_ff_create(struct input_dev *dev, unsigned int max_effects);
void input_ff_destroy(struct input_dev *dev);

int input_ff_event(struct input_dev *dev, unsigned int type, unsigned int code, int value);

int input_ff_upload(struct input_dev *dev, struct ff_effect *effect, struct file *file);
int input_ff_erase(struct input_dev *dev, int effect_id, struct file *file);
int input_ff_flush(struct input_dev *dev, struct file *file);

int input_ff_create_memless(struct input_dev *dev, void *data,
		int (*play_effect)(struct input_dev *, void *, struct ff_effect *));

#endif
