

#ifndef __XEN_PUBLIC_ARCH_X86_MCA_H__
#define __XEN_PUBLIC_ARCH_X86_MCA_H__


#define __HYPERVISOR_mca __HYPERVISOR_arch_0

#define XEN_MCA_INTERFACE_VERSION	0x01ecc003


#define XEN_MC_NONURGENT	0x1

#define XEN_MC_URGENT		0x2

#define XEN_MC_ACK		0x4


#define XEN_MC_OK		0x0

#define XEN_MC_FETCHFAILED	0x1

#define XEN_MC_NODATA		0x2

#ifndef __ASSEMBLY__

#define VIRQ_MCA VIRQ_ARCH_0


#define MC_TYPE_GLOBAL		0
#define MC_TYPE_BANK		1
#define MC_TYPE_EXTENDED	2
#define MC_TYPE_RECOVERY	3

struct mcinfo_common {
	uint16_t type; 
	uint16_t size; 
};

#define MC_FLAG_CORRECTABLE	(1 << 0)
#define MC_FLAG_UNCORRECTABLE	(1 << 1)
#define MC_FLAG_RECOVERABLE	(1 << 2)
#define MC_FLAG_POLLED		(1 << 3)
#define MC_FLAG_RESET		(1 << 4)
#define MC_FLAG_CMCI		(1 << 5)
#define MC_FLAG_MCE		(1 << 6)


struct mcinfo_global {
	struct mcinfo_common common;

	uint16_t mc_domid; 
	uint16_t mc_vcpuid; 
	uint32_t mc_socketid; 
	uint16_t mc_coreid; 
	uint16_t mc_core_threadid; 
	uint32_t mc_apicid;
	uint32_t mc_flags;
	uint64_t mc_gstatus; 
};


struct mcinfo_bank {
	struct mcinfo_common common;

	uint16_t mc_bank; 
	uint16_t mc_domid; 
	uint64_t mc_status; 
	uint64_t mc_addr; 
	uint64_t mc_misc;
	uint64_t mc_ctrl2;
	uint64_t mc_tsc;
};

struct mcinfo_msr {
	uint64_t reg; 
	uint64_t value; 
};


struct mcinfo_extended {
	struct mcinfo_common common;
	uint32_t mc_msrs; 
	
	struct mcinfo_msr mc_msr[sizeof(void *) * 4];
};




#define REC_ACTION_RECOVERED (0x1 << 0)

#define REC_ACTION_NONE (0x1 << 1)

#define REC_ACTION_NEED_RESET (0x1 << 2)




#define MC_ACTION_PAGE_OFFLINE (0x1 << 0)

#define MC_ACTION_CPU_OFFLINE (0x1 << 1)

#define MC_ACTION_CACHE_SHRINK (0x1 << 2)


struct page_offline_action {
	
	uint64_t mfn;
	uint64_t status;
};

struct cpu_offline_action {
	
	uint32_t mc_socketid;
	uint16_t mc_coreid;
	uint16_t mc_core_threadid;
};

#define MAX_UNION_SIZE 16
struct mcinfo_recovery {
	struct mcinfo_common common;
	uint16_t mc_bank; 
	uint8_t action_flags;
	uint8_t action_types;
	union {
		struct page_offline_action page_retire;
		struct cpu_offline_action cpu_offline;
		uint8_t pad[MAX_UNION_SIZE];
	} action_info;
};


#define MCINFO_MAXSIZE 768
struct mc_info {
	
	uint32_t mi_nentries;
	uint32_t flags;
	uint64_t mi_data[(MCINFO_MAXSIZE - 1) / 8];
};
DEFINE_GUEST_HANDLE_STRUCT(mc_info);

#define __MC_MSR_ARRAYSIZE 8
#define __MC_MSR_MCGCAP 0
#define __MC_NMSRS 1
#define MC_NCAPS 7
struct mcinfo_logical_cpu {
	uint32_t mc_cpunr;
	uint32_t mc_chipid;
	uint16_t mc_coreid;
	uint16_t mc_threadid;
	uint32_t mc_apicid;
	uint32_t mc_clusterid;
	uint32_t mc_ncores;
	uint32_t mc_ncores_active;
	uint32_t mc_nthreads;
	uint32_t mc_cpuid_level;
	uint32_t mc_family;
	uint32_t mc_vendor;
	uint32_t mc_model;
	uint32_t mc_step;
	char mc_vendorid[16];
	char mc_brandid[64];
	uint32_t mc_cpu_caps[MC_NCAPS];
	uint32_t mc_cache_size;
	uint32_t mc_cache_alignment;
	uint32_t mc_nmsrvals;
	struct mcinfo_msr mc_msrvalues[__MC_MSR_ARRAYSIZE];
};
DEFINE_GUEST_HANDLE_STRUCT(mcinfo_logical_cpu);


#define x86_mcinfo_nentries(_mi)    \
	((_mi)->mi_nentries)

#define x86_mcinfo_first(_mi)       \
	((struct mcinfo_common *)(_mi)->mi_data)

#define x86_mcinfo_next(_mic)       \
	((struct mcinfo_common *)((uint8_t *)(_mic) + (_mic)->size))


static inline void x86_mcinfo_lookup(struct mcinfo_common **ret,
				     struct mc_info *mi, uint16_t type)
{
	uint32_t i;
	struct mcinfo_common *mic;
	bool found = 0;

	if (!ret || !mi)
		return;

	mic = x86_mcinfo_first(mi);
	for (i = 0; i < x86_mcinfo_nentries(mi); i++) {
		if (mic->type == type) {
			found = 1;
			break;
		}
		mic = x86_mcinfo_next(mic);
	}

	*ret = found ? mic : NULL;
}


#define XEN_MC_fetch		1
struct xen_mc_fetch {
	
	uint32_t flags;
	uint32_t _pad0;
	
	uint64_t fetch_id;

	
	GUEST_HANDLE(mc_info) data;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_mc_fetch);



#define XEN_MC_notifydomain	2
struct xen_mc_notifydomain {
	
	uint16_t mc_domid; 
	uint16_t mc_vcpuid; 

	
	uint32_t flags;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_mc_notifydomain);

#define XEN_MC_physcpuinfo	3
struct xen_mc_physcpuinfo {
	
	uint32_t ncpus;
	uint32_t _pad0;
	
	GUEST_HANDLE(mcinfo_logical_cpu) info;
};

#define XEN_MC_msrinject	4
#define MC_MSRINJ_MAXMSRS	8
struct xen_mc_msrinject {
	
	uint32_t mcinj_cpunr; 
	uint32_t mcinj_flags; 
	uint32_t mcinj_count; 
	uint32_t _pad0;
	struct mcinfo_msr mcinj_msr[MC_MSRINJ_MAXMSRS];
};


#define MC_MSRINJ_F_INTERPOSE	0x1

#define XEN_MC_mceinject	5
struct xen_mc_mceinject {
	unsigned int mceinj_cpunr; 
};

struct xen_mc {
	uint32_t cmd;
	uint32_t interface_version; 
	union {
		struct xen_mc_fetch        mc_fetch;
		struct xen_mc_notifydomain mc_notifydomain;
		struct xen_mc_physcpuinfo  mc_physcpuinfo;
		struct xen_mc_msrinject    mc_msrinject;
		struct xen_mc_mceinject    mc_mceinject;
	} u;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_mc);


struct xen_mce {
	__u64 status;
	__u64 misc;
	__u64 addr;
	__u64 mcgstatus;
	__u64 ip;
	__u64 tsc;	
	__u64 time;	
	__u8  cpuvendor;	
	__u8  inject_flags;	
	__u16  pad;
	__u32 cpuid;	
	__u8  cs;		
	__u8  bank;	
	__u8  cpu;	
	__u8  finished;   
	__u32 extcpu;	
	__u32 socketid;	
	__u32 apicid;	
	__u64 mcgcap;	
};



#define XEN_MCE_LOG_LEN 32

struct xen_mce_log {
	char signature[12]; 
	unsigned len;	    
	unsigned next;
	unsigned flags;
	unsigned recordlen;	
	struct xen_mce entry[XEN_MCE_LOG_LEN];
};

#define XEN_MCE_OVERFLOW 0		

#define XEN_MCE_LOG_SIGNATURE	"MACHINECHECK"

#define MCE_GET_RECORD_LEN   _IOR('M', 1, int)
#define MCE_GET_LOG_LEN      _IOR('M', 2, int)
#define MCE_GETCLEAR_FLAGS   _IOR('M', 3, int)

#endif 
#endif 
