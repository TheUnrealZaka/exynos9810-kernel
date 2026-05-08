#ifndef __SOUND_SEQ_VIRMIDI_H
#define __SOUND_SEQ_VIRMIDI_H



#include <sound/rawmidi.h>
#include <sound/seq_midi_event.h>


struct snd_virmidi {
	struct list_head list;
	int seq_mode;
	int client;
	int port;
	unsigned int trigger: 1;
	struct snd_midi_event *parser;
	struct snd_seq_event event;
	struct snd_virmidi_dev *rdev;
	struct snd_rawmidi_substream *substream;
};

#define SNDRV_VIRMIDI_SUBSCRIBE		(1<<0)
#define SNDRV_VIRMIDI_USE		(1<<1)


struct snd_virmidi_dev {
	struct snd_card *card;		
	struct snd_rawmidi *rmidi;		
	int seq_mode;			
	int device;			
	int client;			
	int port;			
	unsigned int flags;		
	rwlock_t filelist_lock;
	struct rw_semaphore filelist_sem;
	struct list_head filelist;
};


#define SNDRV_VIRMIDI_SEQ_NONE		0
#define SNDRV_VIRMIDI_SEQ_ATTACH	1
#define SNDRV_VIRMIDI_SEQ_DISPATCH	2

int snd_virmidi_new(struct snd_card *card, int device, struct snd_rawmidi **rrmidi);

#endif 
