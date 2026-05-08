

#include <linux/types.h>
#include <linux/slab.h>
#include <asm/debug.h>


#define IUCV_IPRMDATA	0x80
#define IUCV_IPQUSCE	0x40
#define IUCV_IPBUFLST	0x40
#define IUCV_IPPRTY	0x20
#define IUCV_IPANSLST	0x08
#define IUCV_IPSYNC	0x04
#define IUCV_IPLOCAL	0x01


struct iucv_array {
	u32 address;
	u32 length;
} __attribute__ ((aligned (8)));

extern struct bus_type iucv_bus;
extern struct device *iucv_root;


struct iucv_path {
	u16 pathid;
	u16 msglim;
	u8  flags;
	void *private;
	struct iucv_handler *handler;
	struct list_head list;
};


struct iucv_message {
	u32 id;
	u32 audit;
	u32 class;
	u32 tag;
	u32 length;
	u32 reply_size;
	u8  rmmsg[8];
	u8  flags;
} __packed;


struct iucv_handler {
	 
	int  (*path_pending)(struct iucv_path *, u8 *ipvmid, u8 *ipuser);
	
	void (*path_complete)(struct iucv_path *, u8 *ipuser);
	 
	void (*path_severed)(struct iucv_path *, u8 *ipuser);
	
	void (*path_quiesced)(struct iucv_path *, u8 *ipuser);
	
	void (*path_resumed)(struct iucv_path *, u8 *ipuser);
	
	void (*message_pending)(struct iucv_path *, struct iucv_message *);
	
	void (*message_complete)(struct iucv_path *, struct iucv_message *);

	struct list_head list;
	struct list_head paths;
};


int iucv_register(struct iucv_handler *handler, int smp);


void iucv_unregister(struct iucv_handler *handle, int smp);


static inline struct iucv_path *iucv_path_alloc(u16 msglim, u8 flags, gfp_t gfp)
{
	struct iucv_path *path;

	path = kzalloc(sizeof(struct iucv_path), gfp);
	if (path) {
		path->msglim = msglim;
		path->flags = flags;
	}
	return path;
}


static inline void iucv_path_free(struct iucv_path *path)
{
	kfree(path);
}


int iucv_path_accept(struct iucv_path *path, struct iucv_handler *handler,
		     u8 *userdata, void *private);


int iucv_path_connect(struct iucv_path *path, struct iucv_handler *handler,
		      u8 *userid, u8 *system, u8 *userdata,
		      void *private);


int iucv_path_quiesce(struct iucv_path *path, u8 *userdata);


int iucv_path_resume(struct iucv_path *path, u8 *userdata);


int iucv_path_sever(struct iucv_path *path, u8 *userdata);


int iucv_message_purge(struct iucv_path *path, struct iucv_message *msg,
		       u32 srccls);


int iucv_message_receive(struct iucv_path *path, struct iucv_message *msg,
			 u8 flags, void *buffer, size_t size, size_t *residual);


int __iucv_message_receive(struct iucv_path *path, struct iucv_message *msg,
			   u8 flags, void *buffer, size_t size,
			   size_t *residual);


int iucv_message_reject(struct iucv_path *path, struct iucv_message *msg);


int iucv_message_reply(struct iucv_path *path, struct iucv_message *msg,
		       u8 flags, void *reply, size_t size);


int iucv_message_send(struct iucv_path *path, struct iucv_message *msg,
		      u8 flags, u32 srccls, void *buffer, size_t size);


int __iucv_message_send(struct iucv_path *path, struct iucv_message *msg,
			u8 flags, u32 srccls, void *buffer, size_t size);


int iucv_message_send2way(struct iucv_path *path, struct iucv_message *msg,
			  u8 flags, u32 srccls, void *buffer, size_t size,
			  void *answer, size_t asize, size_t *residual);

struct iucv_interface {
	int (*message_receive)(struct iucv_path *path, struct iucv_message *msg,
		u8 flags, void *buffer, size_t size, size_t *residual);
	int (*__message_receive)(struct iucv_path *path,
		struct iucv_message *msg, u8 flags, void *buffer, size_t size,
		size_t *residual);
	int (*message_reply)(struct iucv_path *path, struct iucv_message *msg,
		u8 flags, void *reply, size_t size);
	int (*message_reject)(struct iucv_path *path, struct iucv_message *msg);
	int (*message_send)(struct iucv_path *path, struct iucv_message *msg,
		u8 flags, u32 srccls, void *buffer, size_t size);
	int (*__message_send)(struct iucv_path *path, struct iucv_message *msg,
		u8 flags, u32 srccls, void *buffer, size_t size);
	int (*message_send2way)(struct iucv_path *path,
		struct iucv_message *msg, u8 flags, u32 srccls, void *buffer,
		size_t size, void *answer, size_t asize, size_t *residual);
	int (*message_purge)(struct iucv_path *path, struct iucv_message *msg,
		u32 srccls);
	int (*path_accept)(struct iucv_path *path, struct iucv_handler *handler,
		u8 userdata[16], void *private);
	int (*path_connect)(struct iucv_path *path,
		struct iucv_handler *handler,
		u8 userid[8], u8 system[8], u8 userdata[16], void *private);
	int (*path_quiesce)(struct iucv_path *path, u8 userdata[16]);
	int (*path_resume)(struct iucv_path *path, u8 userdata[16]);
	int (*path_sever)(struct iucv_path *path, u8 userdata[16]);
	int (*iucv_register)(struct iucv_handler *handler, int smp);
	void (*iucv_unregister)(struct iucv_handler *handler, int smp);
	struct bus_type *bus;
	struct device *root;
};

extern struct iucv_interface iucv_if;
