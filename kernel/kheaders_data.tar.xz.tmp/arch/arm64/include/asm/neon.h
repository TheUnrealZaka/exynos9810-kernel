

#include <linux/types.h>

#define cpu_has_neon()		(1)

#define kernel_neon_begin()	kernel_neon_begin_partial(32)

void kernel_neon_begin_partial(u32 num_regs);
void kernel_neon_end(void);
