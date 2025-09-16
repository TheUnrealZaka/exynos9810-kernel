

#include <linux/workqueue.h>

#define UPDATE_MEMORY_STATE(BLOCK, VALUE) BLOCK->update_call(BLOCK, VALUE)

struct memory_state_update_block;

typedef void (*memory_state_update_fn_t)(struct memory_state_update_block *ub,
		int value);


struct memory_state_update_block {
	memory_state_update_fn_t update_call;
	int id;
};


struct memory_state_update_block *memory_state_register_frequency_source(void);


struct memory_state_update_block *memory_state_register_bandwidth_source(void);
