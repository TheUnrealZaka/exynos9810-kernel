
#ifndef __SOUND_ASEQUENCER_H
#define __SOUND_ASEQUENCER_H

#include <linux/ioctl.h>
#include <sound/asound.h>
#include <uapi/sound/asequencer.h>


#define snd_seq_event_bounce_ext_data(ev) ((void*)((char *)(ev)->data.ext.ptr + sizeof(struct snd_seq_event_bounce)))



#define snd_seq_ev_is_result_type(ev)	((ev)->type < 5)

#define snd_seq_ev_is_channel_type(ev)	((ev)->type >= 5 && (ev)->type < 20)

#define snd_seq_ev_is_note_type(ev)	((ev)->type >= 5 && (ev)->type < 10)

#define snd_seq_ev_is_control_type(ev)	((ev)->type >= 10 && (ev)->type < 20)

#define snd_seq_ev_is_queue_type(ev)	((ev)->type >= 30 && (ev)->type < 40)

#define snd_seq_ev_is_message_type(ev)	((ev)->type >= 60 && (ev)->type < 69)

#define snd_seq_ev_is_sample_type(ev)	((ev)->type >= 70 && (ev)->type < 79)

#define snd_seq_ev_is_user_type(ev)	((ev)->type >= 90 && (ev)->type < 99)

#define snd_seq_ev_is_fixed_type(ev)	((ev)->type < 100)

#define snd_seq_ev_is_variable_type(ev)	((ev)->type >= 130 && (ev)->type < 140)

#define snd_seq_ev_is_reserved(ev)	((ev)->type >= 150)


#define snd_seq_ev_is_direct(ev)	((ev)->queue == SNDRV_SEQ_QUEUE_DIRECT)



#define snd_seq_ev_is_prior(ev)		(((ev)->flags & SNDRV_SEQ_PRIORITY_MASK) == SNDRV_SEQ_PRIORITY_HIGH)


#define snd_seq_ev_length_type(ev)	((ev)->flags & SNDRV_SEQ_EVENT_LENGTH_MASK)
#define snd_seq_ev_is_fixed(ev)		(snd_seq_ev_length_type(ev) == SNDRV_SEQ_EVENT_LENGTH_FIXED)
#define snd_seq_ev_is_variable(ev)	(snd_seq_ev_length_type(ev) == SNDRV_SEQ_EVENT_LENGTH_VARIABLE)
#define snd_seq_ev_is_varusr(ev)	(snd_seq_ev_length_type(ev) == SNDRV_SEQ_EVENT_LENGTH_VARUSR)


#define snd_seq_ev_timestamp_type(ev)	((ev)->flags & SNDRV_SEQ_TIME_STAMP_MASK)
#define snd_seq_ev_is_tick(ev)		(snd_seq_ev_timestamp_type(ev) == SNDRV_SEQ_TIME_STAMP_TICK)
#define snd_seq_ev_is_real(ev)		(snd_seq_ev_timestamp_type(ev) == SNDRV_SEQ_TIME_STAMP_REAL)


#define snd_seq_ev_timemode_type(ev)	((ev)->flags & SNDRV_SEQ_TIME_MODE_MASK)
#define snd_seq_ev_is_abstime(ev)	(snd_seq_ev_timemode_type(ev) == SNDRV_SEQ_TIME_MODE_ABS)
#define snd_seq_ev_is_reltime(ev)	(snd_seq_ev_timemode_type(ev) == SNDRV_SEQ_TIME_MODE_REL)


#define snd_seq_queue_sync_port(q)	((q) + 16)

#endif 
