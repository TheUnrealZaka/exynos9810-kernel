

#ifndef __SOUND_HDA_REGMAP_H
#define __SOUND_HDA_REGMAP_H

#include <linux/regmap.h>
#include <sound/core.h>
#include <sound/hdaudio.h>

#define AC_AMP_FAKE_MUTE	0x10	

int snd_hdac_regmap_init(struct hdac_device *codec);
void snd_hdac_regmap_exit(struct hdac_device *codec);
int snd_hdac_regmap_add_vendor_verb(struct hdac_device *codec,
				    unsigned int verb);
int snd_hdac_regmap_read_raw(struct hdac_device *codec, unsigned int reg,
			     unsigned int *val);
int snd_hdac_regmap_read_raw_uncached(struct hdac_device *codec,
				      unsigned int reg, unsigned int *val);
int snd_hdac_regmap_write_raw(struct hdac_device *codec, unsigned int reg,
			      unsigned int val);
int snd_hdac_regmap_update_raw(struct hdac_device *codec, unsigned int reg,
			       unsigned int mask, unsigned int val);


#define snd_hdac_regmap_encode_verb(nid, verb)		\
	(((verb) << 8) | 0x80000 | ((unsigned int)(nid) << 20))


#define snd_hdac_regmap_encode_amp(nid, ch, dir, idx)			\
	(snd_hdac_regmap_encode_verb(nid, AC_VERB_GET_AMP_GAIN_MUTE) |	\
	 ((ch) ? AC_AMP_GET_RIGHT : AC_AMP_GET_LEFT) |			\
	 ((dir) == HDA_OUTPUT ? AC_AMP_GET_OUTPUT : AC_AMP_GET_INPUT) | \
	 (idx))


#define snd_hdac_regmap_encode_amp_stereo(nid, dir, idx)		\
	(snd_hdac_regmap_encode_verb(nid, AC_VERB_GET_AMP_GAIN_MUTE) |	\
	 AC_AMP_SET_LEFT | AC_AMP_SET_RIGHT | 	\
	 ((dir) == HDA_OUTPUT ? AC_AMP_GET_OUTPUT : AC_AMP_GET_INPUT) | \
	 (idx))


static inline int
snd_hdac_regmap_write(struct hdac_device *codec, hda_nid_t nid,
		      unsigned int verb, unsigned int val)
{
	unsigned int cmd = snd_hdac_regmap_encode_verb(nid, verb);

	return snd_hdac_regmap_write_raw(codec, cmd, val);
}


static inline int
snd_hdac_regmap_update(struct hdac_device *codec, hda_nid_t nid,
		       unsigned int verb, unsigned int mask,
		       unsigned int val)
{
	unsigned int cmd = snd_hdac_regmap_encode_verb(nid, verb);

	return snd_hdac_regmap_update_raw(codec, cmd, mask, val);
}


static inline int
snd_hdac_regmap_read(struct hdac_device *codec, hda_nid_t nid,
		     unsigned int verb, unsigned int *val)
{
	unsigned int cmd = snd_hdac_regmap_encode_verb(nid, verb);

	return snd_hdac_regmap_read_raw(codec, cmd, val);
}


static inline int
snd_hdac_regmap_get_amp(struct hdac_device *codec, hda_nid_t nid,
			int ch, int dir, int idx)
{
	unsigned int cmd = snd_hdac_regmap_encode_amp(nid, ch, dir, idx);
	int err, val;

	err = snd_hdac_regmap_read_raw(codec, cmd, &val);
	return err < 0 ? err : val;
}


static inline int
snd_hdac_regmap_update_amp(struct hdac_device *codec, hda_nid_t nid,
			   int ch, int dir, int idx, int mask, int val)
{
	unsigned int cmd = snd_hdac_regmap_encode_amp(nid, ch, dir, idx);

	return snd_hdac_regmap_update_raw(codec, cmd, mask, val);
}


static inline int
snd_hdac_regmap_get_amp_stereo(struct hdac_device *codec, hda_nid_t nid,
			       int dir, int idx)
{
	unsigned int cmd = snd_hdac_regmap_encode_amp_stereo(nid, dir, idx);
	int err, val;

	err = snd_hdac_regmap_read_raw(codec, cmd, &val);
	return err < 0 ? err : val;
}


static inline int
snd_hdac_regmap_update_amp_stereo(struct hdac_device *codec, hda_nid_t nid,
				  int dir, int idx, int mask, int val)
{
	unsigned int cmd = snd_hdac_regmap_encode_amp_stereo(nid, dir, idx);

	return snd_hdac_regmap_update_raw(codec, cmd, mask, val);
}


static inline void
snd_hdac_regmap_sync_node(struct hdac_device *codec, hda_nid_t nid)
{
	regcache_mark_dirty(codec->regmap);
	regcache_sync_region(codec->regmap, nid << 20, ((nid + 1) << 20) - 1);
}

#endif 
