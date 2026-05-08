#ifndef __SOUND_EMU8000_H
#define __SOUND_EMU8000_H


#include <sound/emux_synth.h>
#include <sound/seq_kernel.h>


#define EMU8000_MAX_DRAM (28 * 1024 * 1024) 
#define EMU8000_DRAM_OFFSET 0x200000	
#define EMU8000_CHANNELS   32	
#define EMU8000_DRAM_VOICES	30	


#define EMU8000_RAM_READ   0
#define EMU8000_RAM_WRITE  1
#define EMU8000_RAM_CLOSE  2
#define EMU8000_RAM_MODE_MASK	0x03
#define EMU8000_RAM_RIGHT	0x10	

enum {
	EMU8000_CONTROL_BASS = 0,
	EMU8000_CONTROL_TREBLE,
	EMU8000_CONTROL_CHORUS_MODE,
	EMU8000_CONTROL_REVERB_MODE,
	EMU8000_CONTROL_FM_CHORUS_DEPTH,
	EMU8000_CONTROL_FM_REVERB_DEPTH,
	EMU8000_NUM_CONTROLS,
};


struct snd_emu8000 {

	struct snd_emux *emu;

	int index;		
	int seq_ports;		
	int fm_chorus_depth;	
	int fm_reverb_depth;	

	int mem_size;		
	unsigned long port1;	
	unsigned long port2;	
	unsigned long port3;	
	struct resource *res_port1;
	struct resource *res_port2;
	struct resource *res_port3;
	unsigned short last_reg;
	spinlock_t reg_lock;

	int dram_checked;

	struct snd_card *card;		

	int chorus_mode;
	int reverb_mode;
	int bass_level;
	int treble_level;

	struct snd_util_memhdr *memhdr;

	spinlock_t control_lock;
	struct snd_kcontrol *controls[EMU8000_NUM_CONTROLS];

	struct snd_pcm *pcm; 

};


#define SNDRV_SEQ_DEV_ID_EMU8000	"emu8000-synth"



int snd_emu8000_new(struct snd_card *card, int device, long port, int seq_ports,
		    struct snd_seq_device **ret);
void snd_emu8000_poke(struct snd_emu8000 *emu, unsigned int port, unsigned int reg,
		      unsigned int val);
unsigned short snd_emu8000_peek(struct snd_emu8000 *emu, unsigned int port,
				unsigned int reg);
void snd_emu8000_poke_dw(struct snd_emu8000 *emu, unsigned int port, unsigned int reg,
			 unsigned int val);
unsigned int snd_emu8000_peek_dw(struct snd_emu8000 *emu, unsigned int port,
				 unsigned int reg);
void snd_emu8000_dma_chan(struct snd_emu8000 *emu, int ch, int mode);

void snd_emu8000_init_fm(struct snd_emu8000 *emu);

void snd_emu8000_update_chorus_mode(struct snd_emu8000 *emu);
void snd_emu8000_update_reverb_mode(struct snd_emu8000 *emu);
void snd_emu8000_update_equalizer(struct snd_emu8000 *emu);
int snd_emu8000_load_chorus_fx(struct snd_emu8000 *emu, int mode, const void __user *buf, long len);
int snd_emu8000_load_reverb_fx(struct snd_emu8000 *emu, int mode, const void __user *buf, long len);

#endif 
