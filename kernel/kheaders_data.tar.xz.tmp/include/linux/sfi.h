



#ifndef _LINUX_SFI_H
#define _LINUX_SFI_H

#include <linux/init.h>
#include <linux/types.h>


#define SFI_SIG_SYST		"SYST"
#define SFI_SIG_FREQ		"FREQ"
#define SFI_SIG_IDLE		"IDLE"
#define SFI_SIG_CPUS		"CPUS"
#define SFI_SIG_MTMR		"MTMR"
#define SFI_SIG_MRTC		"MRTC"
#define SFI_SIG_MMAP		"MMAP"
#define SFI_SIG_APIC		"APIC"
#define SFI_SIG_XSDT		"XSDT"
#define SFI_SIG_WAKE		"WAKE"
#define SFI_SIG_DEVS		"DEVS"
#define SFI_SIG_GPIO		"GPIO"

#define SFI_SIGNATURE_SIZE	4
#define SFI_OEM_ID_SIZE		6
#define SFI_OEM_TABLE_ID_SIZE	8

#define SFI_NAME_LEN		16

#define SFI_SYST_SEARCH_BEGIN		0x000E0000
#define SFI_SYST_SEARCH_END		0x000FFFFF

#define SFI_GET_NUM_ENTRIES(ptable, entry_type) \
	((ptable->header.len - sizeof(struct sfi_table_header)) / \
	(sizeof(entry_type)))

struct sfi_table_header {
	char	sig[SFI_SIGNATURE_SIZE];
	u32	len;
	u8	rev;
	u8	csum;
	char	oem_id[SFI_OEM_ID_SIZE];
	char	oem_table_id[SFI_OEM_TABLE_ID_SIZE];
} __packed;

struct sfi_table_simple {
	struct sfi_table_header		header;
	u64				pentry[1];
} __packed;


struct sfi_mem_entry {
	u32	type;
	u64	phys_start;
	u64	virt_start;
	u64	pages;
	u64	attrib;
} __packed;

struct sfi_cpu_table_entry {
	u32	apic_id;
} __packed;

struct sfi_cstate_table_entry {
	u32	hint;		
	u32	latency;	
} __packed;

struct sfi_apic_table_entry {
	u64	phys_addr;	
} __packed;

struct sfi_freq_table_entry {
	u32	freq_mhz;	
	u32	latency;	
	u32	ctrl_val;	
} __packed;

struct sfi_wake_table_entry {
	u64	phys_addr;	
} __packed;

struct sfi_timer_table_entry {
	u64	phys_addr;	
	u32	freq_hz;	
	u32	irq;
} __packed;

struct sfi_rtc_table_entry {
	u64	phys_addr;	
	u32	irq;
} __packed;

struct sfi_device_table_entry {
	u8	type;		
#define SFI_DEV_TYPE_SPI	0
#define SFI_DEV_TYPE_I2C	1
#define SFI_DEV_TYPE_UART	2
#define SFI_DEV_TYPE_HSI	3
#define SFI_DEV_TYPE_IPC	4
#define SFI_DEV_TYPE_SD		5

	u8	host_num;	
	u16	addr;
	u8	irq;
	u32	max_freq;
	char	name[SFI_NAME_LEN];
} __packed;

struct sfi_gpio_table_entry {
	char	controller_name[SFI_NAME_LEN];
	u16	pin_no;
	char	pin_name[SFI_NAME_LEN];
} __packed;

typedef int (*sfi_table_handler) (struct sfi_table_header *table);

#ifdef CONFIG_SFI
extern void __init sfi_init(void);
extern int __init sfi_platform_init(void);
extern void __init sfi_init_late(void);
extern int sfi_table_parse(char *signature, char *oem_id, char *oem_table_id,
				sfi_table_handler handler);

extern int sfi_disabled;
static inline void disable_sfi(void)
{
	sfi_disabled = 1;
}

#else 

static inline void sfi_init(void)
{
}

static inline void sfi_init_late(void)
{
}

#define sfi_disabled	0

static inline int sfi_table_parse(char *signature, char *oem_id,
					char *oem_table_id,
					sfi_table_handler handler)
{
	return -1;
}

#endif 

#endif 
