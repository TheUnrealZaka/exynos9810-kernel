#ifndef __XEN_PUBLIC_XENPMU_H__
#define __XEN_PUBLIC_XENPMU_H__

#include "xen.h"

#define XENPMU_VER_MAJ    0
#define XENPMU_VER_MIN    1



#define XENPMU_mode_get        0 
#define XENPMU_mode_set        1
#define XENPMU_feature_get     2
#define XENPMU_feature_set     3
#define XENPMU_init            4
#define XENPMU_finish          5
#define XENPMU_lvtpc_set       6
#define XENPMU_flush           7




struct xen_pmu_params {
	
	struct {
		uint32_t maj;
		uint32_t min;
	} version;
	uint64_t val;

	
	uint32_t vcpu;
	uint32_t pad;
};


#define XENPMU_MODE_OFF           0
#define XENPMU_MODE_SELF          (1<<0)
#define XENPMU_MODE_HV            (1<<1)
#define XENPMU_MODE_ALL           (1<<2)


#define XENPMU_FEATURE_INTEL_BTS  1


struct xen_pmu_data {
	
	uint32_t vcpu_id;

	
	uint32_t pcpu_id;

	
	domid_t  domain_id;

	uint8_t pad[6];

	
	struct xen_pmu_arch pmu;
};

#endif 
