#ifndef __SOUND_SEQ_KERNEL_H
#define __SOUND_SEQ_KERNEL_H


#include <linux/time.h>
#include <sound/asequencer.h>

typedef struct snd_seq_real_time snd_seq_real_time_t;
typedef union snd_seq_timestamp snd_seq_timestamp_t;


#define SNDRV_SEQ_MAX_QUEUES		32


#define SNDRV_SEQ_MAX_CLIENTS 		192


#define SNDRV_SEQ_MAX_PORTS 		254


#define SNDRV_SEQ_MAX_EVENTS		2000


#define SNDRV_SEQ_DEFAULT_EVENTS	500


#define SNDRV_SEQ_MAX_CLIENT_EVENTS	2000


#define SNDRV_SEQ_DEFAULT_CLIENT_EVENTS	200



#define SNDRV_SEQ_MAX_HOPS		8


#define SNDRV_SEQ_MAX_EVENT_LEN		0x3fffffff


struct snd_seq_port_callback {
	struct module *owner;
	void *private_data;
	int (*subscribe)(void *private_data, struct snd_seq_port_subscribe *info);
	int (*unsubscribe)(void *private_data, struct snd_seq_port_subscribe *info);
	int (*use)(void *private_data, struct snd_seq_port_subscribe *info);
	int (*unuse)(void *private_data, struct snd_seq_port_subscribe *info);
	int (*event_input)(struct snd_seq_event *ev, int direct, void *private_data, int atomic, int hop);
	void (*private_free)(void *private_data);
	
};


__printf(3, 4)
int snd_seq_create_kernel_client(struct snd_card *card, int client_index,
				 const char *name_fmt, ...);
int snd_seq_delete_kernel_client(int client);
int snd_seq_kernel_client_enqueue(int client, struct snd_seq_event *ev, int atomic, int hop);
int snd_seq_kernel_client_dispatch(int client, struct snd_seq_event *ev, int atomic, int hop);
int snd_seq_kernel_client_ctl(int client, unsigned int cmd, void *arg);

#define SNDRV_SEQ_EXT_MASK	0xc0000000
#define SNDRV_SEQ_EXT_USRPTR	0x80000000
#define SNDRV_SEQ_EXT_CHAINED	0x40000000

typedef int (*snd_seq_dump_func_t)(void *ptr, void *buf, int count);
int snd_seq_expand_var_event(const struct snd_seq_event *event, int count, char *buf,
			     int in_kernel, int size_aligned);
int snd_seq_dump_var_event(const struct snd_seq_event *event,
			   snd_seq_dump_func_t func, void *private_data);


int snd_seq_set_queue_tempo(int client, struct snd_seq_queue_tempo *tempo);


void snd_port_init_callback(struct snd_seq_port_callback *p);
struct snd_seq_port_callback *snd_port_alloc_callback(void);


int snd_seq_event_port_attach(int client, struct snd_seq_port_callback *pcbp,
			      int cap, int type, int midi_channels, int midi_voices, char *portname);
int snd_seq_event_port_detach(int client, int port);

#ifdef CONFIG_MODULES
void snd_seq_autoload_init(void);
void snd_seq_autoload_exit(void);
#else
#define snd_seq_autoload_init()
#define snd_seq_autoload_exit()
#endif

#endif 
