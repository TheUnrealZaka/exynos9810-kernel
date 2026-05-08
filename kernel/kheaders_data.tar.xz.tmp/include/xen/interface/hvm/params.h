

#ifndef __XEN_PUBLIC_HVM_PARAMS_H__
#define __XEN_PUBLIC_HVM_PARAMS_H__

#include <xen/interface/hvm/hvm_op.h>



#define HVM_PARAM_CALLBACK_IRQ 0


#define HVM_PARAM_CALLBACK_TYPE_GSI      0


#define HVM_PARAM_CALLBACK_TYPE_PCI_INTX 1


#if defined(__i386__) || defined(__x86_64__)
#define HVM_PARAM_CALLBACK_TYPE_VECTOR   2

#elif defined(__arm__) || defined(__aarch64__)
#define HVM_PARAM_CALLBACK_TYPE_PPI      2

#endif

#define HVM_PARAM_STORE_PFN    1
#define HVM_PARAM_STORE_EVTCHN 2

#define HVM_PARAM_PAE_ENABLED  4

#define HVM_PARAM_IOREQ_PFN    5

#define HVM_PARAM_BUFIOREQ_PFN 6


#define HVM_PARAM_TIMER_MODE   10
#define HVMPTM_delay_for_missed_ticks    0
#define HVMPTM_no_delay_for_missed_ticks 1
#define HVMPTM_no_missed_ticks_pending   2
#define HVMPTM_one_missed_tick_pending   3


#define HVM_PARAM_HPET_ENABLED 11


#define HVM_PARAM_IDENT_PT     12


#define HVM_PARAM_DM_DOMAIN    13


#define HVM_PARAM_ACPI_S_STATE 14


#define HVM_PARAM_VM86_TSS     15


#define HVM_PARAM_VPT_ALIGN    16


#define HVM_PARAM_CONSOLE_PFN    17
#define HVM_PARAM_CONSOLE_EVTCHN 18

#define HVM_NR_PARAMS          19

#endif 
