

#ifndef __LINUX_SND_SOC_TPLG_H
#define __LINUX_SND_SOC_TPLG_H

#include <sound/asoc.h>
#include <linux/list.h>

struct firmware;
struct snd_kcontrol;
struct snd_soc_tplg_pcm_be;
struct snd_ctl_elem_value;
struct snd_ctl_elem_info;
struct snd_soc_dapm_widget;
struct snd_soc_component;
struct snd_soc_tplg_pcm_fe;
struct snd_soc_dapm_context;
struct snd_soc_card;


#define SND_SOC_TPLG_INDEX_ALL	0	


enum snd_soc_dobj_type {
	SND_SOC_DOBJ_NONE		= 0,	
	SND_SOC_DOBJ_MIXER,
	SND_SOC_DOBJ_ENUM,
	SND_SOC_DOBJ_BYTES,
	SND_SOC_DOBJ_PCM,
	SND_SOC_DOBJ_DAI_LINK,
	SND_SOC_DOBJ_CODEC_LINK,
	SND_SOC_DOBJ_WIDGET,
};


struct snd_soc_dobj_control {
	struct snd_kcontrol *kcontrol;
	char **dtexts;
	unsigned long *dvalues;
};


struct snd_soc_dobj_widget {
	unsigned int kcontrol_enum:1;	
};


struct snd_soc_dobj {
	enum snd_soc_dobj_type type;
	unsigned int index;	
	struct list_head list;
	struct snd_soc_tplg_ops *ops;
	union {
		struct snd_soc_dobj_control control;
		struct snd_soc_dobj_widget widget;
	};
	void *private; 
};


struct snd_soc_tplg_kcontrol_ops {
	u32 id;
	int (*get)(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol);
	int (*put)(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol);
	int (*info)(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_info *uinfo);
};


struct snd_soc_tplg_bytes_ext_ops {
	u32 id;
	int (*get)(struct snd_kcontrol *kcontrol, unsigned int __user *bytes,
							unsigned int size);
	int (*put)(struct snd_kcontrol *kcontrol,
			const unsigned int __user *bytes, unsigned int size);
};


struct snd_soc_tplg_widget_events {
	u16 type;
	int (*event_handler)(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *k, int event);
};


struct snd_soc_tplg_ops {

	
	int (*control_load)(struct snd_soc_component *,
		struct snd_kcontrol_new *, struct snd_soc_tplg_ctl_hdr *);
	int (*control_unload)(struct snd_soc_component *,
		struct snd_soc_dobj *);

	
	int (*widget_load)(struct snd_soc_component *,
		struct snd_soc_dapm_widget *,
		struct snd_soc_tplg_dapm_widget *);
	int (*widget_unload)(struct snd_soc_component *,
		struct snd_soc_dobj *);

	
	int (*dai_load)(struct snd_soc_component *,
		struct snd_soc_dai_driver *dai_drv);
	int (*dai_unload)(struct snd_soc_component *,
		struct snd_soc_dobj *);

	
	int (*link_load)(struct snd_soc_component *,
		struct snd_soc_dai_link *link);
	int (*link_unload)(struct snd_soc_component *,
		struct snd_soc_dobj *);

	
	int (*vendor_load)(struct snd_soc_component *,
		struct snd_soc_tplg_hdr *);
	int (*vendor_unload)(struct snd_soc_component *,
		struct snd_soc_tplg_hdr *);

	
	void (*complete)(struct snd_soc_component *);

	
	int (*manifest)(struct snd_soc_component *,
		struct snd_soc_tplg_manifest *);

	
	const struct snd_soc_tplg_kcontrol_ops *io_ops;
	int io_ops_count;

	
	const struct snd_soc_tplg_bytes_ext_ops *bytes_ext_ops;
	int bytes_ext_ops_count;
};

#ifdef CONFIG_SND_SOC_TOPOLOGY


static inline const void *snd_soc_tplg_get_data(struct snd_soc_tplg_hdr *hdr)
{
	const void *ptr = hdr;

	return ptr + sizeof(*hdr);
}


int snd_soc_tplg_component_load(struct snd_soc_component *comp,
	struct snd_soc_tplg_ops *ops, const struct firmware *fw,
	u32 index);
int snd_soc_tplg_component_remove(struct snd_soc_component *comp, u32 index);


void snd_soc_tplg_widget_remove(struct snd_soc_dapm_widget *w);
void snd_soc_tplg_widget_remove_all(struct snd_soc_dapm_context *dapm,
	u32 index);


int snd_soc_tplg_widget_bind_event(struct snd_soc_dapm_widget *w,
	const struct snd_soc_tplg_widget_events *events, int num_events,
	u16 event_type);

#else

static inline int snd_soc_tplg_component_remove(struct snd_soc_component *comp,
						u32 index)
{
	return 0;
}

#endif

#endif
