

#ifndef __XEN_PUBLIC_PLATFORM_H__
#define __XEN_PUBLIC_PLATFORM_H__

#include <xen/interface/xen.h>

#define XENPF_INTERFACE_VERSION 0x03000001


#define XENPF_settime32             17
struct xenpf_settime32 {
	
	uint32_t secs;
	uint32_t nsecs;
	uint64_t system_time;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_settime32_t);
#define XENPF_settime64           62
struct xenpf_settime64 {
    
    uint64_t secs;
    uint32_t nsecs;
    uint32_t mbz;
    uint64_t system_time;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_settime64_t);


#define XENPF_add_memtype         31
struct xenpf_add_memtype {
	
	xen_pfn_t mfn;
	uint64_t nr_mfns;
	uint32_t type;
	
	uint32_t handle;
	uint32_t reg;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_add_memtype_t);


#define XENPF_del_memtype         32
struct xenpf_del_memtype {
	
	uint32_t handle;
	uint32_t reg;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_del_memtype_t);


#define XENPF_read_memtype        33
struct xenpf_read_memtype {
	
	uint32_t reg;
	
	xen_pfn_t mfn;
	uint64_t nr_mfns;
	uint32_t type;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_read_memtype_t);

#define XENPF_microcode_update    35
struct xenpf_microcode_update {
	
	GUEST_HANDLE(void) data;          
	uint32_t length;                  
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_microcode_update_t);

#define XENPF_platform_quirk      39
#define QUIRK_NOIRQBALANCING      1 
#define QUIRK_IOAPIC_BAD_REGSEL   2 
#define QUIRK_IOAPIC_GOOD_REGSEL  3 
struct xenpf_platform_quirk {
	
	uint32_t quirk_id;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_platform_quirk_t);

#define XENPF_efi_runtime_call    49
#define XEN_EFI_get_time                      1
#define XEN_EFI_set_time                      2
#define XEN_EFI_get_wakeup_time               3
#define XEN_EFI_set_wakeup_time               4
#define XEN_EFI_get_next_high_monotonic_count 5
#define XEN_EFI_get_variable                  6
#define XEN_EFI_set_variable                  7
#define XEN_EFI_get_next_variable_name        8
#define XEN_EFI_query_variable_info           9
#define XEN_EFI_query_capsule_capabilities   10
#define XEN_EFI_update_capsule               11

struct xenpf_efi_runtime_call {
	uint32_t function;
	
	uint32_t misc;
	xen_ulong_t status;
	union {
#define XEN_EFI_GET_TIME_SET_CLEARS_NS 0x00000001
		struct {
			struct xenpf_efi_time {
				uint16_t year;
				uint8_t month;
				uint8_t day;
				uint8_t hour;
				uint8_t min;
				uint8_t sec;
				uint32_t ns;
				int16_t tz;
				uint8_t daylight;
			} time;
			uint32_t resolution;
			uint32_t accuracy;
		} get_time;

		struct xenpf_efi_time set_time;

#define XEN_EFI_GET_WAKEUP_TIME_ENABLED 0x00000001
#define XEN_EFI_GET_WAKEUP_TIME_PENDING 0x00000002
		struct xenpf_efi_time get_wakeup_time;

#define XEN_EFI_SET_WAKEUP_TIME_ENABLE      0x00000001
#define XEN_EFI_SET_WAKEUP_TIME_ENABLE_ONLY 0x00000002
		struct xenpf_efi_time set_wakeup_time;

#define XEN_EFI_VARIABLE_NON_VOLATILE       0x00000001
#define XEN_EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define XEN_EFI_VARIABLE_RUNTIME_ACCESS     0x00000004
		struct {
			GUEST_HANDLE(void) name;  
			xen_ulong_t size;
			GUEST_HANDLE(void) data;
			struct xenpf_efi_guid {
				uint32_t data1;
				uint16_t data2;
				uint16_t data3;
				uint8_t data4[8];
			} vendor_guid;
		} get_variable, set_variable;

		struct {
			xen_ulong_t size;
			GUEST_HANDLE(void) name;  
			struct xenpf_efi_guid vendor_guid;
		} get_next_variable_name;

		struct {
			uint32_t attr;
			uint64_t max_store_size;
			uint64_t remain_store_size;
			uint64_t max_size;
		} query_variable_info;

		struct {
			GUEST_HANDLE(void) capsule_header_array;
			xen_ulong_t capsule_count;
			uint64_t max_capsule_size;
			uint32_t reset_type;
		} query_capsule_capabilities;

		struct {
			GUEST_HANDLE(void) capsule_header_array;
			xen_ulong_t capsule_count;
			uint64_t sg_list; 
		} update_capsule;
	} u;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_efi_runtime_call);

#define  XEN_FW_EFI_VERSION        0
#define  XEN_FW_EFI_CONFIG_TABLE   1
#define  XEN_FW_EFI_VENDOR         2
#define  XEN_FW_EFI_MEM_INFO       3
#define  XEN_FW_EFI_RT_VERSION     4

#define XENPF_firmware_info       50
#define XEN_FW_DISK_INFO          1 
#define XEN_FW_DISK_MBR_SIGNATURE 2 
#define XEN_FW_VBEDDC_INFO        3 
#define XEN_FW_EFI_INFO           4 
#define XEN_FW_KBD_SHIFT_FLAGS    5 

struct xenpf_firmware_info {
	
	uint32_t type;
	uint32_t index;
	
	union {
		struct {
			
			uint8_t device;                   
			uint8_t version;                  
			uint16_t interface_support;       
			
			uint16_t legacy_max_cylinder;     
			uint8_t legacy_max_head;          
			uint8_t legacy_sectors_per_track; 
			
			
			GUEST_HANDLE(void) edd_params;
		} disk_info; 
		struct {
			uint8_t device;                   
			uint32_t mbr_signature;           
		} disk_mbr_signature; 
		struct {
			
			uint8_t capabilities;
			uint8_t edid_transfer_time;
			
			GUEST_HANDLE(uchar) edid;
		} vbeddc_info; 

		union xenpf_efi_info {
			uint32_t version;
			struct {
				uint64_t addr;   
				uint32_t nent;
			} cfg;
			struct {
				uint32_t revision;
				uint32_t bufsz;  
				GUEST_HANDLE(void) name;
				
			} vendor;
			struct {
				uint64_t addr;
				uint64_t size;
				uint64_t attr;
				uint32_t type;
			} mem;
		} efi_info; 

		uint8_t kbd_shift_flags; 
	} u;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_firmware_info_t);

#define XENPF_enter_acpi_sleep    51
struct xenpf_enter_acpi_sleep {
	
	uint16_t val_a;             
	uint16_t val_b;             
	uint32_t sleep_state;       
#define XENPF_ACPI_SLEEP_EXTENDED 0x00000001
	uint32_t flags;             
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_enter_acpi_sleep_t);

#define XENPF_change_freq         52
struct xenpf_change_freq {
	
	uint32_t flags; 
	uint32_t cpu;   
	uint64_t freq;  
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_change_freq_t);


#define XENPF_getidletime         53
struct xenpf_getidletime {
	
	
	GUEST_HANDLE(uchar) cpumap_bitmap;
	
	
	uint32_t cpumap_nr_cpus;
	
	GUEST_HANDLE(uint64_t) idletime;
	
	
	uint64_t now;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_getidletime_t);

#define XENPF_set_processor_pminfo      54


#define XEN_PROCESSOR_PM_CX	1
#define XEN_PROCESSOR_PM_PX	2
#define XEN_PROCESSOR_PM_TX	4


#define XEN_PM_CX   0
#define XEN_PM_PX   1
#define XEN_PM_TX   2
#define XEN_PM_PDC  3

#define XEN_PX_PCT   1
#define XEN_PX_PSS   2
#define XEN_PX_PPC   4
#define XEN_PX_PSD   8

struct xen_power_register {
	uint32_t     space_id;
	uint32_t     bit_width;
	uint32_t     bit_offset;
	uint32_t     access_size;
	uint64_t     address;
};

struct xen_processor_csd {
	uint32_t    domain;      
	uint32_t    coord_type;  
	uint32_t    num;         
};
DEFINE_GUEST_HANDLE_STRUCT(xen_processor_csd);

struct xen_processor_cx {
	struct xen_power_register  reg; 
	uint8_t     type;     
	uint32_t    latency;  
	uint32_t    power;    
	uint32_t    dpcnt;    
	GUEST_HANDLE(xen_processor_csd) dp; 
};
DEFINE_GUEST_HANDLE_STRUCT(xen_processor_cx);

struct xen_processor_flags {
	uint32_t bm_control:1;
	uint32_t bm_check:1;
	uint32_t has_cst:1;
	uint32_t power_setup_done:1;
	uint32_t bm_rld_set:1;
};

struct xen_processor_power {
	uint32_t count;  
	struct xen_processor_flags flags;  
	GUEST_HANDLE(xen_processor_cx) states; 
};

struct xen_pct_register {
	uint8_t  descriptor;
	uint16_t length;
	uint8_t  space_id;
	uint8_t  bit_width;
	uint8_t  bit_offset;
	uint8_t  reserved;
	uint64_t address;
};

struct xen_processor_px {
	uint64_t core_frequency; 
	uint64_t power;      
	uint64_t transition_latency; 
	uint64_t bus_master_latency; 
	uint64_t control;        
	uint64_t status;     
};
DEFINE_GUEST_HANDLE_STRUCT(xen_processor_px);

struct xen_psd_package {
	uint64_t num_entries;
	uint64_t revision;
	uint64_t domain;
	uint64_t coord_type;
	uint64_t num_processors;
};

struct xen_processor_performance {
	uint32_t flags;     
	uint32_t platform_limit;  
	struct xen_pct_register control_register;
	struct xen_pct_register status_register;
	uint32_t state_count;     
	GUEST_HANDLE(xen_processor_px) states;
	struct xen_psd_package domain_info;
	uint32_t shared_type;     
};
DEFINE_GUEST_HANDLE_STRUCT(xen_processor_performance);

struct xenpf_set_processor_pminfo {
	
	uint32_t id;    
	uint32_t type;  
	union {
		struct xen_processor_power          power;
		struct xen_processor_performance    perf; 
		GUEST_HANDLE(uint32_t)              pdc;
	};
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_set_processor_pminfo);

#define XENPF_get_cpuinfo 55
struct xenpf_pcpuinfo {
	
	uint32_t xen_cpuid;
	
	
	uint32_t max_present;
#define XEN_PCPU_FLAGS_ONLINE   1
	
#define XEN_PCPU_FLAGS_INVALID  2
	uint32_t flags;
	uint32_t apic_id;
	uint32_t acpi_id;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_pcpuinfo);

#define XENPF_cpu_online	56
#define XENPF_cpu_offline	57
struct xenpf_cpu_ol {
	uint32_t cpuid;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_cpu_ol);

#define XENPF_cpu_hotadd	58
struct xenpf_cpu_hotadd {
	uint32_t apic_id;
	uint32_t acpi_id;
	uint32_t pxm;
};

#define XENPF_mem_hotadd	59
struct xenpf_mem_hotadd {
	uint64_t spfn;
	uint64_t epfn;
	uint32_t pxm;
	uint32_t flags;
};

#define XENPF_core_parking     60
struct xenpf_core_parking {
	
#define XEN_CORE_PARKING_SET   1
#define XEN_CORE_PARKING_GET   2
	uint32_t type;
	
	
	uint32_t idle_nums;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_core_parking);

#define XENPF_get_symbol      63
struct xenpf_symdata {
	
	uint32_t	namelen; 

	
	uint32_t	symnum; 
				
				

	
	GUEST_HANDLE(char) name;
	uint64_t	address;
	char            type;
};
DEFINE_GUEST_HANDLE_STRUCT(xenpf_symdata);

struct xen_platform_op {
	uint32_t cmd;
	uint32_t interface_version; 
	union {
		struct xenpf_settime32         settime32;
		struct xenpf_settime64         settime64;
		struct xenpf_add_memtype       add_memtype;
		struct xenpf_del_memtype       del_memtype;
		struct xenpf_read_memtype      read_memtype;
		struct xenpf_microcode_update  microcode;
		struct xenpf_platform_quirk    platform_quirk;
		struct xenpf_efi_runtime_call  efi_runtime_call;
		struct xenpf_firmware_info     firmware_info;
		struct xenpf_enter_acpi_sleep  enter_acpi_sleep;
		struct xenpf_change_freq       change_freq;
		struct xenpf_getidletime       getidletime;
		struct xenpf_set_processor_pminfo set_pminfo;
		struct xenpf_pcpuinfo          pcpu_info;
		struct xenpf_cpu_ol            cpu_ol;
		struct xenpf_cpu_hotadd        cpu_add;
		struct xenpf_mem_hotadd        mem_add;
		struct xenpf_core_parking      core_parking;
		struct xenpf_symdata           symdata;
		uint8_t                        pad[128];
	} u;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_platform_op_t);

#endif 
