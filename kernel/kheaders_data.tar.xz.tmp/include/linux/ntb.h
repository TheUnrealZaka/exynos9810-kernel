

#ifndef _NTB_H_
#define _NTB_H_

#include <linux/completion.h>
#include <linux/device.h>

struct ntb_client;
struct ntb_dev;
struct pci_dev;


enum ntb_topo {
	NTB_TOPO_NONE = -1,
	NTB_TOPO_PRI,
	NTB_TOPO_SEC,
	NTB_TOPO_B2B_USD,
	NTB_TOPO_B2B_DSD,
};

static inline int ntb_topo_is_b2b(enum ntb_topo topo)
{
	switch ((int)topo) {
	case NTB_TOPO_B2B_USD:
	case NTB_TOPO_B2B_DSD:
		return 1;
	}
	return 0;
}

static inline char *ntb_topo_string(enum ntb_topo topo)
{
	switch (topo) {
	case NTB_TOPO_NONE:	return "NTB_TOPO_NONE";
	case NTB_TOPO_PRI:	return "NTB_TOPO_PRI";
	case NTB_TOPO_SEC:	return "NTB_TOPO_SEC";
	case NTB_TOPO_B2B_USD:	return "NTB_TOPO_B2B_USD";
	case NTB_TOPO_B2B_DSD:	return "NTB_TOPO_B2B_DSD";
	}
	return "NTB_TOPO_INVALID";
}


enum ntb_speed {
	NTB_SPEED_AUTO = -1,
	NTB_SPEED_NONE = 0,
	NTB_SPEED_GEN1 = 1,
	NTB_SPEED_GEN2 = 2,
	NTB_SPEED_GEN3 = 3,
};


enum ntb_width {
	NTB_WIDTH_AUTO = -1,
	NTB_WIDTH_NONE = 0,
	NTB_WIDTH_1 = 1,
	NTB_WIDTH_2 = 2,
	NTB_WIDTH_4 = 4,
	NTB_WIDTH_8 = 8,
	NTB_WIDTH_12 = 12,
	NTB_WIDTH_16 = 16,
	NTB_WIDTH_32 = 32,
};


struct ntb_client_ops {
	int (*probe)(struct ntb_client *client, struct ntb_dev *ntb);
	void (*remove)(struct ntb_client *client, struct ntb_dev *ntb);
};

static inline int ntb_client_ops_is_valid(const struct ntb_client_ops *ops)
{
	
	return
		ops->probe			&&
		ops->remove			&&
		1;
}


struct ntb_ctx_ops {
	void (*link_event)(void *ctx);
	void (*db_event)(void *ctx, int db_vector);
};

static inline int ntb_ctx_ops_is_valid(const struct ntb_ctx_ops *ops)
{
	
	return
		
		
		1;
}


struct ntb_dev_ops {
	int (*mw_count)(struct ntb_dev *ntb);
	int (*mw_get_range)(struct ntb_dev *ntb, int idx,
			    phys_addr_t *base, resource_size_t *size,
			resource_size_t *align, resource_size_t *align_size);
	int (*mw_set_trans)(struct ntb_dev *ntb, int idx,
			    dma_addr_t addr, resource_size_t size);
	int (*mw_clear_trans)(struct ntb_dev *ntb, int idx);

	int (*link_is_up)(struct ntb_dev *ntb,
			  enum ntb_speed *speed, enum ntb_width *width);
	int (*link_enable)(struct ntb_dev *ntb,
			   enum ntb_speed max_speed, enum ntb_width max_width);
	int (*link_disable)(struct ntb_dev *ntb);

	int (*db_is_unsafe)(struct ntb_dev *ntb);
	u64 (*db_valid_mask)(struct ntb_dev *ntb);
	int (*db_vector_count)(struct ntb_dev *ntb);
	u64 (*db_vector_mask)(struct ntb_dev *ntb, int db_vector);

	u64 (*db_read)(struct ntb_dev *ntb);
	int (*db_set)(struct ntb_dev *ntb, u64 db_bits);
	int (*db_clear)(struct ntb_dev *ntb, u64 db_bits);

	u64 (*db_read_mask)(struct ntb_dev *ntb);
	int (*db_set_mask)(struct ntb_dev *ntb, u64 db_bits);
	int (*db_clear_mask)(struct ntb_dev *ntb, u64 db_bits);

	int (*peer_db_addr)(struct ntb_dev *ntb,
			    phys_addr_t *db_addr, resource_size_t *db_size);
	u64 (*peer_db_read)(struct ntb_dev *ntb);
	int (*peer_db_set)(struct ntb_dev *ntb, u64 db_bits);
	int (*peer_db_clear)(struct ntb_dev *ntb, u64 db_bits);

	u64 (*peer_db_read_mask)(struct ntb_dev *ntb);
	int (*peer_db_set_mask)(struct ntb_dev *ntb, u64 db_bits);
	int (*peer_db_clear_mask)(struct ntb_dev *ntb, u64 db_bits);

	int (*spad_is_unsafe)(struct ntb_dev *ntb);
	int (*spad_count)(struct ntb_dev *ntb);

	u32 (*spad_read)(struct ntb_dev *ntb, int idx);
	int (*spad_write)(struct ntb_dev *ntb, int idx, u32 val);

	int (*peer_spad_addr)(struct ntb_dev *ntb, int idx,
			      phys_addr_t *spad_addr);
	u32 (*peer_spad_read)(struct ntb_dev *ntb, int idx);
	int (*peer_spad_write)(struct ntb_dev *ntb, int idx, u32 val);
};

static inline int ntb_dev_ops_is_valid(const struct ntb_dev_ops *ops)
{
	
	return
		ops->mw_count				&&
		ops->mw_get_range			&&
		ops->mw_set_trans			&&
		
		ops->link_is_up				&&
		ops->link_enable			&&
		ops->link_disable			&&
		
		ops->db_valid_mask			&&

		
		(!ops->db_vector_count == !ops->db_vector_mask) &&

		ops->db_read				&&
		
		ops->db_clear				&&
		
		ops->db_set_mask			&&
		ops->db_clear_mask			&&
		
		
		ops->peer_db_set			&&
		
		
		
		
		
		ops->spad_count				&&
		ops->spad_read				&&
		ops->spad_write				&&
		
		
		ops->peer_spad_write			&&
		1;
}


struct ntb_client {
	struct device_driver		drv;
	const struct ntb_client_ops	ops;
};

#define drv_ntb_client(__drv) container_of((__drv), struct ntb_client, drv)


struct ntb_dev {
	struct device			dev;
	struct pci_dev			*pdev;
	enum ntb_topo			topo;
	const struct ntb_dev_ops	*ops;
	void				*ctx;
	const struct ntb_ctx_ops	*ctx_ops;

	

	
	spinlock_t			ctx_lock;
	
	struct completion		released;
};

#define dev_ntb(__dev) container_of((__dev), struct ntb_dev, dev)


#define ntb_register_client(client) \
	__ntb_register_client((client), THIS_MODULE, KBUILD_MODNAME)

int __ntb_register_client(struct ntb_client *client, struct module *mod,
			  const char *mod_name);


void ntb_unregister_client(struct ntb_client *client);

#define module_ntb_client(__ntb_client) \
	module_driver(__ntb_client, ntb_register_client, \
			ntb_unregister_client)


int ntb_register_device(struct ntb_dev *ntb);


void ntb_unregister_device(struct ntb_dev *ntb);


int ntb_set_ctx(struct ntb_dev *ntb, void *ctx,
		const struct ntb_ctx_ops *ctx_ops);


void ntb_clear_ctx(struct ntb_dev *ntb);


void ntb_link_event(struct ntb_dev *ntb);


void ntb_db_event(struct ntb_dev *ntb, int vector);


static inline int ntb_mw_count(struct ntb_dev *ntb)
{
	return ntb->ops->mw_count(ntb);
}


static inline int ntb_mw_get_range(struct ntb_dev *ntb, int idx,
				   phys_addr_t *base, resource_size_t *size,
		resource_size_t *align, resource_size_t *align_size)
{
	return ntb->ops->mw_get_range(ntb, idx, base, size,
			align, align_size);
}


static inline int ntb_mw_set_trans(struct ntb_dev *ntb, int idx,
				   dma_addr_t addr, resource_size_t size)
{
	return ntb->ops->mw_set_trans(ntb, idx, addr, size);
}


static inline int ntb_mw_clear_trans(struct ntb_dev *ntb, int idx)
{
	if (!ntb->ops->mw_clear_trans)
		return ntb->ops->mw_set_trans(ntb, idx, 0, 0);

	return ntb->ops->mw_clear_trans(ntb, idx);
}


static inline int ntb_link_is_up(struct ntb_dev *ntb,
				 enum ntb_speed *speed, enum ntb_width *width)
{
	return ntb->ops->link_is_up(ntb, speed, width);
}


static inline int ntb_link_enable(struct ntb_dev *ntb,
				  enum ntb_speed max_speed,
				  enum ntb_width max_width)
{
	return ntb->ops->link_enable(ntb, max_speed, max_width);
}


static inline int ntb_link_disable(struct ntb_dev *ntb)
{
	return ntb->ops->link_disable(ntb);
}


static inline int ntb_db_is_unsafe(struct ntb_dev *ntb)
{
	if (!ntb->ops->db_is_unsafe)
		return 0;

	return ntb->ops->db_is_unsafe(ntb);
}


static inline u64 ntb_db_valid_mask(struct ntb_dev *ntb)
{
	return ntb->ops->db_valid_mask(ntb);
}


static inline int ntb_db_vector_count(struct ntb_dev *ntb)
{
	if (!ntb->ops->db_vector_count)
		return 1;

	return ntb->ops->db_vector_count(ntb);
}


static inline u64 ntb_db_vector_mask(struct ntb_dev *ntb, int vector)
{
	if (!ntb->ops->db_vector_mask)
		return ntb_db_valid_mask(ntb);

	return ntb->ops->db_vector_mask(ntb, vector);
}


static inline u64 ntb_db_read(struct ntb_dev *ntb)
{
	return ntb->ops->db_read(ntb);
}


static inline int ntb_db_set(struct ntb_dev *ntb, u64 db_bits)
{
	if (!ntb->ops->db_set)
		return -EINVAL;

	return ntb->ops->db_set(ntb, db_bits);
}


static inline int ntb_db_clear(struct ntb_dev *ntb, u64 db_bits)
{
	return ntb->ops->db_clear(ntb, db_bits);
}


static inline u64 ntb_db_read_mask(struct ntb_dev *ntb)
{
	if (!ntb->ops->db_read_mask)
		return 0;

	return ntb->ops->db_read_mask(ntb);
}


static inline int ntb_db_set_mask(struct ntb_dev *ntb, u64 db_bits)
{
	return ntb->ops->db_set_mask(ntb, db_bits);
}


static inline int ntb_db_clear_mask(struct ntb_dev *ntb, u64 db_bits)
{
	return ntb->ops->db_clear_mask(ntb, db_bits);
}


static inline int ntb_peer_db_addr(struct ntb_dev *ntb,
				   phys_addr_t *db_addr,
				   resource_size_t *db_size)
{
	if (!ntb->ops->peer_db_addr)
		return -EINVAL;

	return ntb->ops->peer_db_addr(ntb, db_addr, db_size);
}


static inline u64 ntb_peer_db_read(struct ntb_dev *ntb)
{
	if (!ntb->ops->peer_db_read)
		return 0;

	return ntb->ops->peer_db_read(ntb);
}


static inline int ntb_peer_db_set(struct ntb_dev *ntb, u64 db_bits)
{
	return ntb->ops->peer_db_set(ntb, db_bits);
}


static inline int ntb_peer_db_clear(struct ntb_dev *ntb, u64 db_bits)
{
	if (!ntb->ops->db_clear)
		return -EINVAL;

	return ntb->ops->peer_db_clear(ntb, db_bits);
}


static inline u64 ntb_peer_db_read_mask(struct ntb_dev *ntb)
{
	if (!ntb->ops->db_read_mask)
		return 0;

	return ntb->ops->peer_db_read_mask(ntb);
}


static inline int ntb_peer_db_set_mask(struct ntb_dev *ntb, u64 db_bits)
{
	if (!ntb->ops->db_set_mask)
		return -EINVAL;

	return ntb->ops->peer_db_set_mask(ntb, db_bits);
}


static inline int ntb_peer_db_clear_mask(struct ntb_dev *ntb, u64 db_bits)
{
	if (!ntb->ops->db_clear_mask)
		return -EINVAL;

	return ntb->ops->peer_db_clear_mask(ntb, db_bits);
}


static inline int ntb_spad_is_unsafe(struct ntb_dev *ntb)
{
	if (!ntb->ops->spad_is_unsafe)
		return 0;

	return ntb->ops->spad_is_unsafe(ntb);
}


static inline int ntb_spad_count(struct ntb_dev *ntb)
{
	return ntb->ops->spad_count(ntb);
}


static inline u32 ntb_spad_read(struct ntb_dev *ntb, int idx)
{
	return ntb->ops->spad_read(ntb, idx);
}


static inline int ntb_spad_write(struct ntb_dev *ntb, int idx, u32 val)
{
	return ntb->ops->spad_write(ntb, idx, val);
}


static inline int ntb_peer_spad_addr(struct ntb_dev *ntb, int idx,
				     phys_addr_t *spad_addr)
{
	if (!ntb->ops->peer_spad_addr)
		return -EINVAL;

	return ntb->ops->peer_spad_addr(ntb, idx, spad_addr);
}


static inline u32 ntb_peer_spad_read(struct ntb_dev *ntb, int idx)
{
	return ntb->ops->peer_spad_read(ntb, idx);
}


static inline int ntb_peer_spad_write(struct ntb_dev *ntb, int idx, u32 val)
{
	return ntb->ops->peer_spad_write(ntb, idx, val);
}

#endif
