#ifndef _XEN_XEN_H
#define _XEN_XEN_H

enum xen_domain_type {
	XEN_NATIVE,		
	XEN_PV_DOMAIN,		
	XEN_HVM_DOMAIN,		
};

#ifdef CONFIG_XEN
extern enum xen_domain_type xen_domain_type;
#else
#define xen_domain_type		XEN_NATIVE
#endif

#define xen_domain()		(xen_domain_type != XEN_NATIVE)
#define xen_pv_domain()		(xen_domain() &&			\
				 xen_domain_type == XEN_PV_DOMAIN)
#define xen_hvm_domain()	(xen_domain() &&			\
				 xen_domain_type == XEN_HVM_DOMAIN)

#ifdef CONFIG_XEN_DOM0
#include <xen/interface/xen.h>
#include <asm/xen/hypervisor.h>

#define xen_initial_domain()	(xen_domain() && \
				 xen_start_info && xen_start_info->flags & SIF_INITDOMAIN)
#else  
#define xen_initial_domain()	(0)
#endif	

#ifdef CONFIG_XEN_PVH

#include <xen/features.h>
#define xen_pvh_domain() (xen_pv_domain() && \
			  xen_feature(XENFEAT_auto_translated_physmap) && \
			  xen_have_vector_callback)
#else
#define xen_pvh_domain()	(0)
#endif
#endif	
