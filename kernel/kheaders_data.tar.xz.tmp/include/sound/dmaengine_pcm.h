
#ifndef __SOUND_DMAENGINE_PCM_H__
#define __SOUND_DMAENGINE_PCM_H__

#include <sound/pcm.h>
#include <sound/soc.h>
#include <linux/dmaengine.h>


static inline enum dma_transfer_direction
snd_pcm_substream_to_dma_direction(const struct snd_pcm_substream *substream)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return DMA_MEM_TO_DEV;
	else
		return DMA_DEV_TO_MEM;
}

int snd_hwparams_to_dma_slave_config(const struct snd_pcm_substream *substream,
	const struct snd_pcm_hw_params *params, struct dma_slave_config *slave_config);
int snd_dmaengine_pcm_trigger(struct snd_pcm_substream *substream, int cmd);
snd_pcm_uframes_t snd_dmaengine_pcm_pointer(struct snd_pcm_substream *substream);
snd_pcm_uframes_t snd_dmaengine_pcm_pointer_no_residue(struct snd_pcm_substream *substream);

int snd_dmaengine_pcm_open(struct snd_pcm_substream *substream,
	struct dma_chan *chan);
int snd_dmaengine_pcm_close(struct snd_pcm_substream *substream);

int snd_dmaengine_pcm_open_request_chan(struct snd_pcm_substream *substream,
	dma_filter_fn filter_fn, void *filter_data);
int snd_dmaengine_pcm_close_release_chan(struct snd_pcm_substream *substream);

struct dma_chan *snd_dmaengine_pcm_request_channel(dma_filter_fn filter_fn,
	void *filter_data);
struct dma_chan *snd_dmaengine_pcm_get_chan(struct snd_pcm_substream *substream);


#define SND_DMAENGINE_PCM_DAI_FLAG_PACK BIT(0)


struct snd_dmaengine_dai_dma_data {
	dma_addr_t addr;
	enum dma_slave_buswidth addr_width;
	u32 maxburst;
	unsigned int slave_id;
	void *filter_data;
	const char *chan_name;
	unsigned int fifo_size;
	unsigned int flags;
};

void snd_dmaengine_pcm_set_config_from_dai_data(
	const struct snd_pcm_substream *substream,
	const struct snd_dmaengine_dai_dma_data *dma_data,
	struct dma_slave_config *config);



#define SND_DMAENGINE_PCM_FLAG_COMPAT BIT(0)

#define SND_DMAENGINE_PCM_FLAG_NO_DT BIT(1)

#define SND_DMAENGINE_PCM_FLAG_HALF_DUPLEX BIT(3)

#define SND_DMAENGINE_PCM_FLAG_CUSTOM_CHANNEL_NAME BIT(4)


struct snd_dmaengine_pcm_config {
	int (*prepare_slave_config)(struct snd_pcm_substream *substream,
			struct snd_pcm_hw_params *params,
			struct dma_slave_config *slave_config);
	struct dma_chan *(*compat_request_channel)(
			struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_substream *substream);
	dma_filter_fn compat_filter_fn;
	struct device *dma_dev;
	const char *chan_names[SNDRV_PCM_STREAM_LAST + 1];

	const struct snd_pcm_hardware *pcm_hardware;
	unsigned int prealloc_buffer_size;
};

int snd_dmaengine_pcm_register(struct device *dev,
	const struct snd_dmaengine_pcm_config *config,
	unsigned int flags);
void snd_dmaengine_pcm_unregister(struct device *dev);

int devm_snd_dmaengine_pcm_register(struct device *dev,
	const struct snd_dmaengine_pcm_config *config,
	unsigned int flags);

int snd_dmaengine_pcm_prepare_slave_config(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params,
	struct dma_slave_config *slave_config);

#endif
