

#ifndef __NET_TC_SKBEDIT_H
#define __NET_TC_SKBEDIT_H

#include <net/act_api.h>
#include <linux/tc_act/tc_skbedit.h>

struct tcf_skbedit {
	struct tc_action	common;
	u32		flags;
	u32		priority;
	u32		mark;
	u16		queue_mapping;
	u16		ptype;
};
#define to_skbedit(a) ((struct tcf_skbedit *)a)


static inline bool is_tcf_skbedit_mark(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	if (a->ops && a->ops->type == TCA_ACT_SKBEDIT)
		return to_skbedit(a)->flags == SKBEDIT_F_MARK;
#endif
	return false;
}

static inline u32 tcf_skbedit_mark(const struct tc_action *a)
{
	return to_skbedit(a)->mark;
}

#endif 
