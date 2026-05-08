

#ifndef __XEN_PUBLIC_MEMORY_H__
#define __XEN_PUBLIC_MEMORY_H__

#include <linux/spinlock.h>


#define XENMEM_increase_reservation 0
#define XENMEM_decrease_reservation 1
#define XENMEM_populate_physmap     6
struct xen_memory_reservation {

    
    GUEST_HANDLE(xen_pfn_t) extent_start;

    
    xen_ulong_t  nr_extents;
    unsigned int   extent_order;

    
    unsigned int   address_bits;

    
    domid_t        domid;

};
DEFINE_GUEST_HANDLE_STRUCT(xen_memory_reservation);


#define XENMEM_exchange             11
struct xen_memory_exchange {
    
    struct xen_memory_reservation in;

    
    struct xen_memory_reservation out;

    
    xen_ulong_t nr_exchanged;
};

DEFINE_GUEST_HANDLE_STRUCT(xen_memory_exchange);

#define XENMEM_maximum_ram_page     2


#define XENMEM_current_reservation  3
#define XENMEM_maximum_reservation  4


#define XENMEM_machphys_mfn_list    5
struct xen_machphys_mfn_list {
    
    unsigned int max_extents;

    
    GUEST_HANDLE(xen_pfn_t) extent_start;

    
    unsigned int nr_extents;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_machphys_mfn_list);


#define XENMEM_machphys_mapping     12
struct xen_machphys_mapping {
    xen_ulong_t v_start, v_end; 
    xen_ulong_t max_mfn;        
};
DEFINE_GUEST_HANDLE_STRUCT(xen_machphys_mapping_t);

#define XENMAPSPACE_shared_info  0 
#define XENMAPSPACE_grant_table  1 
#define XENMAPSPACE_gmfn         2 
#define XENMAPSPACE_gmfn_range   3 
#define XENMAPSPACE_gmfn_foreign 4 
#define XENMAPSPACE_dev_mmio     5 


#define XENMEM_add_to_physmap      7
struct xen_add_to_physmap {
    
    domid_t domid;

    
    uint16_t    size;

    
    unsigned int space;

    
    xen_ulong_t idx;

    
    xen_pfn_t gpfn;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_add_to_physmap);




#define XENMEM_add_to_physmap_range 23
struct xen_add_to_physmap_range {
    
    
    domid_t domid;
    uint16_t space; 

    
    uint16_t size;
    domid_t foreign_domid; 

    
    GUEST_HANDLE(xen_ulong_t) idxs;

    
    GUEST_HANDLE(xen_pfn_t) gpfns;

    

    
    GUEST_HANDLE(int) errs;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_add_to_physmap_range);


#define XENMEM_memory_map           9
struct xen_memory_map {
    
    unsigned int nr_entries;

    
    GUEST_HANDLE(void) buffer;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_memory_map);


#define XENMEM_machine_memory_map   10



extern spinlock_t xen_reservation_lock;


#define XENMEM_remove_from_physmap      15
struct xen_remove_from_physmap {
    
    domid_t domid;

    
    xen_pfn_t gpfn;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_remove_from_physmap);

#endif 
