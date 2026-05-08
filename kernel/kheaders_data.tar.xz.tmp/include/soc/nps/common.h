

#ifndef SOC_NPS_COMMON_H
#define SOC_NPS_COMMON_H

#ifdef CONFIG_SMP
#define NPS_IPI_IRQ					5
#endif

#define NPS_HOST_REG_BASE			0xF6000000

#define NPS_MSU_BLKID				0x018

#define CTOP_INST_RSPI_GIC_0_R12		0x3C56117E
#define CTOP_INST_MOV2B_FLIP_R3_B1_B2_INST	0x5B60
#define CTOP_INST_MOV2B_FLIP_R3_B1_B2_LIMM	0x00010422

#ifndef AUX_IENABLE
#define AUX_IENABLE				0x40c
#endif

#define CTOP_AUX_IACK				(0xFFFFF800 + 0x088)

#ifndef __ASSEMBLY__


#ifdef CONFIG_ARC
static inline void nps_ack_gic(void)
{
	__asm__ __volatile__ (
	"       .word %0\n"
	:
	: "i"(CTOP_INST_RSPI_GIC_0_R12)
	: "memory");
}
#else
static inline void nps_ack_gic(void) { }
#define write_aux_reg(r, v)
#define read_aux_reg(r) 0
#endif


struct global_id {
	union {
		struct {
#ifdef CONFIG_EZNPS_MTM_EXT
			u32 __reserved:20, cluster:4, core:4, thread:4;
#else
			u32 __reserved:24, cluster:4, core:4;
#endif
		};
		u32 value;
	};
};


static inline int nps_cluster_logic_to_phys(int cluster)
{
#ifdef __arc__
	 __asm__ __volatile__(
	"       mov r3,%0\n"
	"       .short %1\n"
	"       .word %2\n"
	"       mov %0,r3\n"
	: "+r"(cluster)
	: "i"(CTOP_INST_MOV2B_FLIP_R3_B1_B2_INST),
	  "i"(CTOP_INST_MOV2B_FLIP_R3_B1_B2_LIMM)
	: "r3");
#endif

	return cluster;
}

#define NPS_CPU_TO_CLUSTER_NUM(cpu) \
	({ struct global_id gid; gid.value = cpu; \
		nps_cluster_logic_to_phys(gid.cluster); })

struct nps_host_reg_address {
	union {
		struct {
			u32 base:8, cl_x:4, cl_y:4,
			blkid:6, reg:8, __reserved:2;
		};
		u32 value;
	};
};

struct nps_host_reg_address_non_cl {
	union {
		struct {
			u32 base:7, blkid:11, reg:12, __reserved:2;
		};
		u32 value;
	};
};

static inline void *nps_host_reg_non_cl(u32 blkid, u32 reg)
{
	struct nps_host_reg_address_non_cl reg_address;

	reg_address.value = NPS_HOST_REG_BASE;
	reg_address.blkid = blkid;
	reg_address.reg = reg;

	return (void *)reg_address.value;
}

static inline void *nps_host_reg(u32 cpu, u32 blkid, u32 reg)
{
	struct nps_host_reg_address reg_address;
	u32 cl = NPS_CPU_TO_CLUSTER_NUM(cpu);

	reg_address.value = NPS_HOST_REG_BASE;
	reg_address.cl_x  = (cl >> 2) & 0x3;
	reg_address.cl_y  = cl & 0x3;
	reg_address.blkid = blkid;
	reg_address.reg   = reg;

	return (void *)reg_address.value;
}
#endif 

#endif 
