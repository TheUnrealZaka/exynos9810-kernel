

#ifndef __LINUX_SND_SOC_DPCM_H
#define __LINUX_SND_SOC_DPCM_H

#include <linux/slab.h>
#include <linux/list.h>
#include <sound/pcm.h>

struct snd_soc_pcm_runtime;


enum snd_soc_dpcm_update {
	SND_SOC_DPCM_UPDATE_NO	= 0,
	SND_SOC_DPCM_UPDATE_BE,
	SND_SOC_DPCM_UPDATE_FE,
};


enum snd_soc_dpcm_link_state {
	SND_SOC_DPCM_LINK_STATE_NEW	= 0,	
	SND_SOC_DPCM_LINK_STATE_FREE,		
};


enum snd_soc_dpcm_state {
	SND_SOC_DPCM_STATE_NEW	= 0,
	SND_SOC_DPCM_STATE_OPEN,
	SND_SOC_DPCM_STATE_HW_PARAMS,
	SND_SOC_DPCM_STATE_PREPARE,
	SND_SOC_DPCM_STATE_START,
	SND_SOC_DPCM_STATE_STOP,
	SND_SOC_DPCM_STATE_PAUSED,
	SND_SOC_DPCM_STATE_SUSPEND,
	SND_SOC_DPCM_STATE_HW_FREE,
	SND_SOC_DPCM_STATE_CLOSE,
};


enum snd_soc_dpcm_trigger {
	SND_SOC_DPCM_TRIGGER_PRE		= 0,
	SND_SOC_DPCM_TRIGGER_POST,
	SND_SOC_DPCM_TRIGGER_BESPOKE,
	SND_SOC_DPCM_TRIGGER_PRE_POST, 
	SND_SOC_DPCM_TRIGGER_POST_PRE, 
};


struct snd_soc_dpcm {
	
	struct snd_soc_pcm_runtime *be;
	struct snd_soc_pcm_runtime *fe;

	
	enum snd_soc_dpcm_link_state state;

	
	struct list_head list_be;
	struct list_head list_fe;

	
	struct snd_pcm_hw_params hw_params;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_state;
#endif
};


struct snd_soc_dpcm_runtime {
	struct list_head be_clients;
	struct list_head fe_clients;

	int users;
	struct snd_pcm_runtime *runtime;
	struct snd_pcm_hw_params hw_params;

	
	enum snd_soc_dpcm_update runtime_update;
	enum snd_soc_dpcm_state state;

	int trigger_pending; 
};


int snd_soc_dpcm_can_be_free_stop(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream);


int snd_soc_dpcm_can_be_params(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream);


int snd_soc_dpcm_fe_can_update(struct snd_soc_pcm_runtime *fe, int stream);


int snd_soc_dpcm_be_can_update(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream);


struct snd_pcm_substream *
	snd_soc_dpcm_get_substream(struct snd_soc_pcm_runtime *be, int stream);


enum snd_soc_dpcm_state
	snd_soc_dpcm_be_get_state(struct snd_soc_pcm_runtime *be, int stream);


void snd_soc_dpcm_be_set_state(struct snd_soc_pcm_runtime *be, int stream,
	enum snd_soc_dpcm_state state);


int soc_dpcm_be_digital_mute(struct snd_soc_pcm_runtime *fe, int mute);
void soc_dpcm_debugfs_add(struct snd_soc_pcm_runtime *rtd);
int soc_dpcm_runtime_update(struct snd_soc_card *);

int dpcm_path_get(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list_);
int dpcm_process_paths(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list, int new);
int dpcm_be_dai_startup(struct snd_soc_pcm_runtime *fe, int stream);
int dpcm_be_dai_shutdown(struct snd_soc_pcm_runtime *fe, int stream);
void dpcm_be_disconnect(struct snd_soc_pcm_runtime *fe, int stream);
void dpcm_clear_pending_state(struct snd_soc_pcm_runtime *fe, int stream);
int dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream);
int dpcm_be_dai_hw_params(struct snd_soc_pcm_runtime *fe, int tream);
int dpcm_be_dai_trigger(struct snd_soc_pcm_runtime *fe, int stream, int cmd);
int dpcm_be_dai_prepare(struct snd_soc_pcm_runtime *fe, int stream);
int dpcm_dapm_stream_event(struct snd_soc_pcm_runtime *fe, int dir,
	int event);

static inline void dpcm_path_put(struct snd_soc_dapm_widget_list **list)
{
	kfree(*list);
}


#endif
