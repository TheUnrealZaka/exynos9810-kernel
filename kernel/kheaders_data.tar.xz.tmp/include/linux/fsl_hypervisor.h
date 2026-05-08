
#ifndef FSL_HYPERVISOR_H
#define FSL_HYPERVISOR_H

#include <uapi/linux/fsl_hypervisor.h>



int fsl_hv_failover_register(struct notifier_block *nb);


int fsl_hv_failover_unregister(struct notifier_block *nb);

#endif
