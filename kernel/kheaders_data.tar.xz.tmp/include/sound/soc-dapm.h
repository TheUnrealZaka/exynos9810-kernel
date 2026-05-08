

#ifndef __LINUX_SND_SOC_DAPM_H
#define __LINUX_SND_SOC_DAPM_H

#include <linux/types.h>
#include <sound/control.h>
#include <sound/soc-topology.h>
#include <sound/asoc.h>

struct device;


#define SND_SOC_NOPM	-1




#define SND_SOC_DAPM_VMID(wname) \
{	.id = snd_soc_dapm_vmid, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0}


#define SND_SOC_DAPM_SIGGEN(wname) \
{	.id = snd_soc_dapm_siggen, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM }
#define SND_SOC_DAPM_SINK(wname) \
{	.id = snd_soc_dapm_sink, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM }
#define SND_SOC_DAPM_INPUT(wname) \
{	.id = snd_soc_dapm_input, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM }
#define SND_SOC_DAPM_OUTPUT(wname) \
{	.id = snd_soc_dapm_output, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM }
#define SND_SOC_DAPM_MIC(wname, wevent) \
{	.id = snd_soc_dapm_mic, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD}
#define SND_SOC_DAPM_HP(wname, wevent) \
{	.id = snd_soc_dapm_hp, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD}
#define SND_SOC_DAPM_SPK(wname, wevent) \
{	.id = snd_soc_dapm_spk, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD}
#define SND_SOC_DAPM_LINE(wname, wevent) \
{	.id = snd_soc_dapm_line, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD}

#define SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert) \
	.reg = wreg, .mask = 1, .shift = wshift, \
	.on_val = winvert ? 0 : 1, .off_val = winvert ? 1 : 0


#define SND_SOC_DAPM_PGA(wname, wreg, wshift, winvert,\
	 wcontrols, wncontrols) \
{	.id = snd_soc_dapm_pga, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols}
#define SND_SOC_DAPM_OUT_DRV(wname, wreg, wshift, winvert,\
	 wcontrols, wncontrols) \
{	.id = snd_soc_dapm_out_drv, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols}
#define SND_SOC_DAPM_MIXER(wname, wreg, wshift, winvert, \
	 wcontrols, wncontrols)\
{	.id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols}
#define SND_SOC_DAPM_MIXER_NAMED_CTL(wname, wreg, wshift, winvert, \
	 wcontrols, wncontrols)\
{       .id = snd_soc_dapm_mixer_named_ctl, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols}

#define SND_SOC_DAPM_MICBIAS(wname, wreg, wshift, winvert) \
{	.id = snd_soc_dapm_micbias, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = NULL, .num_kcontrols = 0}
#define SND_SOC_DAPM_SWITCH(wname, wreg, wshift, winvert, wcontrols) \
{	.id = snd_soc_dapm_switch, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1}
#define SND_SOC_DAPM_MUX(wname, wreg, wshift, winvert, wcontrols) \
{	.id = snd_soc_dapm_mux, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1}
#define SND_SOC_DAPM_DEMUX(wname, wreg, wshift, winvert, wcontrols) \
{	.id = snd_soc_dapm_demux, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1}


#define SOC_PGA_ARRAY(wname, wreg, wshift, winvert,\
	 wcontrols) \
{	.id = snd_soc_dapm_pga, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols)}
#define SOC_MIXER_ARRAY(wname, wreg, wshift, winvert, \
	 wcontrols)\
{	.id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols)}
#define SOC_MIXER_NAMED_CTL_ARRAY(wname, wreg, wshift, winvert, \
	 wcontrols)\
{       .id = snd_soc_dapm_mixer_named_ctl, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols)}


#define SND_SOC_DAPM_PGA_E(wname, wreg, wshift, winvert, wcontrols, \
	wncontrols, wevent, wflags) \
{	.id = snd_soc_dapm_pga, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols, \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_OUT_DRV_E(wname, wreg, wshift, winvert, wcontrols, \
	wncontrols, wevent, wflags) \
{	.id = snd_soc_dapm_out_drv, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols, \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_MIXER_E(wname, wreg, wshift, winvert, wcontrols, \
	wncontrols, wevent, wflags) \
{	.id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = wncontrols, \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_MIXER_NAMED_CTL_E(wname, wreg, wshift, winvert, \
	wcontrols, wncontrols, wevent, wflags) \
{       .id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, \
	.num_kcontrols = wncontrols, .event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_SWITCH_E(wname, wreg, wshift, winvert, wcontrols, \
	wevent, wflags) \
{	.id = snd_soc_dapm_switch, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1, \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_MUX_E(wname, wreg, wshift, winvert, wcontrols, \
	wevent, wflags) \
{	.id = snd_soc_dapm_mux, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1, \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_DEMUX_E(wname, wreg, wshift, winvert, wcontrols, \
	wevent, wflags) \
{	.id = snd_soc_dapm_demux, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = 1, \
	.event = wevent, .event_flags = wflags}


#define SND_SOC_DAPM_PGA_S(wname, wsubseq, wreg, wshift, winvert, \
	wevent, wflags) \
{	.id = snd_soc_dapm_pga, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags, \
	.subseq = wsubseq}
#define SND_SOC_DAPM_SUPPLY_S(wname, wsubseq, wreg, wshift, winvert, wevent, \
	wflags)	\
{	.id = snd_soc_dapm_supply, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags, .subseq = wsubseq}


#define SOC_PGA_E_ARRAY(wname, wreg, wshift, winvert, wcontrols, \
	wevent, wflags) \
{	.id = snd_soc_dapm_pga, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols), \
	.event = wevent, .event_flags = wflags}
#define SOC_MIXER_E_ARRAY(wname, wreg, wshift, winvert, wcontrols, \
	wevent, wflags) \
{	.id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols), \
	.event = wevent, .event_flags = wflags}
#define SOC_MIXER_NAMED_CTL_E_ARRAY(wname, wreg, wshift, winvert, \
	wcontrols, wevent, wflags) \
{       .id = snd_soc_dapm_mixer, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.kcontrol_news = wcontrols, .num_kcontrols = ARRAY_SIZE(wcontrols), \
	.event = wevent, .event_flags = wflags}


#define SND_SOC_DAPM_PRE(wname, wevent) \
{	.id = snd_soc_dapm_pre, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_PRE_PMD}
#define SND_SOC_DAPM_POST(wname, wevent) \
{	.id = snd_soc_dapm_post, .name = wname, .kcontrol_news = NULL, \
	.num_kcontrols = 0, .reg = SND_SOC_NOPM, .event = wevent, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_POST_PMD}


#define SND_SOC_DAPM_AIF_IN(wname, stname, wslot, wreg, wshift, winvert) \
{	.id = snd_soc_dapm_aif_in, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), }
#define SND_SOC_DAPM_AIF_IN_E(wname, stname, wslot, wreg, wshift, winvert, \
			      wevent, wflags)				\
{	.id = snd_soc_dapm_aif_in, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags }
#define SND_SOC_DAPM_AIF_OUT(wname, stname, wslot, wreg, wshift, winvert) \
{	.id = snd_soc_dapm_aif_out, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), }
#define SND_SOC_DAPM_AIF_OUT_E(wname, stname, wslot, wreg, wshift, winvert, \
			     wevent, wflags)				\
{	.id = snd_soc_dapm_aif_out, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags }
#define SND_SOC_DAPM_DAC(wname, stname, wreg, wshift, winvert) \
{	.id = snd_soc_dapm_dac, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert) }
#define SND_SOC_DAPM_DAC_E(wname, stname, wreg, wshift, winvert, \
			   wevent, wflags)				\
{	.id = snd_soc_dapm_dac, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags}

#define SND_SOC_DAPM_ADC(wname, stname, wreg, wshift, winvert) \
{	.id = snd_soc_dapm_adc, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), }
#define SND_SOC_DAPM_ADC_E(wname, stname, wreg, wshift, winvert, \
			   wevent, wflags)				\
{	.id = snd_soc_dapm_adc, .name = wname, .sname = stname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_CLOCK_SUPPLY(wname) \
{	.id = snd_soc_dapm_clock_supply, .name = wname, \
	.reg = SND_SOC_NOPM, .event = dapm_clock_event, \
	.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD }


#define SND_SOC_DAPM_REG(wid, wname, wreg, wshift, wmask, won_val, woff_val) \
{	.id = wid, .name = wname, .kcontrol_news = NULL, .num_kcontrols = 0, \
	.reg = wreg, .shift = wshift, .mask = wmask, \
	.on_val = won_val, .off_val = woff_val, }
#define SND_SOC_DAPM_SUPPLY(wname, wreg, wshift, winvert, wevent, wflags) \
{	.id = snd_soc_dapm_supply, .name = wname, \
	SND_SOC_DAPM_INIT_REG_VAL(wreg, wshift, winvert), \
	.event = wevent, .event_flags = wflags}
#define SND_SOC_DAPM_REGULATOR_SUPPLY(wname, wdelay, wflags)	    \
{	.id = snd_soc_dapm_regulator_supply, .name = wname, \
	.reg = SND_SOC_NOPM, .shift = wdelay, .event = dapm_regulator_event, \
	.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD, \
	.on_val = wflags}



#define SOC_DAPM_SINGLE(xname, reg, shift, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_dapm_get_volsw, .put = snd_soc_dapm_put_volsw, \
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
#define SOC_DAPM_SINGLE_AUTODISABLE(xname, reg, shift, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_dapm_get_volsw, .put = snd_soc_dapm_put_volsw, \
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 1) }
#define SOC_DAPM_SINGLE_VIRT(xname, max) \
	SOC_DAPM_SINGLE(xname, SND_SOC_NOPM, 0, max, 0)
#define SOC_DAPM_SINGLE_TLV(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.get = snd_soc_dapm_get_volsw, .put = snd_soc_dapm_put_volsw, \
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
#define SOC_DAPM_SINGLE_TLV_AUTODISABLE(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.get = snd_soc_dapm_get_volsw, .put = snd_soc_dapm_put_volsw, \
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 1) }
#define SOC_DAPM_SINGLE_TLV_VIRT(xname, max, tlv_array) \
	SOC_DAPM_SINGLE(xname, SND_SOC_NOPM, 0, max, 0, tlv_array)
#define SOC_DAPM_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_enum_double, \
 	.get = snd_soc_dapm_get_enum_double, \
 	.put = snd_soc_dapm_put_enum_double, \
  	.private_value = (unsigned long)&xenum }
#define SOC_DAPM_ENUM_EXT(xname, xenum, xget, xput) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_enum_double, \
	.get = xget, \
	.put = xput, \
	.private_value = (unsigned long)&xenum }
#define SOC_DAPM_PIN_SWITCH(xname) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname " Switch", \
	.info = snd_soc_dapm_info_pin_switch, \
	.get = snd_soc_dapm_get_pin_switch, \
	.put = snd_soc_dapm_put_pin_switch, \
	.private_value = (unsigned long)xname }


#define SND_SOC_DAPM_STREAM_NOP			0x0
#define SND_SOC_DAPM_STREAM_START		0x1
#define SND_SOC_DAPM_STREAM_STOP		0x2
#define SND_SOC_DAPM_STREAM_SUSPEND		0x4
#define SND_SOC_DAPM_STREAM_RESUME		0x8
#define SND_SOC_DAPM_STREAM_PAUSE_PUSH	0x10
#define SND_SOC_DAPM_STREAM_PAUSE_RELEASE	0x20


#define SND_SOC_DAPM_PRE_PMU	0x1 	
#define SND_SOC_DAPM_POST_PMU	0x2		
#define SND_SOC_DAPM_PRE_PMD	0x4 	
#define SND_SOC_DAPM_POST_PMD	0x8		
#define SND_SOC_DAPM_PRE_REG	0x10	
#define SND_SOC_DAPM_POST_REG	0x20	
#define SND_SOC_DAPM_WILL_PMU   0x40    
#define SND_SOC_DAPM_WILL_PMD   0x80    
#define SND_SOC_DAPM_PRE_POST_PMD \
				(SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMD)
#define SND_SOC_DAPM_PRE_POST_PMU \
				(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMU)


#define SND_SOC_DAPM_EVENT_ON(e)	\
	(e & (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMU))
#define SND_SOC_DAPM_EVENT_OFF(e)	\
	(e & (SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMD))


#define SND_SOC_DAPM_REGULATOR_BYPASS     0x1     

struct snd_soc_dapm_widget;
enum snd_soc_dapm_type;
struct snd_soc_dapm_path;
struct snd_soc_dapm_pin;
struct snd_soc_dapm_route;
struct snd_soc_dapm_context;
struct regulator;
struct snd_soc_dapm_widget_list;
struct snd_soc_dapm_update;
enum snd_soc_dapm_direction;

int dapm_regulator_event(struct snd_soc_dapm_widget *w,
			 struct snd_kcontrol *kcontrol, int event);
int dapm_clock_event(struct snd_soc_dapm_widget *w,
			 struct snd_kcontrol *kcontrol, int event);


int snd_soc_dapm_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_dapm_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_dapm_get_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_dapm_put_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_dapm_info_pin_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_dapm_get_pin_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *uncontrol);
int snd_soc_dapm_put_pin_switch(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *uncontrol);
int snd_soc_dapm_new_controls(struct snd_soc_dapm_context *dapm,
	const struct snd_soc_dapm_widget *widget,
	int num);
struct snd_soc_dapm_widget *snd_soc_dapm_new_control(
		struct snd_soc_dapm_context *dapm,
		const struct snd_soc_dapm_widget *widget);
int snd_soc_dapm_new_dai_widgets(struct snd_soc_dapm_context *dapm,
				 struct snd_soc_dai *dai);
int snd_soc_dapm_link_dai_widgets(struct snd_soc_card *card);
void snd_soc_dapm_connect_dai_link_widgets(struct snd_soc_card *card);
int snd_soc_dapm_new_pcm(struct snd_soc_card *card,
			 const struct snd_soc_pcm_stream *params,
			 unsigned int num_params,
			 struct snd_soc_dapm_widget *source,
			 struct snd_soc_dapm_widget *sink);


int snd_soc_dapm_new_widgets(struct snd_soc_card *card);
void snd_soc_dapm_free(struct snd_soc_dapm_context *dapm);
int snd_soc_dapm_add_routes(struct snd_soc_dapm_context *dapm,
			    const struct snd_soc_dapm_route *route, int num);
int snd_soc_dapm_del_routes(struct snd_soc_dapm_context *dapm,
			    const struct snd_soc_dapm_route *route, int num);
int snd_soc_dapm_weak_routes(struct snd_soc_dapm_context *dapm,
			     const struct snd_soc_dapm_route *route, int num);
void snd_soc_dapm_free_widget(struct snd_soc_dapm_widget *w);
void snd_soc_dapm_reset_cache(struct snd_soc_dapm_context *dapm);


void snd_soc_dapm_stream_event(struct snd_soc_pcm_runtime *rtd, int stream,
	int event);
void snd_soc_dapm_shutdown(struct snd_soc_card *card);


int snd_soc_dapm_mixer_update_power(struct snd_soc_dapm_context *dapm,
		struct snd_kcontrol *kcontrol, int connect,
		struct snd_soc_dapm_update *update);
int snd_soc_dapm_mux_update_power(struct snd_soc_dapm_context *dapm,
		struct snd_kcontrol *kcontrol, int mux, struct soc_enum *e,
		struct snd_soc_dapm_update *update);


extern struct attribute *soc_dapm_dev_attrs[];
void snd_soc_dapm_debugfs_init(struct snd_soc_dapm_context *dapm,
				struct dentry *parent);


int snd_soc_dapm_enable_pin(struct snd_soc_dapm_context *dapm,
			    const char *pin);
int snd_soc_dapm_enable_pin_unlocked(struct snd_soc_dapm_context *dapm,
				     const char *pin);
int snd_soc_dapm_disable_pin(struct snd_soc_dapm_context *dapm,
			     const char *pin);
int snd_soc_dapm_disable_pin_unlocked(struct snd_soc_dapm_context *dapm,
				      const char *pin);
int snd_soc_dapm_nc_pin(struct snd_soc_dapm_context *dapm, const char *pin);
int snd_soc_dapm_nc_pin_unlocked(struct snd_soc_dapm_context *dapm,
				 const char *pin);
int snd_soc_dapm_get_pin_status(struct snd_soc_dapm_context *dapm,
				const char *pin);
int snd_soc_dapm_sync(struct snd_soc_dapm_context *dapm);
int snd_soc_dapm_sync_unlocked(struct snd_soc_dapm_context *dapm);
int snd_soc_dapm_force_enable_pin(struct snd_soc_dapm_context *dapm,
				  const char *pin);
int snd_soc_dapm_force_enable_pin_unlocked(struct snd_soc_dapm_context *dapm,
					   const char *pin);
int snd_soc_dapm_ignore_suspend(struct snd_soc_dapm_context *dapm,
				const char *pin);
unsigned int dapm_kcontrol_get_value(const struct snd_kcontrol *kcontrol);


void dapm_mark_endpoints_dirty(struct snd_soc_card *card);


int snd_soc_dapm_dai_get_connected_widgets(struct snd_soc_dai *dai, int stream,
	struct snd_soc_dapm_widget_list **list,
	bool (*custom_stop_condition)(struct snd_soc_dapm_widget *,
				      enum snd_soc_dapm_direction));

struct snd_soc_dapm_context *snd_soc_dapm_kcontrol_dapm(
	struct snd_kcontrol *kcontrol);

struct snd_soc_dapm_widget *snd_soc_dapm_kcontrol_widget(
		struct snd_kcontrol *kcontrol);

int snd_soc_dapm_force_bias_level(struct snd_soc_dapm_context *dapm,
	enum snd_soc_bias_level level);

int snd_soc_dapm_connected_output_ep(struct snd_soc_dapm_widget *widget,
	struct list_head *list);

int snd_soc_dapm_connected_input_ep(struct snd_soc_dapm_widget *widget,
	struct list_head *list);


enum snd_soc_dapm_type {
	snd_soc_dapm_input = 0,		
	snd_soc_dapm_output,		
	snd_soc_dapm_mux,			
	snd_soc_dapm_demux,			
	snd_soc_dapm_mixer,			
	snd_soc_dapm_mixer_named_ctl,		
	snd_soc_dapm_pga,			
	snd_soc_dapm_out_drv,			
	snd_soc_dapm_adc,			
	snd_soc_dapm_dac,			
	snd_soc_dapm_micbias,		
	snd_soc_dapm_mic,			
	snd_soc_dapm_hp,			
	snd_soc_dapm_spk,			
	snd_soc_dapm_line,			
	snd_soc_dapm_switch,		
	snd_soc_dapm_vmid,			
	snd_soc_dapm_pre,			
	snd_soc_dapm_post,			
	snd_soc_dapm_supply,		
	snd_soc_dapm_regulator_supply,	
	snd_soc_dapm_clock_supply,	
	snd_soc_dapm_aif_in,		
	snd_soc_dapm_aif_out,		
	snd_soc_dapm_siggen,		
	snd_soc_dapm_sink,
	snd_soc_dapm_dai_in,		
	snd_soc_dapm_dai_out,
	snd_soc_dapm_dai_link,		
	snd_soc_dapm_kcontrol,		
};

enum snd_soc_dapm_subclass {
	SND_SOC_DAPM_CLASS_INIT		= 0,
	SND_SOC_DAPM_CLASS_RUNTIME	= 1,
};


struct snd_soc_dapm_route {
	const char *sink;
	const char *control;
	const char *source;

	
	int (*connected)(struct snd_soc_dapm_widget *source,
			 struct snd_soc_dapm_widget *sink);
};


struct snd_soc_dapm_path {
	const char *name;

	
	union {
		struct {
			struct snd_soc_dapm_widget *source;
			struct snd_soc_dapm_widget *sink;
		};
		struct snd_soc_dapm_widget *node[2];
	};

	
	u32 connect:1;	
	u32 walking:1;  
	u32 weak:1;	
	u32 is_supply:1;	

	int (*connected)(struct snd_soc_dapm_widget *source,
			 struct snd_soc_dapm_widget *sink);

	struct list_head list_node[2];
	struct list_head list_kcontrol;
	struct list_head list;
};


struct snd_soc_dapm_widget {
	enum snd_soc_dapm_type id;
	const char *name;		
	const char *sname;	
	struct list_head list;
	struct snd_soc_dapm_context *dapm;

	void *priv;				
	struct regulator *regulator;		
	const struct snd_soc_pcm_stream *params; 
	unsigned int num_params; 
	unsigned int params_select; 

	
	int reg;				
	unsigned char shift;			
	unsigned int mask;			
	unsigned int on_val;			
	unsigned int off_val;			
	unsigned char power:1;			
	unsigned char active:1;			
	unsigned char connected:1;		
	unsigned char new:1;			
	unsigned char force:1;			
	unsigned char ignore_suspend:1;         
	unsigned char new_power:1;		
	unsigned char power_checked:1;		
	unsigned char is_supply:1;		
	unsigned char is_ep:2;			
	int subseq;				

	int (*power_check)(struct snd_soc_dapm_widget *w);

	
	unsigned short event_flags;		
	int (*event)(struct snd_soc_dapm_widget*, struct snd_kcontrol *, int);

	
	int num_kcontrols;
	const struct snd_kcontrol_new *kcontrol_news;
	struct snd_kcontrol **kcontrols;
	struct snd_soc_dobj dobj;

	
	struct list_head edges[2];

	
	struct list_head work_list;
	struct list_head power_list;
	struct list_head dirty;
	int endpoints[2];

	struct clk *clk;
};

struct snd_soc_dapm_update {
	struct snd_kcontrol *kcontrol;
	int reg;
	int mask;
	int val;
};

struct snd_soc_dapm_wcache {
	struct snd_soc_dapm_widget *widget;
};


struct snd_soc_dapm_context {
	enum snd_soc_bias_level bias_level;
	unsigned int idle_bias_off:1; 
	
	unsigned int suspend_bias_off:1;
	void (*seq_notifier)(struct snd_soc_dapm_context *,
			     enum snd_soc_dapm_type, int);

	struct device *dev; 
	struct snd_soc_component *component; 
	struct snd_soc_card *card; 

	
	enum snd_soc_bias_level target_bias_level;
	struct list_head list;

	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);
	int (*set_bias_level)(struct snd_soc_dapm_context *dapm,
			      enum snd_soc_bias_level level);

	struct snd_soc_dapm_wcache path_sink_cache;
	struct snd_soc_dapm_wcache path_source_cache;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dapm;
#endif
};


struct snd_soc_dapm_widget_list {
	int num_widgets;
	struct snd_soc_dapm_widget *widgets[0];
};

struct snd_soc_dapm_stats {
	int power_checks;
	int path_checks;
	int neighbour_checks;
};


static inline void snd_soc_dapm_init_bias_level(
	struct snd_soc_dapm_context *dapm, enum snd_soc_bias_level level)
{
	dapm->bias_level = level;
}


static inline enum snd_soc_bias_level snd_soc_dapm_get_bias_level(
	struct snd_soc_dapm_context *dapm)
{
	return dapm->bias_level;
}

enum snd_soc_dapm_direction {
	SND_SOC_DAPM_DIR_IN,
	SND_SOC_DAPM_DIR_OUT
};

#define SND_SOC_DAPM_DIR_TO_EP(x) BIT(x)

#define SND_SOC_DAPM_EP_SOURCE SND_SOC_DAPM_DIR_TO_EP(SND_SOC_DAPM_DIR_IN)
#define SND_SOC_DAPM_EP_SINK SND_SOC_DAPM_DIR_TO_EP(SND_SOC_DAPM_DIR_OUT)


#define snd_soc_dapm_widget_for_each_path(w, dir, p) \
	list_for_each_entry(p, &w->edges[dir], list_node[dir])


#define snd_soc_dapm_widget_for_each_path_safe(w, dir, p, next_p) \
	list_for_each_entry_safe(p, next_p, &w->edges[dir], list_node[dir])


#define snd_soc_dapm_widget_for_each_sink_path(w, p) \
	snd_soc_dapm_widget_for_each_path(w, SND_SOC_DAPM_DIR_IN, p)


#define snd_soc_dapm_widget_for_each_source_path(w, p) \
	snd_soc_dapm_widget_for_each_path(w, SND_SOC_DAPM_DIR_OUT, p)

#endif
