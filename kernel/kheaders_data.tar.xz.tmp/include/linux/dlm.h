
#ifndef __DLM_DOT_H__
#define __DLM_DOT_H__

#include <uapi/linux/dlm.h>


struct dlm_slot {
	int nodeid; 
	int slot;   
};



struct dlm_lockspace_ops {
	void (*recover_prep) (void *ops_arg);
	void (*recover_slot) (void *ops_arg, struct dlm_slot *slot);
	void (*recover_done) (void *ops_arg, struct dlm_slot *slots,
			      int num_slots, int our_slot, uint32_t generation);
};



int dlm_new_lockspace(const char *name, const char *cluster,
		      uint32_t flags, int lvblen,
		      const struct dlm_lockspace_ops *ops, void *ops_arg,
		      int *ops_result, dlm_lockspace_t **lockspace);



int dlm_release_lockspace(dlm_lockspace_t *lockspace, int force);



int dlm_lock(dlm_lockspace_t *lockspace,
	     int mode,
	     struct dlm_lksb *lksb,
	     uint32_t flags,
	     void *name,
	     unsigned int namelen,
	     uint32_t parent_lkid,
	     void (*lockast) (void *astarg),
	     void *astarg,
	     void (*bast) (void *astarg, int mode));



int dlm_unlock(dlm_lockspace_t *lockspace,
	       uint32_t lkid,
	       uint32_t flags,
	       struct dlm_lksb *lksb,
	       void *astarg);

#endif				
