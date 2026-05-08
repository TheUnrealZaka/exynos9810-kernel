

#ifndef __SOUND_HDA_HWDEP_H
#define __SOUND_HDA_HWDEP_H

#define HDA_HWDEP_VERSION	((1 << 16) | (0 << 8) | (0 << 0)) 


#define HDA_REG_NID_SHIFT	24
#define HDA_REG_VERB_SHIFT	8
#define HDA_REG_VAL_SHIFT	0
#define HDA_VERB(nid,verb,param)	((nid)<<24 | (verb)<<8 | (param))

struct hda_verb_ioctl {
	u32 verb;	
	u32 res;	
};


#define HDA_IOCTL_PVERSION		_IOR('H', 0x10, int)
#define HDA_IOCTL_VERB_WRITE		_IOWR('H', 0x11, struct hda_verb_ioctl)
#define HDA_IOCTL_GET_WCAP		_IOWR('H', 0x12, struct hda_verb_ioctl)

#endif
