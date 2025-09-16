

#ifndef _I915_COMPONENT_H_
#define _I915_COMPONENT_H_


#define MAX_PORTS 5


struct i915_audio_component_ops {
	
	struct module *owner;
	
	void (*get_power)(struct device *);
	
	void (*put_power)(struct device *);
	
	void (*codec_wake_override)(struct device *, bool enable);
	
	int (*get_cdclk_freq)(struct device *);
	
	int (*sync_audio_rate)(struct device *, int port, int rate);
	
	int (*get_eld)(struct device *, int port, bool *enabled,
		       unsigned char *buf, int max_bytes);
};


struct i915_audio_component_audio_ops {
	
	void *audio_ptr;
	
	void (*pin_eld_notify)(void *audio_ptr, int port);
};


struct i915_audio_component {
	
	struct device *dev;
	
	int aud_sample_rate[MAX_PORTS];
	
	const struct i915_audio_component_ops *ops;
	
	const struct i915_audio_component_audio_ops *audio_ops;
};

#endif 
