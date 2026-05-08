
#ifndef __SCIF_H__
#define __SCIF_H__

#include <linux/types.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/scif_ioctl.h>

#define SCIF_ACCEPT_SYNC	1
#define SCIF_SEND_BLOCK		1
#define SCIF_RECV_BLOCK		1

enum {
	SCIF_PROT_READ = (1 << 0),
	SCIF_PROT_WRITE = (1 << 1)
};

enum {
	SCIF_MAP_FIXED = 0x10,
	SCIF_MAP_KERNEL	= 0x20,
};

enum {
	SCIF_FENCE_INIT_SELF = (1 << 0),
	SCIF_FENCE_INIT_PEER = (1 << 1),
	SCIF_SIGNAL_LOCAL = (1 << 4),
	SCIF_SIGNAL_REMOTE = (1 << 5)
};

enum {
	SCIF_RMA_USECPU = (1 << 0),
	SCIF_RMA_USECACHE = (1 << 1),
	SCIF_RMA_SYNC = (1 << 2),
	SCIF_RMA_ORDERED = (1 << 3)
};


#define SCIF_ADMIN_PORT_END	1024


#define SCIF_PORT_RSVD		1088

typedef struct scif_endpt *scif_epd_t;
typedef struct scif_pinned_pages *scif_pinned_pages_t;


struct scif_range {
	void *cookie;
	int nr_pages;
	int prot_flags;
	dma_addr_t *phys_addr;
	void __iomem **va;
};


struct scif_pollepd {
	scif_epd_t epd;
	short events;
	short revents;
};


struct scif_peer_dev {
	struct device dev;
	u8 dnode;
};


struct scif_client {
	const char *name;
	void (*probe)(struct scif_peer_dev *spdev);
	void (*remove)(struct scif_peer_dev *spdev);
	struct subsys_interface si;
};

#define SCIF_OPEN_FAILED ((scif_epd_t)-1)
#define SCIF_REGISTER_FAILED ((off_t)-1)
#define SCIF_MMAP_FAILED ((void *)-1)


scif_epd_t scif_open(void);


int scif_bind(scif_epd_t epd, u16 pn);


int scif_listen(scif_epd_t epd, int backlog);


int scif_connect(scif_epd_t epd, struct scif_port_id *dst);


int scif_accept(scif_epd_t epd, struct scif_port_id *peer, scif_epd_t
		*newepd, int flags);


int scif_close(scif_epd_t epd);


int scif_send(scif_epd_t epd, void *msg, int len, int flags);


int scif_recv(scif_epd_t epd, void *msg, int len, int flags);


off_t scif_register(scif_epd_t epd, void *addr, size_t len, off_t offset,
		    int prot_flags, int map_flags);


int scif_unregister(scif_epd_t epd, off_t offset, size_t len);


int scif_readfrom(scif_epd_t epd, off_t loffset, size_t len, off_t
		  roffset, int rma_flags);


int scif_writeto(scif_epd_t epd, off_t loffset, size_t len, off_t
		 roffset, int rma_flags);


int scif_vreadfrom(scif_epd_t epd, void *addr, size_t len, off_t roffset,
		   int rma_flags);


int scif_vwriteto(scif_epd_t epd, void *addr, size_t len, off_t roffset,
		  int rma_flags);


int scif_fence_mark(scif_epd_t epd, int flags, int *mark);


int scif_fence_wait(scif_epd_t epd, int mark);


int scif_fence_signal(scif_epd_t epd, off_t loff, u64 lval, off_t roff,
		      u64 rval, int flags);


int scif_get_node_ids(u16 *nodes, int len, u16 *self);


int scif_pin_pages(void *addr, size_t len, int prot_flags, int map_flags,
		   scif_pinned_pages_t *pinned_pages);


int scif_unpin_pages(scif_pinned_pages_t pinned_pages);


off_t scif_register_pinned_pages(scif_epd_t epd,
				 scif_pinned_pages_t pinned_pages,
				 off_t offset, int map_flags);


int scif_get_pages(scif_epd_t epd, off_t offset, size_t len,
		   struct scif_range **pages);


int scif_put_pages(struct scif_range *pages);


int scif_poll(struct scif_pollepd *epds, unsigned int nepds, long timeout);


int scif_client_register(struct scif_client *client);


void scif_client_unregister(struct scif_client *client);

#endif 
