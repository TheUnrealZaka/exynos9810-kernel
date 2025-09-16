#ifndef _FS_CEPH_LIBCEPH_H
#define _FS_CEPH_LIBCEPH_H

#include <linux/ceph/ceph_debug.h>

#include <asm/unaligned.h>
#include <linux/backing-dev.h>
#include <linux/completion.h>
#include <linux/exportfs.h>
#include <linux/bug.h>
#include <linux/fs.h>
#include <linux/mempool.h>
#include <linux/pagemap.h>
#include <linux/wait.h>
#include <linux/writeback.h>
#include <linux/slab.h>

#include <linux/ceph/types.h>
#include <linux/ceph/messenger.h>
#include <linux/ceph/msgpool.h>
#include <linux/ceph/mon_client.h>
#include <linux/ceph/osd_client.h>
#include <linux/ceph/ceph_fs.h>
#include <linux/ceph/string_table.h>


#define CEPH_OPT_FSID             (1<<0)
#define CEPH_OPT_NOSHARE          (1<<1) 
#define CEPH_OPT_MYIP             (1<<2) 
#define CEPH_OPT_NOCRC            (1<<3) 
#define CEPH_OPT_NOMSGAUTH	  (1<<4) 
#define CEPH_OPT_TCP_NODELAY	  (1<<5) 
#define CEPH_OPT_NOMSGSIGN	  (1<<6) 

#define CEPH_OPT_DEFAULT   (CEPH_OPT_TCP_NODELAY)

#define ceph_set_opt(client, opt) \
	(client)->options->flags |= CEPH_OPT_##opt;
#define ceph_test_opt(client, opt) \
	(!!((client)->options->flags & CEPH_OPT_##opt))

struct ceph_options {
	int flags;
	struct ceph_fsid fsid;
	struct ceph_entity_addr my_addr;
	unsigned long mount_timeout;		
	unsigned long osd_idle_ttl;		
	unsigned long osd_keepalive_timeout;	

	

	struct ceph_entity_addr *mon_addr; 
	int num_mon;
	char *name;
	struct ceph_crypto_key *key;
};


#define CEPH_MOUNT_TIMEOUT_DEFAULT	msecs_to_jiffies(60 * 1000)
#define CEPH_OSD_KEEPALIVE_DEFAULT	msecs_to_jiffies(5 * 1000)
#define CEPH_OSD_IDLE_TTL_DEFAULT	msecs_to_jiffies(60 * 1000)

#define CEPH_MONC_HUNT_INTERVAL		msecs_to_jiffies(3 * 1000)
#define CEPH_MONC_PING_INTERVAL		msecs_to_jiffies(10 * 1000)
#define CEPH_MONC_PING_TIMEOUT		msecs_to_jiffies(30 * 1000)
#define CEPH_MONC_HUNT_BACKOFF		2
#define CEPH_MONC_HUNT_MAX_MULT		10

#define CEPH_MSG_MAX_FRONT_LEN	(16*1024*1024)
#define CEPH_MSG_MAX_MIDDLE_LEN	(16*1024*1024)


#define CEPH_MSG_MAX_DATA_LEN	(32*1024*1024)

#define CEPH_AUTH_NAME_DEFAULT   "guest"


#define CEPH_CAPS_WANTED_DELAY_MIN_DEFAULT      5  
#define CEPH_CAPS_WANTED_DELAY_MAX_DEFAULT     60  

#define CEPH_CAP_RELEASE_SAFETY_DEFAULT        (CEPH_CAPS_PER_RELEASE * 4)


enum {
	CEPH_MOUNT_MOUNTING,
	CEPH_MOUNT_MOUNTED,
	CEPH_MOUNT_UNMOUNTING,
	CEPH_MOUNT_UNMOUNTED,
	CEPH_MOUNT_SHUTDOWN,
};

static inline unsigned long ceph_timeout_jiffies(unsigned long timeout)
{
	return timeout ?: MAX_SCHEDULE_TIMEOUT;
}

struct ceph_mds_client;


struct ceph_client {
	struct ceph_fsid fsid;
	bool have_fsid;

	void *private;

	struct ceph_options *options;

	struct mutex mount_mutex;      
	wait_queue_head_t auth_wq;
	int auth_err;

	int (*extra_mon_dispatch)(struct ceph_client *, struct ceph_msg *);

	u64 supported_features;
	u64 required_features;

	struct ceph_messenger msgr;   
	struct ceph_mon_client monc;
	struct ceph_osd_client osdc;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dir;
	struct dentry *debugfs_monmap;
	struct dentry *debugfs_osdmap;
	struct dentry *debugfs_options;
#endif
};

#define from_msgr(ms)	container_of(ms, struct ceph_client, msgr)





struct ceph_snap_context {
	atomic_t nref;
	u64 seq;
	u32 num_snaps;
	u64 snaps[];
};

extern struct ceph_snap_context *ceph_create_snap_context(u32 snap_count,
					gfp_t gfp_flags);
extern struct ceph_snap_context *ceph_get_snap_context(
					struct ceph_snap_context *sc);
extern void ceph_put_snap_context(struct ceph_snap_context *sc);


static inline int calc_pages_for(u64 off, u64 len)
{
	return ((off+len+PAGE_SIZE-1) >> PAGE_SHIFT) -
		(off >> PAGE_SHIFT);
}


#define DEFINE_RB_INSDEL_FUNCS(name, type, keyfld, nodefld)		\
static void insert_##name(struct rb_root *root, type *t)		\
{									\
	struct rb_node **n = &root->rb_node;				\
	struct rb_node *parent = NULL;					\
									\
	BUG_ON(!RB_EMPTY_NODE(&t->nodefld));				\
									\
	while (*n) {							\
		type *cur = rb_entry(*n, type, nodefld);		\
									\
		parent = *n;						\
		if (t->keyfld < cur->keyfld)				\
			n = &(*n)->rb_left;				\
		else if (t->keyfld > cur->keyfld)			\
			n = &(*n)->rb_right;				\
		else							\
			BUG();						\
	}								\
									\
	rb_link_node(&t->nodefld, parent, n);				\
	rb_insert_color(&t->nodefld, root);				\
}									\
static void erase_##name(struct rb_root *root, type *t)			\
{									\
	BUG_ON(RB_EMPTY_NODE(&t->nodefld));				\
	rb_erase(&t->nodefld, root);					\
	RB_CLEAR_NODE(&t->nodefld);					\
}

#define DEFINE_RB_LOOKUP_FUNC(name, type, keyfld, nodefld)		\
extern type __lookup_##name##_key;					\
static type *lookup_##name(struct rb_root *root,			\
			   typeof(__lookup_##name##_key.keyfld) key)	\
{									\
	struct rb_node *n = root->rb_node;				\
									\
	while (n) {							\
		type *cur = rb_entry(n, type, nodefld);			\
									\
		if (key < cur->keyfld)					\
			n = n->rb_left;					\
		else if (key > cur->keyfld)				\
			n = n->rb_right;				\
		else							\
			return cur;					\
	}								\
									\
	return NULL;							\
}

#define DEFINE_RB_FUNCS(name, type, keyfld, nodefld)			\
DEFINE_RB_INSDEL_FUNCS(name, type, keyfld, nodefld)			\
DEFINE_RB_LOOKUP_FUNC(name, type, keyfld, nodefld)

extern struct kmem_cache *ceph_inode_cachep;
extern struct kmem_cache *ceph_cap_cachep;
extern struct kmem_cache *ceph_cap_flush_cachep;
extern struct kmem_cache *ceph_dentry_cachep;
extern struct kmem_cache *ceph_file_cachep;


extern bool libceph_compatible(void *data);

extern const char *ceph_msg_type_name(int type);
extern int ceph_check_fsid(struct ceph_client *client, struct ceph_fsid *fsid);
extern void *ceph_kvmalloc(size_t size, gfp_t flags);

extern struct ceph_options *ceph_parse_options(char *options,
			      const char *dev_name, const char *dev_name_end,
			      int (*parse_extra_token)(char *c, void *private),
			      void *private);
int ceph_print_client_options(struct seq_file *m, struct ceph_client *client);
extern void ceph_destroy_options(struct ceph_options *opt);
extern int ceph_compare_options(struct ceph_options *new_opt,
				struct ceph_client *client);
extern struct ceph_client *ceph_create_client(struct ceph_options *opt,
					      void *private,
					      u64 supported_features,
					      u64 required_features);
struct ceph_entity_addr *ceph_client_addr(struct ceph_client *client);
u64 ceph_client_gid(struct ceph_client *client);
extern void ceph_destroy_client(struct ceph_client *client);
extern int __ceph_open_session(struct ceph_client *client,
			       unsigned long started);
extern int ceph_open_session(struct ceph_client *client);
int ceph_wait_for_latest_osdmap(struct ceph_client *client,
				unsigned long timeout);


extern void ceph_release_page_vector(struct page **pages, int num_pages);

extern struct page **ceph_get_direct_page_vector(const void __user *data,
						 int num_pages,
						 bool write_page);
extern void ceph_put_page_vector(struct page **pages, int num_pages,
				 bool dirty);
extern struct page **ceph_alloc_page_vector(int num_pages, gfp_t flags);
extern int ceph_copy_user_to_page_vector(struct page **pages,
					 const void __user *data,
					 loff_t off, size_t len);
extern void ceph_copy_to_page_vector(struct page **pages,
				    const void *data,
				    loff_t off, size_t len);
extern void ceph_copy_from_page_vector(struct page **pages,
				    void *data,
				    loff_t off, size_t len);
extern void ceph_zero_page_vector_range(int off, int len, struct page **pages);


#endif 
