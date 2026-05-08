#ifndef __SOUND_MIXER_OSS_H
#define __SOUND_MIXER_OSS_H



#if defined(CONFIG_SND_MIXER_OSS) || defined(CONFIG_SND_MIXER_OSS_MODULE)

#define SNDRV_OSS_MAX_MIXERS	32

struct snd_mixer_oss_file;

struct snd_mixer_oss_slot {
	int number;
	unsigned int stereo: 1;
	int (*get_volume)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int *left, int *right);
	int (*put_volume)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int left, int right);
	int (*get_recsrc)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int *active);
	int (*put_recsrc)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int active);
	unsigned long private_value;
	void *private_data;
	void (*private_free)(struct snd_mixer_oss_slot *slot);
	int volume[2];
};

struct snd_mixer_oss {
	struct snd_card *card;
	char id[16];
	char name[32];
	struct snd_mixer_oss_slot slots[SNDRV_OSS_MAX_MIXERS]; 
	unsigned int mask_recsrc;		
	int (*get_recsrc)(struct snd_mixer_oss_file *fmixer,
			  unsigned int *active_index);
	int (*put_recsrc)(struct snd_mixer_oss_file *fmixer,
			  unsigned int active_index);
	void *private_data_recsrc;
	void (*private_free_recsrc)(struct snd_mixer_oss *mixer);
	struct mutex reg_mutex;
	struct snd_info_entry *proc_entry;
	int oss_dev_alloc;
	
	int oss_recsrc;
};

struct snd_mixer_oss_file {
	struct snd_card *card;
	struct snd_mixer_oss *mixer;
};

int snd_mixer_oss_ioctl_card(struct snd_card *card,
			     unsigned int cmd, unsigned long arg);

#endif 

#endif 
