#ifndef __SOUND_TIMER_H
#define __SOUND_TIMER_H



#include <sound/asound.h>
#include <linux/interrupt.h>

#define snd_timer_chip(timer) ((timer)->private_data)

#define SNDRV_TIMER_DEVICES	16

#define SNDRV_TIMER_DEV_FLG_PCM	0x10000000

#define SNDRV_TIMER_HW_AUTO	0x00000001	
#define SNDRV_TIMER_HW_STOP	0x00000002	
#define SNDRV_TIMER_HW_SLAVE	0x00000004	
#define SNDRV_TIMER_HW_FIRST	0x00000008	
#define SNDRV_TIMER_HW_TASKLET	0x00000010	

#define SNDRV_TIMER_IFLG_SLAVE	  0x00000001
#define SNDRV_TIMER_IFLG_RUNNING  0x00000002
#define SNDRV_TIMER_IFLG_START	  0x00000004
#define SNDRV_TIMER_IFLG_AUTO	  0x00000008	
#define SNDRV_TIMER_IFLG_FAST	  0x00000010	
#define SNDRV_TIMER_IFLG_CALLBACK 0x00000020	
#define SNDRV_TIMER_IFLG_EXCLUSIVE 0x00000040	
#define SNDRV_TIMER_IFLG_EARLY_EVENT 0x00000080	

#define SNDRV_TIMER_FLG_CHANGE	0x00000001
#define SNDRV_TIMER_FLG_RESCHED	0x00000002	

struct snd_timer;

struct snd_timer_hardware {
	
	unsigned int flags;		
	unsigned long resolution;	
	unsigned long resolution_min;	
	unsigned long resolution_max;	
	unsigned long ticks;		
	
	int (*open) (struct snd_timer * timer);
	int (*close) (struct snd_timer * timer);
	unsigned long (*c_resolution) (struct snd_timer * timer);
	int (*start) (struct snd_timer * timer);
	int (*stop) (struct snd_timer * timer);
	int (*set_period) (struct snd_timer * timer, unsigned long period_num, unsigned long period_den);
	int (*precise_resolution) (struct snd_timer * timer, unsigned long *num, unsigned long *den);
};

struct snd_timer {
	int tmr_class;
	struct snd_card *card;
	struct module *module;
	int tmr_device;
	int tmr_subdevice;
	char id[64];
	char name[80];
	unsigned int flags;
	int running;			
	unsigned long sticks;		
	void *private_data;
	void (*private_free) (struct snd_timer *timer);
	struct snd_timer_hardware hw;
	spinlock_t lock;
	struct list_head device_list;
	struct list_head open_list_head;
	struct list_head active_list_head;
	struct list_head ack_list_head;
	struct list_head sack_list_head; 
	struct tasklet_struct task_queue;
	int max_instances;	
	int num_instances;	
};

struct snd_timer_instance {
	struct snd_timer *timer;
	char *owner;
	unsigned int flags;
	void *private_data;
	void (*private_free) (struct snd_timer_instance *ti);
	void (*callback) (struct snd_timer_instance *timeri,
			  unsigned long ticks, unsigned long resolution);
	void (*ccallback) (struct snd_timer_instance * timeri,
			   int event,
			   struct timespec * tstamp,
			   unsigned long resolution);
	void (*disconnect)(struct snd_timer_instance *timeri);
	void *callback_data;
	unsigned long ticks;		
	unsigned long cticks;		
	unsigned long pticks;		
	unsigned long resolution;	
	unsigned long lost;		
	int slave_class;
	unsigned int slave_id;
	struct list_head open_list;
	struct list_head active_list;
	struct list_head ack_list;
	struct list_head slave_list_head;
	struct list_head slave_active_head;
	struct snd_timer_instance *master;
};



int snd_timer_new(struct snd_card *card, char *id, struct snd_timer_id *tid, struct snd_timer **rtimer);
void snd_timer_notify(struct snd_timer *timer, int event, struct timespec *tstamp);
int snd_timer_global_new(char *id, int device, struct snd_timer **rtimer);
int snd_timer_global_free(struct snd_timer *timer);
int snd_timer_global_register(struct snd_timer *timer);

int snd_timer_open(struct snd_timer_instance **ti, char *owner, struct snd_timer_id *tid, unsigned int slave_id);
int snd_timer_close(struct snd_timer_instance *timeri);
unsigned long snd_timer_resolution(struct snd_timer_instance *timeri);
int snd_timer_start(struct snd_timer_instance *timeri, unsigned int ticks);
int snd_timer_stop(struct snd_timer_instance *timeri);
int snd_timer_continue(struct snd_timer_instance *timeri);
int snd_timer_pause(struct snd_timer_instance *timeri);

void snd_timer_interrupt(struct snd_timer *timer, unsigned long ticks_left);

#endif 
