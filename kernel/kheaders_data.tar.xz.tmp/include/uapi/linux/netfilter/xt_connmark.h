#ifndef _XT_CONNMARK_H
#define _XT_CONNMARK_H

#include <linux/types.h>



enum {
	XT_CONNMARK_SET = 0,
	XT_CONNMARK_SAVE,
	XT_CONNMARK_RESTORE
};

struct xt_connmark_tginfo1 {
	__u32 ctmark, ctmask, nfmask;
	__u8 mode;
};

struct xt_connmark_mtinfo1 {
	__u32 mark, mask;
	__u8 invert;
};

#endif 
