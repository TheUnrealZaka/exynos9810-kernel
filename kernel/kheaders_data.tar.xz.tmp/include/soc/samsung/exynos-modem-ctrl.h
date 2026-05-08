

#ifndef __EXYNOS_MODEM_CTRL_H
#define __EXYNOS_MODEM_CTRL_H

#include <linux/skbuff.h>

extern int ss310ap_force_crash_exit_ext(void);
extern int ss310ap_send_panic_noti_ext(void);
extern bool __skb_free_head_cp_zerocopy(struct sk_buff *skb);

#ifdef CONFIG_CP_ZEROCOPY

static inline bool skb_free_head_cp_zerocopy(struct sk_buff *skb)
{
	return __skb_free_head_cp_zerocopy(skb);
}
#else
static inline bool skb_free_head_cp_zerocopy(struct sk_buff *skb) {
	return false;
}
#endif

#endif
