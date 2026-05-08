#ifndef _ASM_GENERIC_FUTEX_H
#define _ASM_GENERIC_FUTEX_H

#include <linux/futex.h>
#include <linux/uaccess.h>
#include <asm/errno.h>

#ifndef CONFIG_SMP



static inline int
arch_futex_atomic_op_inuser(int op, u32 oparg, int *oval, u32 __user *uaddr)
{
	int oldval, ret;
	u32 tmp;

	preempt_disable();
	pagefault_disable();

	ret = -EFAULT;
	if (unlikely(get_user(oldval, uaddr) != 0))
		goto out_pagefault_enable;

	ret = 0;
	tmp = oldval;

	switch (op) {
	case FUTEX_OP_SET:
		tmp = oparg;
		break;
	case FUTEX_OP_ADD:
		tmp += oparg;
		break;
	case FUTEX_OP_OR:
		tmp |= oparg;
		break;
	case FUTEX_OP_ANDN:
		tmp &= ~oparg;
		break;
	case FUTEX_OP_XOR:
		tmp ^= oparg;
		break;
	default:
		ret = -ENOSYS;
	}

	if (ret == 0 && unlikely(put_user(tmp, uaddr) != 0))
		ret = -EFAULT;

out_pagefault_enable:
	pagefault_enable();
	preempt_enable();

	if (ret == 0)
		*oval = oldval;

	return ret;
}


static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
	u32 val;

	preempt_disable();
	if (unlikely(get_user(val, uaddr) != 0)) {
		preempt_enable();
		return -EFAULT;
	}

	if (val == oldval && unlikely(put_user(newval, uaddr) != 0)) {
		preempt_enable();
		return -EFAULT;
	}

	*uval = val;
	preempt_enable();

	return 0;
}

#else
static inline int
arch_futex_atomic_op_inuser(int op, u32 oparg, int *oval, u32 __user *uaddr)
{
	int oldval = 0, ret;

	pagefault_disable();

	switch (op) {
	case FUTEX_OP_SET:
	case FUTEX_OP_ADD:
	case FUTEX_OP_OR:
	case FUTEX_OP_ANDN:
	case FUTEX_OP_XOR:
	default:
		ret = -ENOSYS;
	}

	pagefault_enable();

	if (!ret)
		*oval = oldval;

	return ret;
}

static inline int
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
	return -ENOSYS;
}

#endif 
#endif
