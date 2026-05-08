

#ifndef __LINUX_SND_SOC_DAI_H
#define __LINUX_SND_SOC_DAI_H


#include <linux/list.h>

struct snd_pcm_substream;
struct snd_soc_dapm_widget;
struct snd_compr_stream;


#define SND_SOC_DAIFMT_I2S		1 
#define SND_SOC_DAIFMT_RIGHT_J		2 
#define SND_SOC_DAIFMT_LEFT_J		3 
#define SND_SOC_DAIFMT_DSP_A		4 
#define SND_SOC_DAIFMT_DSP_B		5 
#define SND_SOC_DAIFMT_AC97		6 
#define SND_SOC_DAIFMT_PDM		7 


#define SND_SOC_DAIFMT_MSB		SND_SOC_DAIFMT_LEFT_J
#define SND_SOC_DAIFMT_LSB		SND_SOC_DAIFMT_RIGHT_J


#define SND_SOC_DAIFMT_CONT		(1 << 4) 
#define SND_SOC_DAIFMT_GATED		(0 << 4) 


#define SND_SOC_DAIFMT_NB_NF		(0 << 8) 
#define SND_SOC_DAIFMT_NB_IF		(2 << 8) 
#define SND_SOC_DAIFMT_IB_NF		(3 << 8) 
#define SND_SOC_DAIFMT_IB_IF		(4 << 8) 


#define SND_SOC_DAIFMT_CBM_CFM		(1 << 12) 
#define SND_SOC_DAIFMT_CBS_CFM		(2 << 12) 
#define SND_SOC_DAIFMT_CBM_CFS		(3 << 12) 
#define SND_SOC_DAIFMT_CBS_CFS		(4 << 12) 

#define SND_SOC_DAIFMT_FORMAT_MASK	0x000f
#define SND_SOC_DAIFMT_CLOCK_MASK	0x00f0
#define SND_SOC_DAIFMT_INV_MASK		0x0f00
#define SND_SOC_DAIFMT_MASTER_MASK	0xf000


#define SND_SOC_CLOCK_IN		0
#define SND_SOC_CLOCK_OUT		1

#define SND_SOC_STD_AC97_FMTS (SNDRV_PCM_FMTBIT_S8 |\
			       SNDRV_PCM_FMTBIT_S16_LE |\
			       SNDRV_PCM_FMTBIT_S16_BE |\
			       SNDRV_PCM_FMTBIT_S20_3LE |\
			       SNDRV_PCM_FMTBIT_S20_3BE |\
			       SNDRV_PCM_FMTBIT_S24_3LE |\
			       SNDRV_PCM_FMTBIT_S24_3BE |\
                               SNDRV_PCM_FMTBIT_S32_LE |\
                               SNDRV_PCM_FMTBIT_S32_BE)

struct snd_soc_dai_driver;
struct snd_soc_dai;
struct snd_ac97_bus_ops;


int snd_soc_dai_set_sysclk(struct snd_soc_dai *dai, int clk_id,
	unsigned int freq, int dir);

int snd_soc_dai_set_clkdiv(struct snd_soc_dai *dai,
	int div_id, int div);

int snd_soc_dai_set_pll(struct snd_soc_dai *dai,
	int pll_id, int source, unsigned int freq_in, unsigned int freq_out);

int snd_soc_dai_set_bclk_ratio(struct snd_soc_dai *dai, unsigned int ratio);


int snd_soc_dai_set_fmt(struct snd_soc_dai *dai, unsigned int fmt);

int snd_soc_dai_set_tdm_slot(struct snd_soc_dai *dai,
	unsigned int tx_mask, unsigned int rx_mask, int slots, int slot_width);

int snd_soc_dai_set_channel_map(struct snd_soc_dai *dai,
	unsigned int tx_num, unsigned int *tx_slot,
	unsigned int rx_num, unsigned int *rx_slot);

int snd_soc_dai_set_tristate(struct snd_soc_dai *dai, int tristate);


int snd_soc_dai_digital_mute(struct snd_soc_dai *dai, int mute,
			     int direction);

int snd_soc_dai_is_dummy(struct snd_soc_dai *dai);

struct snd_soc_dai_ops {
	
	int (*set_sysclk)(struct snd_soc_dai *dai,
		int clk_id, unsigned int freq, int dir);
	int (*set_pll)(struct snd_soc_dai *dai, int pll_id, int source,
		unsigned int freq_in, unsigned int freq_out);
	int (*set_clkdiv)(struct snd_soc_dai *dai, int div_id, int div);
	int (*set_bclk_ratio)(struct snd_soc_dai *dai, unsigned int ratio);

	
	int (*set_fmt)(struct snd_soc_dai *dai, unsigned int fmt);
	int (*xlate_tdm_slot_mask)(unsigned int slots,
		unsigned int *tx_mask, unsigned int *rx_mask);
	int (*set_tdm_slot)(struct snd_soc_dai *dai,
		unsigned int tx_mask, unsigned int rx_mask,
		int slots, int slot_width);
	int (*set_channel_map)(struct snd_soc_dai *dai,
		unsigned int tx_num, unsigned int *tx_slot,
		unsigned int rx_num, unsigned int *rx_slot);
	int (*set_tristate)(struct snd_soc_dai *dai, int tristate);

	
	int (*digital_mute)(struct snd_soc_dai *dai, int mute);
	int (*mute_stream)(struct snd_soc_dai *dai, int mute, int stream);

	
	int (*startup)(struct snd_pcm_substream *,
		struct snd_soc_dai *);
	void (*shutdown)(struct snd_pcm_substream *,
		struct snd_soc_dai *);
	int (*hw_params)(struct snd_pcm_substream *,
		struct snd_pcm_hw_params *, struct snd_soc_dai *);
	int (*hw_free)(struct snd_pcm_substream *,
		struct snd_soc_dai *);
	int (*prepare)(struct snd_pcm_substream *,
		struct snd_soc_dai *);
	
	int (*trigger)(struct snd_pcm_substream *, int,
		struct snd_soc_dai *);
	int (*bespoke_trigger)(struct snd_pcm_substream *, int,
		struct snd_soc_dai *);
	
	snd_pcm_sframes_t (*delay)(struct snd_pcm_substream *,
		struct snd_soc_dai *);
};


struct snd_soc_dai_driver {
	
	const char *name;
	unsigned int id;
	unsigned int base;
	struct snd_soc_dobj dobj;

	
	int (*probe)(struct snd_soc_dai *dai);
	int (*remove)(struct snd_soc_dai *dai);
	int (*suspend)(struct snd_soc_dai *dai);
	int (*resume)(struct snd_soc_dai *dai);
	
	int (*compress_new)(struct snd_soc_pcm_runtime *rtd, int num);
	
	bool bus_control;

	
	const struct snd_soc_dai_ops *ops;

	
	struct snd_soc_pcm_stream capture;
	struct snd_soc_pcm_stream playback;
	unsigned int symmetric_rates:1;
	unsigned int symmetric_channels:1;
	unsigned int symmetric_samplebits:1;

	
	int probe_order;
	int remove_order;
};


struct snd_soc_dai {
	const char *name;
	int id;
	struct device *dev;

	
	struct snd_soc_dai_driver *driver;

	
	unsigned int capture_active:4;		
	unsigned int playback_active:4;		
	unsigned int symmetric_rates:1;
	unsigned int symmetric_channels:1;
	unsigned int symmetric_samplebits:1;
	unsigned int active;
	unsigned char probed:1;

	struct snd_soc_dapm_widget *playback_widget;
	struct snd_soc_dapm_widget *capture_widget;

	
	void *playback_dma_data;
	void *capture_dma_data;

	
	unsigned int rate;
	unsigned int channels;
	unsigned int sample_bits;
	unsigned int sample_width;

	
	struct snd_soc_codec *codec;
	struct snd_soc_component *component;

	
	unsigned int tx_mask;
	unsigned int rx_mask;

	struct list_head list;
};

static inline void *snd_soc_dai_get_dma_data(const struct snd_soc_dai *dai,
					     const struct snd_pcm_substream *ss)
{
	return (ss->stream == SNDRV_PCM_STREAM_PLAYBACK) ?
		dai->playback_dma_data : dai->capture_dma_data;
}

static inline void snd_soc_dai_set_dma_data(struct snd_soc_dai *dai,
					    const struct snd_pcm_substream *ss,
					    void *data)
{
	if (ss->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dai->playback_dma_data = data;
	else
		dai->capture_dma_data = data;
}

static inline void snd_soc_dai_init_dma_data(struct snd_soc_dai *dai,
					     void *playback, void *capture)
{
	dai->playback_dma_data = playback;
	dai->capture_dma_data = capture;
}

static inline void snd_soc_dai_set_drvdata(struct snd_soc_dai *dai,
		void *data)
{
	dev_set_drvdata(dai->dev, data);
}

static inline void *snd_soc_dai_get_drvdata(struct snd_soc_dai *dai)
{
	return dev_get_drvdata(dai->dev);
}

#endif
