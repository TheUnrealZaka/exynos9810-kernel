

#ifndef __XEN_PUBLIC_XEN_H__
#define __XEN_PUBLIC_XEN_H__

#include <asm/xen/interface.h>




#define __HYPERVISOR_set_trap_table        0
#define __HYPERVISOR_mmu_update            1
#define __HYPERVISOR_set_gdt               2
#define __HYPERVISOR_stack_switch          3
#define __HYPERVISOR_set_callbacks         4
#define __HYPERVISOR_fpu_taskswitch        5
#define __HYPERVISOR_sched_op_compat       6
#define __HYPERVISOR_platform_op           7
#define __HYPERVISOR_set_debugreg          8
#define __HYPERVISOR_get_debugreg          9
#define __HYPERVISOR_update_descriptor    10
#define __HYPERVISOR_memory_op            12
#define __HYPERVISOR_multicall            13
#define __HYPERVISOR_update_va_mapping    14
#define __HYPERVISOR_set_timer_op         15
#define __HYPERVISOR_event_channel_op_compat 16
#define __HYPERVISOR_xen_version          17
#define __HYPERVISOR_console_io           18
#define __HYPERVISOR_physdev_op_compat    19
#define __HYPERVISOR_grant_table_op       20
#define __HYPERVISOR_vm_assist            21
#define __HYPERVISOR_update_va_mapping_otherdomain 22
#define __HYPERVISOR_iret                 23 
#define __HYPERVISOR_vcpu_op              24
#define __HYPERVISOR_set_segment_base     25 
#define __HYPERVISOR_mmuext_op            26
#define __HYPERVISOR_xsm_op               27
#define __HYPERVISOR_nmi_op               28
#define __HYPERVISOR_sched_op             29
#define __HYPERVISOR_callback_op          30
#define __HYPERVISOR_xenoprof_op          31
#define __HYPERVISOR_event_channel_op     32
#define __HYPERVISOR_physdev_op           33
#define __HYPERVISOR_hvm_op               34
#define __HYPERVISOR_sysctl               35
#define __HYPERVISOR_domctl               36
#define __HYPERVISOR_kexec_op             37
#define __HYPERVISOR_tmem_op              38
#define __HYPERVISOR_xc_reserved_op       39 
#define __HYPERVISOR_xenpmu_op            40


#define __HYPERVISOR_arch_0               48
#define __HYPERVISOR_arch_1               49
#define __HYPERVISOR_arch_2               50
#define __HYPERVISOR_arch_3               51
#define __HYPERVISOR_arch_4               52
#define __HYPERVISOR_arch_5               53
#define __HYPERVISOR_arch_6               54
#define __HYPERVISOR_arch_7               55


#define VIRQ_TIMER      0  
#define VIRQ_DEBUG      1  
#define VIRQ_CONSOLE    2  
#define VIRQ_DOM_EXC    3  
#define VIRQ_TBUF       4  
#define VIRQ_DEBUGGER   6  
#define VIRQ_XENOPROF   7  
#define VIRQ_CON_RING   8  
#define VIRQ_PCPU_STATE 9  
#define VIRQ_MEM_EVENT  10 
#define VIRQ_XC_RESERVED 11 
#define VIRQ_ENOMEM     12 
#define VIRQ_XENPMU     13  


#define VIRQ_ARCH_0    16
#define VIRQ_ARCH_1    17
#define VIRQ_ARCH_2    18
#define VIRQ_ARCH_3    19
#define VIRQ_ARCH_4    20
#define VIRQ_ARCH_5    21
#define VIRQ_ARCH_6    22
#define VIRQ_ARCH_7    23

#define NR_VIRQS       24


#define MMU_NORMAL_PT_UPDATE      0 
#define MMU_MACHPHYS_UPDATE       1 
#define MMU_PT_UPDATE_PRESERVE_AD 2 



#define MMUEXT_PIN_L1_TABLE      0
#define MMUEXT_PIN_L2_TABLE      1
#define MMUEXT_PIN_L3_TABLE      2
#define MMUEXT_PIN_L4_TABLE      3
#define MMUEXT_UNPIN_TABLE       4
#define MMUEXT_NEW_BASEPTR       5
#define MMUEXT_TLB_FLUSH_LOCAL   6
#define MMUEXT_INVLPG_LOCAL      7
#define MMUEXT_TLB_FLUSH_MULTI   8
#define MMUEXT_INVLPG_MULTI      9
#define MMUEXT_TLB_FLUSH_ALL    10
#define MMUEXT_INVLPG_ALL       11
#define MMUEXT_FLUSH_CACHE      12
#define MMUEXT_SET_LDT          13
#define MMUEXT_NEW_USER_BASEPTR 15
#define MMUEXT_CLEAR_PAGE       16
#define MMUEXT_COPY_PAGE        17
#define MMUEXT_FLUSH_CACHE_GLOBAL 18
#define MMUEXT_MARK_SUPER       19
#define MMUEXT_UNMARK_SUPER     20

#ifndef __ASSEMBLY__
struct mmuext_op {
	unsigned int cmd;
	union {
		
		xen_pfn_t mfn;
		
		unsigned long linear_addr;
	} arg1;
	union {
		
		unsigned int nr_ents;
		
		void *vcpumask;
		
		xen_pfn_t src_mfn;
	} arg2;
};
DEFINE_GUEST_HANDLE_STRUCT(mmuext_op);
#endif




#define UVMF_NONE               (0UL<<0) 
#define UVMF_TLB_FLUSH          (1UL<<0) 
#define UVMF_INVLPG             (2UL<<0) 
#define UVMF_FLUSHTYPE_MASK     (3UL<<0)
#define UVMF_MULTI              (0UL<<2) 
#define UVMF_LOCAL              (0UL<<2) 
#define UVMF_ALL                (1UL<<2) 


#define CONSOLEIO_write         0
#define CONSOLEIO_read          1


#define VMASST_CMD_enable                0
#define VMASST_CMD_disable               1


#define VMASST_TYPE_4gb_segments         0


#define VMASST_TYPE_4gb_segments_notify  1


#define VMASST_TYPE_writable_pagetables  2


#define VMASST_TYPE_pae_extended_cr3     3


#define VMASST_TYPE_architectural_iopl   4


#define VMASST_TYPE_runstate_update_flag 5

#define MAX_VMASST_TYPE 5

#ifndef __ASSEMBLY__

typedef uint16_t domid_t;


#define DOMID_FIRST_RESERVED (0x7FF0U)


#define DOMID_SELF (0x7FF0U)


#define DOMID_IO   (0x7FF1U)


#define DOMID_XEN  (0x7FF2U)


#define DOMID_COW  (0x7FF3U)


#define DOMID_INVALID (0x7FF4U)


#define DOMID_IDLE (0x7FFFU)


struct mmu_update {
    uint64_t ptr;       
    uint64_t val;       
};
DEFINE_GUEST_HANDLE_STRUCT(mmu_update);


struct multicall_entry {
    xen_ulong_t op;
    xen_long_t result;
    xen_ulong_t args[6];
};
DEFINE_GUEST_HANDLE_STRUCT(multicall_entry);

struct vcpu_time_info {
	
	uint32_t version;
	uint32_t pad0;
	uint64_t tsc_timestamp;   
	uint64_t system_time;     
	
	uint32_t tsc_to_system_mul;
	int8_t   tsc_shift;
	int8_t   pad1[3];
}; 

struct vcpu_info {
	
	uint8_t evtchn_upcall_pending;
	uint8_t evtchn_upcall_mask;
	xen_ulong_t evtchn_pending_sel;
	struct arch_vcpu_info arch;
	struct pvclock_vcpu_time_info time;
}; 


struct shared_info {
	struct vcpu_info vcpu_info[MAX_VIRT_CPUS];

	
	xen_ulong_t evtchn_pending[sizeof(xen_ulong_t) * 8];
	xen_ulong_t evtchn_mask[sizeof(xen_ulong_t) * 8];

	
	struct pvclock_wall_clock wc;

	struct arch_shared_info arch;

};



#define MAX_GUEST_CMDLINE 1024
struct start_info {
	
	char magic[32];             
	unsigned long nr_pages;     
	unsigned long shared_info;  
	uint32_t flags;             
	xen_pfn_t store_mfn;        
	uint32_t store_evtchn;      
	union {
		struct {
			xen_pfn_t mfn;      
			uint32_t  evtchn;   
		} domU;
		struct {
			uint32_t info_off;  
			uint32_t info_size; 
		} dom0;
	} console;
	
	unsigned long pt_base;      
	unsigned long nr_pt_frames; 
	unsigned long mfn_list;     
	unsigned long mod_start;    
	unsigned long mod_len;      
	int8_t cmd_line[MAX_GUEST_CMDLINE];
	
	unsigned long first_p2m_pfn;
	unsigned long nr_p2m_frames;
};


#define SIF_PRIVILEGED      (1<<0)  
#define SIF_INITDOMAIN      (1<<1)  
#define SIF_MULTIBOOT_MOD   (1<<2)  
#define SIF_MOD_START_PFN   (1<<3)  
#define SIF_VIRT_P2M_4TOOLS (1<<4)  
				    
#define SIF_PM_MASK       (0xFF<<8) 


struct xen_multiboot_mod_list {
	
	uint32_t mod_start;
	
	uint32_t mod_end;
	
	uint32_t cmdline;
	
	uint32_t pad;
};

struct dom0_vga_console_info {
	uint8_t video_type;
#define XEN_VGATYPE_TEXT_MODE_3 0x03
#define XEN_VGATYPE_VESA_LFB    0x23
#define XEN_VGATYPE_EFI_LFB     0x70

	union {
		struct {
			
			uint16_t font_height;
			
			uint16_t cursor_x, cursor_y;
			
			uint16_t rows, columns;
		} text_mode_3;

		struct {
			
			uint16_t width, height;
			
			uint16_t bytes_per_line;
			
			uint16_t bits_per_pixel;
			
			uint32_t lfb_base;
			uint32_t lfb_size;
			
			uint8_t  red_pos, red_size;
			uint8_t  green_pos, green_size;
			uint8_t  blue_pos, blue_size;
			uint8_t  rsvd_pos, rsvd_size;

			
			uint32_t gbl_caps;
			
			uint16_t mode_attrs;
		} vesa_lfb;
	} u;
};

typedef uint64_t cpumap_t;

typedef uint8_t xen_domain_handle_t[16];


#define __mk_unsigned_long(x) x ## UL
#define mk_unsigned_long(x) __mk_unsigned_long(x)

#define TMEM_SPEC_VERSION 1

struct tmem_op {
	uint32_t cmd;
	int32_t pool_id;
	union {
		struct {  
			uint64_t uuid[2];
			uint32_t flags;
		} new;
		struct {
			uint64_t oid[3];
			uint32_t index;
			uint32_t tmem_offset;
			uint32_t pfn_offset;
			uint32_t len;
			GUEST_HANDLE(void) gmfn; 
		} gen;
	} u;
};

DEFINE_GUEST_HANDLE(u64);

#else 


#define mk_unsigned_long(x) x

#endif 

#endif 
