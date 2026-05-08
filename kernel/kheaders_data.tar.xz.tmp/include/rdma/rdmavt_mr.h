#ifndef DEF_RDMAVT_INCMR_H
#define DEF_RDMAVT_INCMR_H






struct rvt_seg {
	void *vaddr;
	size_t length;
};


#define RVT_SEGSZ     (PAGE_SIZE / sizeof(struct rvt_seg))

struct rvt_segarray {
	struct rvt_seg segs[RVT_SEGSZ];
};

struct rvt_mregion {
	struct ib_pd *pd;       
	u64 user_base;          
	u64 iova;               
	size_t length;
	u32 lkey;
	u32 offset;             
	int access_flags;
	u32 max_segs;           
	u32 mapsz;              
	u8  page_shift;         
	u8  lkey_published;     
	atomic_t lkey_invalid;	
	struct completion comp; 
	atomic_t refcount;
	struct rvt_segarray *map[0];    
};

#define RVT_MAX_LKEY_TABLE_BITS 23

struct rvt_lkey_table {
	spinlock_t lock; 
	u32 next;               
	u32 gen;                
	u32 max;                
	struct rvt_mregion __rcu **table;
};


struct rvt_sge {
	struct rvt_mregion *mr;
	void *vaddr;            
	u32 sge_length;         
	u32 length;             
	u16 m;                  
	u16 n;                  
};

struct rvt_sge_state {
	struct rvt_sge *sg_list;      
	struct rvt_sge sge;   
	u32 total_len;
	u8 num_sge;
};

static inline void rvt_put_mr(struct rvt_mregion *mr)
{
	if (unlikely(atomic_dec_and_test(&mr->refcount)))
		complete(&mr->comp);
}

static inline void rvt_get_mr(struct rvt_mregion *mr)
{
	atomic_inc(&mr->refcount);
}

static inline void rvt_put_ss(struct rvt_sge_state *ss)
{
	while (ss->num_sge) {
		rvt_put_mr(ss->sge.mr);
		if (--ss->num_sge)
			ss->sge = *ss->sg_list++;
	}
}

#endif          
