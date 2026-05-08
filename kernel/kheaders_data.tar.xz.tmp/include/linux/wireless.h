
#ifndef _LINUX_WIRELESS_H
#define _LINUX_WIRELESS_H

#include <uapi/linux/wireless.h>

#ifdef CONFIG_COMPAT

#include <linux/compat.h>

struct compat_iw_point {
	compat_caddr_t pointer;
	__u16 length;
	__u16 flags;
};
#endif
#ifdef CONFIG_COMPAT
struct __compat_iw_event {
	__u16		len;			
	__u16		cmd;			
	compat_caddr_t	pointer;
};
#define IW_EV_COMPAT_LCP_LEN offsetof(struct __compat_iw_event, pointer)
#define IW_EV_COMPAT_POINT_OFF offsetof(struct compat_iw_point, length)


#define IW_EV_COMPAT_CHAR_LEN	(IW_EV_COMPAT_LCP_LEN + IFNAMSIZ)
#define IW_EV_COMPAT_UINT_LEN	(IW_EV_COMPAT_LCP_LEN + sizeof(__u32))
#define IW_EV_COMPAT_FREQ_LEN	(IW_EV_COMPAT_LCP_LEN + sizeof(struct iw_freq))
#define IW_EV_COMPAT_PARAM_LEN	(IW_EV_COMPAT_LCP_LEN + sizeof(struct iw_param))
#define IW_EV_COMPAT_ADDR_LEN	(IW_EV_COMPAT_LCP_LEN + sizeof(struct sockaddr))
#define IW_EV_COMPAT_QUAL_LEN	(IW_EV_COMPAT_LCP_LEN + sizeof(struct iw_quality))
#define IW_EV_COMPAT_POINT_LEN	\
	(IW_EV_COMPAT_LCP_LEN + sizeof(struct compat_iw_point) - \
	 IW_EV_COMPAT_POINT_OFF)
#endif
#endif	
