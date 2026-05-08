#ifndef __SOUND_CONTROL_H
#define __SOUND_CONTROL_H



#include <linux/nospec.h>
#include <sound/asound.h>

#define snd_kcontrol_chip(kcontrol) ((kcontrol)->private_data)

struct snd_kcontrol;
typedef int (snd_kcontrol_info_t) (struct snd_kcontrol * kcontrol, struct snd_ctl_elem_info * uinfo);
typedef int (snd_kcontrol_get_t) (struct snd_kcontrol * kcontrol, struct snd_ctl_elem_value * ucontrol);
typedef int (snd_kcontrol_put_t) (struct snd_kcontrol * kcontrol, struct snd_ctl_elem_value * ucontrol);
typedef int (snd_kcontrol_tlv_rw_t)(struct snd_kcontrol *kcontrol,
				    int op_flag, 
				    unsigned int size,
				    unsigned int __user *tlv);

enum {
	SNDRV_CTL_TLV_OP_READ = 0,
	SNDRV_CTL_TLV_OP_WRITE = 1,
	SNDRV_CTL_TLV_OP_CMD = -1,
};

struct snd_kcontrol_new {
	snd_ctl_elem_iface_t iface;	
	unsigned int device;		
	unsigned int subdevice;		
	const unsigned char *name;	
	unsigned int index;		
	unsigned int access;		
	unsigned int count;		
	snd_kcontrol_info_t *info;
	snd_kcontrol_get_t *get;
	snd_kcontrol_put_t *put;
	union {
		snd_kcontrol_tlv_rw_t *c;
		const unsigned int *p;
	} tlv;
	unsigned long private_value;
};

struct snd_kcontrol_volatile {
	struct snd_ctl_file *owner;	
	unsigned int access;	
};

struct snd_kcontrol {
	struct list_head list;		
	struct snd_ctl_elem_id id;
	unsigned int count;		
	snd_kcontrol_info_t *info;
	snd_kcontrol_get_t *get;
	snd_kcontrol_put_t *put;
	union {
		snd_kcontrol_tlv_rw_t *c;
		const unsigned int *p;
	} tlv;
	unsigned long private_value;
	void *private_data;
	void (*private_free)(struct snd_kcontrol *kcontrol);
	struct snd_kcontrol_volatile vd[0];	
};

#define snd_kcontrol(n) list_entry(n, struct snd_kcontrol, list)

struct snd_kctl_event {
	struct list_head list;	
	struct snd_ctl_elem_id id;
	unsigned int mask;
};

#define snd_kctl_event(n) list_entry(n, struct snd_kctl_event, list)

struct pid;

enum {
	SND_CTL_SUBDEV_PCM,
	SND_CTL_SUBDEV_RAWMIDI,
	SND_CTL_SUBDEV_ITEMS,
};

struct snd_ctl_file {
	struct list_head list;		
	struct snd_card *card;
	struct pid *pid;
	int preferred_subdevice[SND_CTL_SUBDEV_ITEMS];
	wait_queue_head_t change_sleep;
	spinlock_t read_lock;
	struct fasync_struct *fasync;
	int subscribed;			
	struct list_head events;	
};

#define snd_ctl_file(n) list_entry(n, struct snd_ctl_file, list)

typedef int (*snd_kctl_ioctl_func_t) (struct snd_card * card,
				      struct snd_ctl_file * control,
				      unsigned int cmd, unsigned long arg);

void snd_ctl_notify(struct snd_card * card, unsigned int mask, struct snd_ctl_elem_id * id);

struct snd_kcontrol *snd_ctl_new1(const struct snd_kcontrol_new * kcontrolnew, void * private_data);
void snd_ctl_free_one(struct snd_kcontrol * kcontrol);
int snd_ctl_add(struct snd_card * card, struct snd_kcontrol * kcontrol);
int snd_ctl_remove(struct snd_card * card, struct snd_kcontrol * kcontrol);
int snd_ctl_replace(struct snd_card *card, struct snd_kcontrol *kcontrol, bool add_on_replace);
int snd_ctl_remove_id(struct snd_card * card, struct snd_ctl_elem_id *id);
int snd_ctl_rename_id(struct snd_card * card, struct snd_ctl_elem_id *src_id, struct snd_ctl_elem_id *dst_id);
int snd_ctl_activate_id(struct snd_card *card, struct snd_ctl_elem_id *id,
			int active);
struct snd_kcontrol *snd_ctl_find_numid(struct snd_card * card, unsigned int numid);
struct snd_kcontrol *snd_ctl_find_id(struct snd_card * card, struct snd_ctl_elem_id *id);

int snd_ctl_create(struct snd_card *card);

int snd_ctl_register_ioctl(snd_kctl_ioctl_func_t fcn);
int snd_ctl_unregister_ioctl(snd_kctl_ioctl_func_t fcn);
#ifdef CONFIG_COMPAT
int snd_ctl_register_ioctl_compat(snd_kctl_ioctl_func_t fcn);
int snd_ctl_unregister_ioctl_compat(snd_kctl_ioctl_func_t fcn);
#else
#define snd_ctl_register_ioctl_compat(fcn)
#define snd_ctl_unregister_ioctl_compat(fcn)
#endif

int snd_ctl_get_preferred_subdevice(struct snd_card *card, int type);

static inline unsigned int snd_ctl_get_ioffnum(struct snd_kcontrol *kctl, struct snd_ctl_elem_id *id)
{
	unsigned int ioff = id->numid - kctl->id.numid;
	return array_index_nospec(ioff, kctl->count);
}

static inline unsigned int snd_ctl_get_ioffidx(struct snd_kcontrol *kctl, struct snd_ctl_elem_id *id)
{
	unsigned int ioff = id->index - kctl->id.index;
	return array_index_nospec(ioff, kctl->count);
}

static inline unsigned int snd_ctl_get_ioff(struct snd_kcontrol *kctl, struct snd_ctl_elem_id *id)
{
	if (id->numid) {
		return snd_ctl_get_ioffnum(kctl, id);
	} else {
		return snd_ctl_get_ioffidx(kctl, id);
	}
}

static inline struct snd_ctl_elem_id *snd_ctl_build_ioff(struct snd_ctl_elem_id *dst_id,
						    struct snd_kcontrol *src_kctl,
						    unsigned int offset)
{
	*dst_id = src_kctl->id;
	dst_id->index += offset;
	dst_id->numid += offset;
	return dst_id;
}


int snd_ctl_boolean_mono_info(struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_info *uinfo);
int snd_ctl_boolean_stereo_info(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_info *uinfo);
int snd_ctl_enum_info(struct snd_ctl_elem_info *info, unsigned int channels,
		      unsigned int items, const char *const names[]);


struct snd_kcontrol *snd_ctl_make_virtual_master(char *name,
						 const unsigned int *tlv);
int _snd_ctl_add_slave(struct snd_kcontrol *master, struct snd_kcontrol *slave,
		       unsigned int flags);

#define SND_CTL_SLAVE_NEED_UPDATE	(1 << 0)


static inline int
snd_ctl_add_slave(struct snd_kcontrol *master, struct snd_kcontrol *slave)
{
	return _snd_ctl_add_slave(master, slave, 0);
}


static inline int
snd_ctl_add_slave_uncached(struct snd_kcontrol *master,
			   struct snd_kcontrol *slave)
{
	return _snd_ctl_add_slave(master, slave, SND_CTL_SLAVE_NEED_UPDATE);
}

int snd_ctl_add_vmaster_hook(struct snd_kcontrol *kctl,
			     void (*hook)(void *private_data, int),
			     void *private_data);
void snd_ctl_sync_vmaster(struct snd_kcontrol *kctl, bool hook_only);
#define snd_ctl_sync_vmaster_hook(kctl)	snd_ctl_sync_vmaster(kctl, true)


struct snd_kcontrol *
snd_kctl_jack_new(const char *name, struct snd_card *card);
void snd_kctl_jack_report(struct snd_card *card,
			  struct snd_kcontrol *kctl, bool status);

#endif	
