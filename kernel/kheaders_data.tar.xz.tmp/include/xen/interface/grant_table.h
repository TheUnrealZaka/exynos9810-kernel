

#ifndef __XEN_PUBLIC_GRANT_TABLE_H__
#define __XEN_PUBLIC_GRANT_TABLE_H__

#include <xen/interface/xen.h>






typedef uint32_t grant_ref_t;




struct grant_entry_v1 {
    
    uint16_t flags;
    
    domid_t  domid;
    
    uint32_t frame;
};


#define GTF_invalid         (0U<<0)
#define GTF_permit_access   (1U<<0)
#define GTF_accept_transfer (2U<<0)
#define GTF_transitive      (3U<<0)
#define GTF_type_mask       (3U<<0)


#define _GTF_readonly       (2)
#define GTF_readonly        (1U<<_GTF_readonly)
#define _GTF_reading        (3)
#define GTF_reading         (1U<<_GTF_reading)
#define _GTF_writing        (4)
#define GTF_writing         (1U<<_GTF_writing)
#define _GTF_sub_page       (8)
#define GTF_sub_page        (1U<<_GTF_sub_page)


#define _GTF_transfer_committed (2)
#define GTF_transfer_committed  (1U<<_GTF_transfer_committed)
#define _GTF_transfer_completed (3)
#define GTF_transfer_completed  (1U<<_GTF_transfer_completed)




struct grant_entry_header {
    uint16_t flags;
    domid_t  domid;
};


union grant_entry_v2 {
    struct grant_entry_header hdr;

    
    struct {
	struct grant_entry_header hdr;
	uint32_t pad0;
	uint64_t frame;
    } full_page;

    
    struct {
	struct grant_entry_header hdr;
	uint16_t page_off;
	uint16_t length;
	uint64_t frame;
    } sub_page;

    
    struct {
	struct grant_entry_header hdr;
	domid_t trans_domid;
	uint16_t pad0;
	grant_ref_t gref;
    } transitive;

    uint32_t __spacer[4]; 
};

typedef uint16_t grant_status_t;




typedef uint32_t grant_handle_t;


#define GNTTABOP_map_grant_ref        0
struct gnttab_map_grant_ref {
    
    uint64_t host_addr;
    uint32_t flags;               
    grant_ref_t ref;
    domid_t  dom;
    
    int16_t  status;              
    grant_handle_t handle;
    uint64_t dev_bus_addr;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_map_grant_ref);


#define GNTTABOP_unmap_grant_ref      1
struct gnttab_unmap_grant_ref {
    
    uint64_t host_addr;
    uint64_t dev_bus_addr;
    grant_handle_t handle;
    
    int16_t  status;              
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_unmap_grant_ref);


#define GNTTABOP_setup_table          2
struct gnttab_setup_table {
    
    domid_t  dom;
    uint32_t nr_frames;
    
    int16_t  status;              
    GUEST_HANDLE(xen_pfn_t) frame_list;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_setup_table);


#define GNTTABOP_dump_table           3
struct gnttab_dump_table {
    
    domid_t dom;
    
    int16_t status;               
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_dump_table);


#define GNTTABOP_transfer                4
struct gnttab_transfer {
    
    xen_pfn_t mfn;
    domid_t       domid;
    grant_ref_t   ref;
    
    int16_t       status;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_transfer);



#define _GNTCOPY_source_gref      (0)
#define GNTCOPY_source_gref       (1<<_GNTCOPY_source_gref)
#define _GNTCOPY_dest_gref        (1)
#define GNTCOPY_dest_gref         (1<<_GNTCOPY_dest_gref)

#define GNTTABOP_copy                 5
struct gnttab_copy {
	
	struct {
		union {
			grant_ref_t ref;
			xen_pfn_t   gmfn;
		} u;
		domid_t  domid;
		uint16_t offset;
	} source, dest;
	uint16_t      len;
	uint16_t      flags;          
	
	int16_t       status;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_copy);


#define GNTTABOP_query_size           6
struct gnttab_query_size {
    
    domid_t  dom;
    
    uint32_t nr_frames;
    uint32_t max_nr_frames;
    int16_t  status;              
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_query_size);


#define GNTTABOP_unmap_and_replace    7
struct gnttab_unmap_and_replace {
    
    uint64_t host_addr;
    uint64_t new_addr;
    grant_handle_t handle;
    
    int16_t  status;              
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_unmap_and_replace);


#define GNTTABOP_set_version          8
struct gnttab_set_version {
    
    uint32_t version;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_set_version);


#define GNTTABOP_get_status_frames     9
struct gnttab_get_status_frames {
    
    uint32_t nr_frames;
    domid_t  dom;
    
    int16_t  status;              
    GUEST_HANDLE(uint64_t) frame_list;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_get_status_frames);


#define GNTTABOP_get_version          10
struct gnttab_get_version {
    
    domid_t dom;
    uint16_t pad;
    
    uint32_t version;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_get_version);


#define GNTTABOP_cache_flush          12
struct gnttab_cache_flush {
    union {
        uint64_t dev_bus_addr;
        grant_ref_t ref;
    } a;
    uint16_t offset;   
    uint16_t length;   
#define GNTTAB_CACHE_CLEAN          (1<<0)
#define GNTTAB_CACHE_INVAL          (1<<1)
#define GNTTAB_CACHE_SOURCE_GREF    (1<<31)
    uint32_t op;
};
DEFINE_GUEST_HANDLE_STRUCT(gnttab_cache_flush);


 
#define _GNTMAP_device_map      (0)
#define GNTMAP_device_map       (1<<_GNTMAP_device_map)
 
#define _GNTMAP_host_map        (1)
#define GNTMAP_host_map         (1<<_GNTMAP_host_map)
 
#define _GNTMAP_readonly        (2)
#define GNTMAP_readonly         (1<<_GNTMAP_readonly)
 
#define _GNTMAP_application_map (3)
#define GNTMAP_application_map  (1<<_GNTMAP_application_map)

 
#define _GNTMAP_contains_pte    (4)
#define GNTMAP_contains_pte     (1<<_GNTMAP_contains_pte)


#define _GNTMAP_guest_avail0    (16)
#define GNTMAP_guest_avail_mask ((uint32_t)~0 << _GNTMAP_guest_avail0)


#define GNTST_okay             (0)  
#define GNTST_general_error    (-1) 
#define GNTST_bad_domain       (-2) 
#define GNTST_bad_gntref       (-3) 
#define GNTST_bad_handle       (-4) 
#define GNTST_bad_virt_addr    (-5) 
#define GNTST_bad_dev_addr     (-6) 
#define GNTST_no_device_space  (-7) 
#define GNTST_permission_denied (-8) 
#define GNTST_bad_page         (-9) 
#define GNTST_bad_copy_arg    (-10) 
#define GNTST_address_too_big (-11) 
#define GNTST_eagain          (-12) 

#define GNTTABOP_error_msgs {                   \
    "okay",                                     \
    "undefined error",                          \
    "unrecognised domain id",                   \
    "invalid grant reference",                  \
    "invalid mapping handle",                   \
    "invalid virtual address",                  \
    "invalid device address",                   \
    "no spare translation slot in the I/O MMU", \
    "permission denied",                        \
    "bad page",                                 \
    "copy arguments cross page boundary",       \
    "page address size too large",              \
    "operation not done; try again"             \
}

#endif 
