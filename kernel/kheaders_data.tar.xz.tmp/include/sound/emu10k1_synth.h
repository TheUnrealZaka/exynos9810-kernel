#ifndef __EMU10K1_SYNTH_H
#define __EMU10K1_SYNTH_H


#include <sound/emu10k1.h>
#include <sound/emux_synth.h>


#define SNDRV_SEQ_DEV_ID_EMU10K1_SYNTH	"emu10k1-synth"


struct snd_emu10k1_synth_arg {
	struct snd_emu10k1 *hwptr;	
	int index;		
	int seq_ports;		
	int max_voices;		
};

#define EMU10K1_MAX_MEMSIZE	(32 * 1024 * 1024) 

#endif
