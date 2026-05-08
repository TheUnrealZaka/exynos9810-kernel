#ifndef __SOUND_SEQ_DEVICE_H
#define __SOUND_SEQ_DEVICE_H





struct snd_seq_device {
	
	struct snd_card *card;	
	int device;		
	const char *id;		
	char name[80];		
	int argsize;		
	void *driver_data;	
	void *private_data;	
	void (*private_free)(struct snd_seq_device *device);
	struct device dev;
};

#define to_seq_dev(_dev) \
	container_of(_dev, struct snd_seq_device, dev)




struct snd_seq_driver {
	struct device_driver driver;
	char *id;
	int argsize;
};

#define to_seq_drv(_drv) \
	container_of(_drv, struct snd_seq_driver, driver)


#ifdef CONFIG_MODULES
void snd_seq_device_load_drivers(void);
#else
#define snd_seq_device_load_drivers()
#endif
int snd_seq_device_new(struct snd_card *card, int device, const char *id,
		       int argsize, struct snd_seq_device **result);

#define SNDRV_SEQ_DEVICE_ARGPTR(dev) (void *)((char *)(dev) + sizeof(struct snd_seq_device))

int __must_check __snd_seq_driver_register(struct snd_seq_driver *drv,
					   struct module *mod);
#define snd_seq_driver_register(drv) \
	__snd_seq_driver_register(drv, THIS_MODULE)
void snd_seq_driver_unregister(struct snd_seq_driver *drv);

#define module_snd_seq_driver(drv) \
	module_driver(drv, snd_seq_driver_register, snd_seq_driver_unregister)


#define SNDRV_SEQ_DEV_ID_MIDISYNTH	"seq-midi"
#define SNDRV_SEQ_DEV_ID_OPL3		"opl3-synth"

#endif 
