#ifndef __SOUND_SEQ_MIDI_EVENT_H
#define __SOUND_SEQ_MIDI_EVENT_H



#include <sound/asequencer.h>

#define MAX_MIDI_EVENT_BUF	256


struct snd_midi_event {
	int qlen;		
	int read;		
	int type;		
	unsigned char lastcmd;	
	unsigned char nostat;	
	int bufsize;		
	unsigned char *buf;	
	spinlock_t lock;
};

int snd_midi_event_new(int bufsize, struct snd_midi_event **rdev);
void snd_midi_event_free(struct snd_midi_event *dev);
void snd_midi_event_reset_encode(struct snd_midi_event *dev);
void snd_midi_event_reset_decode(struct snd_midi_event *dev);
void snd_midi_event_no_status(struct snd_midi_event *dev, int on);

long snd_midi_event_encode(struct snd_midi_event *dev, unsigned char *buf, long count,
			   struct snd_seq_event *ev);
int snd_midi_event_encode_byte(struct snd_midi_event *dev, int c, struct snd_seq_event *ev);

long snd_midi_event_decode(struct snd_midi_event *dev, unsigned char *buf, long count,
			   struct snd_seq_event *ev);

#endif 
