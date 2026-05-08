

#ifndef _ASM_ACPI_H
#define _ASM_ACPI_H

#include <linux/memblock.h>
#include <linux/psci.h>

#include <asm/cputype.h>
#include <asm/smp_plat.h>


#define ACPI_MADT_GICC_LENGTH	\
	(acpi_gbl_FADT.header.revision < 6 ? 76 : 80)

#define BAD_MADT_GICC_ENTRY(entry, end)					\
	(!(entry) || (entry)->header.length != ACPI_MADT_GICC_LENGTH ||	\
	(unsigned long)(entry) + ACPI_MADT_GICC_LENGTH > (end))


#ifdef	CONFIG_ACPI

static inline void __iomem *acpi_os_ioremap(acpi_physical_address phys,
					    acpi_size size)
{
	
	if (!memblock_is_memory(phys))
		return ioremap(phys, size);

	return ioremap_cache(phys, size);
}
#define acpi_os_ioremap acpi_os_ioremap

typedef u64 phys_cpuid_t;
#define PHYS_CPUID_INVALID INVALID_HWID

#define acpi_strict 1	
extern int acpi_disabled;
extern int acpi_noirq;
extern int acpi_pci_disabled;

static inline void disable_acpi(void)
{
	acpi_disabled = 1;
	acpi_pci_disabled = 1;
	acpi_noirq = 1;
}

static inline void enable_acpi(void)
{
	acpi_disabled = 0;
	acpi_pci_disabled = 0;
	acpi_noirq = 0;
}


#define cpu_physical_id(cpu) cpu_logical_map(cpu)


static inline bool acpi_has_cpu_in_madt(void)
{
	return true;
}

static inline void arch_fix_phys_package_id(int num, u32 slot) { }
void __init acpi_init_cpus(void);

#else
static inline void acpi_init_cpus(void) { }
#endif 

#ifdef CONFIG_ARM64_ACPI_PARKING_PROTOCOL
bool acpi_parking_protocol_valid(int cpu);
void __init
acpi_set_mailbox_entry(int cpu, struct acpi_madt_generic_interrupt *processor);
#else
static inline bool acpi_parking_protocol_valid(int cpu) { return false; }
static inline void
acpi_set_mailbox_entry(int cpu, struct acpi_madt_generic_interrupt *processor)
{}
#endif

static inline const char *acpi_get_enable_method(int cpu)
{
	if (acpi_psci_present())
		return "psci";

	if (acpi_parking_protocol_valid(cpu))
		return "parking-protocol";

	return NULL;
}

#ifdef	CONFIG_ACPI_APEI
pgprot_t arch_apei_get_mem_attribute(phys_addr_t addr);
#endif

#ifdef CONFIG_ACPI_NUMA
int arm64_acpi_numa_init(void);
int acpi_numa_get_nid(unsigned int cpu, u64 hwid);
#else
static inline int arm64_acpi_numa_init(void) { return -ENOSYS; }
static inline int acpi_numa_get_nid(unsigned int cpu, u64 hwid) { return NUMA_NO_NODE; }
#endif 

#define ACPI_TABLE_UPGRADE_MAX_PHYS MEMBLOCK_ALLOC_ACCESSIBLE

#endif 
