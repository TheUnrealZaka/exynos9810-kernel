#ifndef CEPH_CRUSH_CRUSH_H
#define CEPH_CRUSH_CRUSH_H

#ifdef __KERNEL__
# include <linux/types.h>
#else
# include "crush_compat.h"
#endif




#define CRUSH_MAGIC 0x00010000ul   

#define CRUSH_MAX_DEPTH 10  
#define CRUSH_MAX_RULESET (1<<8)  
#define CRUSH_MAX_RULES CRUSH_MAX_RULESET  

#define CRUSH_MAX_DEVICE_WEIGHT (100u * 0x10000u)
#define CRUSH_MAX_BUCKET_WEIGHT (65535u * 0x10000u)

#define CRUSH_ITEM_UNDEF  0x7ffffffe  
#define CRUSH_ITEM_NONE   0x7fffffff  


struct crush_rule_step {
	__u32 op;
	__s32 arg1;
	__s32 arg2;
};


enum {
	CRUSH_RULE_NOOP = 0,
	CRUSH_RULE_TAKE = 1,          
	CRUSH_RULE_CHOOSE_FIRSTN = 2, 
				      
	CRUSH_RULE_CHOOSE_INDEP = 3,  
	CRUSH_RULE_EMIT = 4,          
	CRUSH_RULE_CHOOSELEAF_FIRSTN = 6,
	CRUSH_RULE_CHOOSELEAF_INDEP = 7,

	CRUSH_RULE_SET_CHOOSE_TRIES = 8, 
	CRUSH_RULE_SET_CHOOSELEAF_TRIES = 9, 
	CRUSH_RULE_SET_CHOOSE_LOCAL_TRIES = 10,
	CRUSH_RULE_SET_CHOOSE_LOCAL_FALLBACK_TRIES = 11,
	CRUSH_RULE_SET_CHOOSELEAF_VARY_R = 12,
	CRUSH_RULE_SET_CHOOSELEAF_STABLE = 13
};


#define CRUSH_CHOOSE_N            0
#define CRUSH_CHOOSE_N_MINUS(x)   (-(x))


struct crush_rule_mask {
	__u8 ruleset;
	__u8 type;
	__u8 min_size;
	__u8 max_size;
};

struct crush_rule {
	__u32 len;
	struct crush_rule_mask mask;
	struct crush_rule_step steps[0];
};

#define crush_rule_size(len) (sizeof(struct crush_rule) + \
			      (len)*sizeof(struct crush_rule_step))




enum {
	CRUSH_BUCKET_UNIFORM = 1,
	CRUSH_BUCKET_LIST = 2,
	CRUSH_BUCKET_TREE = 3,
	CRUSH_BUCKET_STRAW = 4,
	CRUSH_BUCKET_STRAW2 = 5,
};
extern const char *crush_bucket_alg_name(int alg);


#define CRUSH_LEGACY_ALLOWED_BUCKET_ALGS (	\
		(1 << CRUSH_BUCKET_UNIFORM) |	\
		(1 << CRUSH_BUCKET_LIST) |	\
		(1 << CRUSH_BUCKET_STRAW))

struct crush_bucket {
	__s32 id;        
	__u16 type;      
	__u8 alg;        
	__u8 hash;       
	__u32 weight;    
	__u32 size;      
	__s32 *items;

	
	__u32 perm_x;  
	__u32 perm_n;  
	__u32 *perm;
};

struct crush_bucket_uniform {
	struct crush_bucket h;
	__u32 item_weight;  
};

struct crush_bucket_list {
	struct crush_bucket h;
	__u32 *item_weights;  
	__u32 *sum_weights;   
};

struct crush_bucket_tree {
	struct crush_bucket h;  
	__u8 num_nodes;
	__u32 *node_weights;
};

struct crush_bucket_straw {
	struct crush_bucket h;
	__u32 *item_weights;   
	__u32 *straws;         
};

struct crush_bucket_straw2 {
	struct crush_bucket h;
	__u32 *item_weights;   
};




struct crush_map {
	struct crush_bucket **buckets;
	struct crush_rule **rules;

	__s32 max_buckets;
	__u32 max_rules;
	__s32 max_devices;

	
	__u32 choose_local_tries;
	
	__u32 choose_local_fallback_tries;
	
	__u32 choose_total_tries;
	
	__u32 chooseleaf_descend_once;

	
	__u8 chooseleaf_vary_r;

	
	__u8 chooseleaf_stable;

#ifndef __KERNEL__
	
	__u8 straw_calc_version;

	
	__u32 allowed_bucket_algs;

	__u32 *choose_tries;
#endif
};



extern int crush_get_bucket_item_weight(const struct crush_bucket *b, int pos);
extern void crush_destroy_bucket_uniform(struct crush_bucket_uniform *b);
extern void crush_destroy_bucket_list(struct crush_bucket_list *b);
extern void crush_destroy_bucket_tree(struct crush_bucket_tree *b);
extern void crush_destroy_bucket_straw(struct crush_bucket_straw *b);
extern void crush_destroy_bucket_straw2(struct crush_bucket_straw2 *b);
extern void crush_destroy_bucket(struct crush_bucket *b);
extern void crush_destroy_rule(struct crush_rule *r);
extern void crush_destroy(struct crush_map *map);

static inline int crush_calc_tree_node(int i)
{
	return ((i+1) << 1)-1;
}

#endif
