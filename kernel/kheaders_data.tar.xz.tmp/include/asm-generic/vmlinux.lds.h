

#ifndef LOAD_OFFSET
#define LOAD_OFFSET 0
#endif

#include <linux/export.h>


#define ALIGN_FUNCTION()  . = ALIGN(8)


#if defined(CONFIG_LD_DEAD_CODE_DATA_ELIMINATION) || defined(CONFIG_LTO_CLANG)
#define TEXT_MAIN .text .text.[0-9a-zA-Z_]*
#define TEXT_CFI_MAIN .text.cfi .text.[0-9a-zA-Z_]*.cfi
#define DATA_MAIN .data .data.[0-9a-zA-Z_]*
#define BSS_MAIN .bss .bss.[0-9a-zA-Z_]*
#else
#define TEXT_MAIN .text
#define TEXT_CFI_MAIN .text.cfi
#define DATA_MAIN .data
#define BSS_MAIN .bss
#endif


#define STRUCT_ALIGNMENT 32
#define STRUCT_ALIGN() . = ALIGN(STRUCT_ALIGNMENT)


#ifdef CONFIG_HOTPLUG_CPU
#define CPU_KEEP(sec)    *(.cpu##sec)
#define CPU_DISCARD(sec)
#else
#define CPU_KEEP(sec)
#define CPU_DISCARD(sec) *(.cpu##sec)
#endif

#if defined(CONFIG_MEMORY_HOTPLUG)
#define MEM_KEEP(sec)    *(.mem##sec)
#define MEM_DISCARD(sec)
#else
#define MEM_KEEP(sec)
#define MEM_DISCARD(sec) *(.mem##sec)
#endif

#ifdef CONFIG_FTRACE_MCOUNT_RECORD
#define MCOUNT_REC()	. = ALIGN(8);				\
			VMLINUX_SYMBOL(__start_mcount_loc) = .; \
			KEEP(*(__mcount_loc))			\
			VMLINUX_SYMBOL(__stop_mcount_loc) = .;
#else
#define MCOUNT_REC()
#endif

#ifdef CONFIG_TRACE_BRANCH_PROFILING
#define LIKELY_PROFILE()	VMLINUX_SYMBOL(__start_annotated_branch_profile) = .; \
				*(_ftrace_annotated_branch)			      \
				VMLINUX_SYMBOL(__stop_annotated_branch_profile) = .;
#else
#define LIKELY_PROFILE()
#endif

#ifdef CONFIG_PROFILE_ALL_BRANCHES
#define BRANCH_PROFILE()	VMLINUX_SYMBOL(__start_branch_profile) = .;   \
				*(_ftrace_branch)			      \
				VMLINUX_SYMBOL(__stop_branch_profile) = .;
#else
#define BRANCH_PROFILE()
#endif

#ifdef CONFIG_KPROBES
#define KPROBE_BLACKLIST()	. = ALIGN(8);				      \
				VMLINUX_SYMBOL(__start_kprobe_blacklist) = .; \
				*(_kprobe_blacklist)			      \
				VMLINUX_SYMBOL(__stop_kprobe_blacklist) = .;
#else
#define KPROBE_BLACKLIST()
#endif

#ifdef CONFIG_EVENT_TRACING
#define FTRACE_EVENTS()	. = ALIGN(8);					\
			VMLINUX_SYMBOL(__start_ftrace_events) = .;	\
			KEEP(*(_ftrace_events))				\
			VMLINUX_SYMBOL(__stop_ftrace_events) = .;	\
			VMLINUX_SYMBOL(__start_ftrace_enum_maps) = .;	\
			KEEP(*(_ftrace_enum_map))			\
			VMLINUX_SYMBOL(__stop_ftrace_enum_maps) = .;
#else
#define FTRACE_EVENTS()
#endif

#ifdef CONFIG_TRACING
#define TRACE_PRINTKS() VMLINUX_SYMBOL(__start___trace_bprintk_fmt) = .;      \
			 *(__trace_printk_fmt)  \
			 VMLINUX_SYMBOL(__stop___trace_bprintk_fmt) = .;
#define TRACEPOINT_STR() VMLINUX_SYMBOL(__start___tracepoint_str) = .;	\
			 *(__tracepoint_str)  \
			 VMLINUX_SYMBOL(__stop___tracepoint_str) = .;
#else
#define TRACE_PRINTKS()
#define TRACEPOINT_STR()
#endif

#ifdef CONFIG_FTRACE_SYSCALLS
#define TRACE_SYSCALLS() . = ALIGN(8);					\
			 VMLINUX_SYMBOL(__start_syscalls_metadata) = .;	\
			 *(__syscalls_metadata)				\
			 VMLINUX_SYMBOL(__stop_syscalls_metadata) = .;
#else
#define TRACE_SYSCALLS()
#endif

#ifdef CONFIG_BPF_EVENTS
#define BPF_RAW_TP() STRUCT_ALIGN();                                   \
 			VMLINUX_SYMBOL(__start__bpf_raw_tp) = .;       \
 			KEEP(*(__bpf_raw_tp_map))                      \
 			VMLINUX_SYMBOL(__stop__bpf_raw_tp) = .;
#else
#define BPF_RAW_TP()
#endif

#ifdef CONFIG_SERIAL_EARLYCON
#define EARLYCON_TABLE() . = ALIGN(8);				\
			 VMLINUX_SYMBOL(__earlycon_table) = .;	\
			 *(__earlycon_table)			\
			 VMLINUX_SYMBOL(__earlycon_table_end) = .;
#else
#define EARLYCON_TABLE()
#endif

#define ___OF_TABLE(cfg, name)	_OF_TABLE_##cfg(name)
#define __OF_TABLE(cfg, name)	___OF_TABLE(cfg, name)
#define OF_TABLE(cfg, name)	__OF_TABLE(IS_ENABLED(cfg), name)
#define _OF_TABLE_0(name)
#define _OF_TABLE_1(name)						\
	. = ALIGN(8);							\
	VMLINUX_SYMBOL(__##name##_of_table) = .;			\
	KEEP(*(__##name##_of_table))					\
	KEEP(*(__##name##_of_table_end))

#define CLKSRC_OF_TABLES()	OF_TABLE(CONFIG_CLKSRC_OF, clksrc)
#define IRQCHIP_OF_MATCH_TABLE() OF_TABLE(CONFIG_IRQCHIP, irqchip)
#define CLK_OF_TABLES()		OF_TABLE(CONFIG_COMMON_CLK, clk)
#define IOMMU_OF_TABLES()	OF_TABLE(CONFIG_OF_IOMMU, iommu)
#define RESERVEDMEM_OF_TABLES()	OF_TABLE(CONFIG_OF_RESERVED_MEM, reservedmem)
#define CPU_METHOD_OF_TABLES()	OF_TABLE(CONFIG_SMP, cpu_method)
#define CPUIDLE_METHOD_OF_TABLES() OF_TABLE(CONFIG_CPU_IDLE, cpuidle_method)

#ifdef CONFIG_ACPI
#define ACPI_PROBE_TABLE(name)						\
	. = ALIGN(8);							\
	VMLINUX_SYMBOL(__##name##_acpi_probe_table) = .;		\
	*(__##name##_acpi_probe_table)					\
	VMLINUX_SYMBOL(__##name##_acpi_probe_table_end) = .;
#else
#define ACPI_PROBE_TABLE(name)
#endif

#define KERNEL_DTB()							\
	STRUCT_ALIGN();							\
	VMLINUX_SYMBOL(__dtb_start) = .;				\
	*(.dtb.init.rodata)						\
	VMLINUX_SYMBOL(__dtb_end) = .;


#define DATA_DATA							\
	*(DATA_MAIN)							\
	*(.ref.data)							\
	*(.data..shared_aligned) 			\
	MEM_KEEP(init.data)						\
	MEM_KEEP(exit.data)						\
	*(.data.unlikely)						\
	STRUCT_ALIGN();							\
	*(__tracepoints)						\
					\
	. = ALIGN(8);                                                   \
	VMLINUX_SYMBOL(__start___jump_table) = .;                       \
	*(__jump_table)                                                 \
	VMLINUX_SYMBOL(__stop___jump_table) = .;                        \
	. = ALIGN(8);							\
	VMLINUX_SYMBOL(__start___verbose) = .;                          \
	*(__verbose)                                                    \
	VMLINUX_SYMBOL(__stop___verbose) = .;				\
	LIKELY_PROFILE()		       				\
	BRANCH_PROFILE()						\
	TRACE_PRINTKS()							\
	BPF_RAW_TP()                                                    \
	TRACEPOINT_STR()


#define NOSAVE_DATA							\
	. = ALIGN(PAGE_SIZE);						\
	VMLINUX_SYMBOL(__nosave_begin) = .;				\
	*(.data..nosave)						\
	. = ALIGN(PAGE_SIZE);						\
	VMLINUX_SYMBOL(__nosave_end) = .;

#define PAGE_ALIGNED_DATA(page_align)					\
	. = ALIGN(page_align);						\
	*(.data..page_aligned)						\
	. = ALIGN(page_align);

#define READ_MOSTLY_DATA(align)						\
	. = ALIGN(align);						\
	*(.data..read_mostly)						\
	. = ALIGN(align);

#define CACHELINE_ALIGNED_DATA(align)					\
	. = ALIGN(align);						\
	*(.data..cacheline_aligned)

#define INIT_TASK_DATA(align)						\
	. = ALIGN(align);						\
	VMLINUX_SYMBOL(__start_init_task) = .;				\
	*(.data..init_task)						\
	VMLINUX_SYMBOL(__end_init_task) = .;


#ifndef RO_AFTER_INIT_DATA
#define RO_AFTER_INIT_DATA						\
	__start_data_ro_after_init = .;					\
	*(.data..ro_after_init)						\
	__end_data_ro_after_init = .;
#endif


#define RO_DATA_SECTION(align)						\
	. = ALIGN((align));						\
	.rodata           : AT(ADDR(.rodata) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__start_rodata) = .;			\
		*(.rodata) *(.rodata.*)					\
		*(__vermagic)			\
		. = ALIGN(8);						\
		VMLINUX_SYMBOL(__start___tracepoints_ptrs) = .;		\
		*(__tracepoints_ptrs)	\
		VMLINUX_SYMBOL(__stop___tracepoints_ptrs) = .;		\
		*(__tracepoints_strings)	\
	}								\
									\
	.rodata1          : AT(ADDR(.rodata1) - LOAD_OFFSET) {		\
		*(.rodata1)						\
		RO_AFTER_INIT_DATA		\
	}								\
									\
	BUG_TABLE							\
									\
							\
	.pci_fixup        : AT(ADDR(.pci_fixup) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start_pci_fixups_early) = .;		\
		KEEP(*(.pci_fixup_early))				\
		VMLINUX_SYMBOL(__end_pci_fixups_early) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_header) = .;		\
		KEEP(*(.pci_fixup_header))				\
		VMLINUX_SYMBOL(__end_pci_fixups_header) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_final) = .;		\
		KEEP(*(.pci_fixup_final))				\
		VMLINUX_SYMBOL(__end_pci_fixups_final) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_enable) = .;		\
		KEEP(*(.pci_fixup_enable))				\
		VMLINUX_SYMBOL(__end_pci_fixups_enable) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_resume) = .;		\
		KEEP(*(.pci_fixup_resume))				\
		VMLINUX_SYMBOL(__end_pci_fixups_resume) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_resume_early) = .;	\
		KEEP(*(.pci_fixup_resume_early))			\
		VMLINUX_SYMBOL(__end_pci_fixups_resume_early) = .;	\
		VMLINUX_SYMBOL(__start_pci_fixups_suspend) = .;		\
		KEEP(*(.pci_fixup_suspend))				\
		VMLINUX_SYMBOL(__end_pci_fixups_suspend) = .;		\
		VMLINUX_SYMBOL(__start_pci_fixups_suspend_late) = .;	\
		KEEP(*(.pci_fixup_suspend_late))			\
		VMLINUX_SYMBOL(__end_pci_fixups_suspend_late) = .;	\
	}								\
									\
						\
	.builtin_fw        : AT(ADDR(.builtin_fw) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start_builtin_fw) = .;			\
		*(.builtin_fw)						\
		VMLINUX_SYMBOL(__end_builtin_fw) = .;			\
	}								\
									\
	TRACEDATA							\
									\
				\
	__ksymtab         : AT(ADDR(__ksymtab) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__start___ksymtab) = .;			\
		KEEP(*(SORT(___ksymtab+*)))				\
		VMLINUX_SYMBOL(__stop___ksymtab) = .;			\
	}								\
									\
				\
	__ksymtab_gpl     : AT(ADDR(__ksymtab_gpl) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start___ksymtab_gpl) = .;		\
		KEEP(*(SORT(___ksymtab_gpl+*)))				\
		VMLINUX_SYMBOL(__stop___ksymtab_gpl) = .;		\
	}								\
									\
			\
	__ksymtab_unused  : AT(ADDR(__ksymtab_unused) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start___ksymtab_unused) = .;		\
		KEEP(*(SORT(___ksymtab_unused+*)))			\
		VMLINUX_SYMBOL(__stop___ksymtab_unused) = .;		\
	}								\
									\
			\
	__ksymtab_unused_gpl : AT(ADDR(__ksymtab_unused_gpl) - LOAD_OFFSET) { \
		VMLINUX_SYMBOL(__start___ksymtab_unused_gpl) = .;	\
		KEEP(*(SORT(___ksymtab_unused_gpl+*)))			\
		VMLINUX_SYMBOL(__stop___ksymtab_unused_gpl) = .;	\
	}								\
									\
			\
	__ksymtab_gpl_future : AT(ADDR(__ksymtab_gpl_future) - LOAD_OFFSET) { \
		VMLINUX_SYMBOL(__start___ksymtab_gpl_future) = .;	\
		KEEP(*(SORT(___ksymtab_gpl_future+*)))			\
		VMLINUX_SYMBOL(__stop___ksymtab_gpl_future) = .;	\
	}								\
									\
				\
	__kcrctab         : AT(ADDR(__kcrctab) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__start___kcrctab) = .;			\
		KEEP(*(SORT(___kcrctab+*)))				\
		VMLINUX_SYMBOL(__stop___kcrctab) = .;			\
	}								\
									\
				\
	__kcrctab_gpl     : AT(ADDR(__kcrctab_gpl) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start___kcrctab_gpl) = .;		\
		KEEP(*(SORT(___kcrctab_gpl+*)))				\
		VMLINUX_SYMBOL(__stop___kcrctab_gpl) = .;		\
	}								\
									\
			\
	__kcrctab_unused  : AT(ADDR(__kcrctab_unused) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__start___kcrctab_unused) = .;		\
		KEEP(*(SORT(___kcrctab_unused+*)))			\
		VMLINUX_SYMBOL(__stop___kcrctab_unused) = .;		\
	}								\
									\
			\
	__kcrctab_unused_gpl : AT(ADDR(__kcrctab_unused_gpl) - LOAD_OFFSET) { \
		VMLINUX_SYMBOL(__start___kcrctab_unused_gpl) = .;	\
		KEEP(*(SORT(___kcrctab_unused_gpl+*)))			\
		VMLINUX_SYMBOL(__stop___kcrctab_unused_gpl) = .;	\
	}								\
									\
			\
	__kcrctab_gpl_future : AT(ADDR(__kcrctab_gpl_future) - LOAD_OFFSET) { \
		VMLINUX_SYMBOL(__start___kcrctab_gpl_future) = .;	\
		KEEP(*(SORT(___kcrctab_gpl_future+*)))			\
		VMLINUX_SYMBOL(__stop___kcrctab_gpl_future) = .;	\
	}								\
									\
					\
        __ksymtab_strings : AT(ADDR(__ksymtab_strings) - LOAD_OFFSET) {	\
		KEEP(*(__ksymtab_strings))				\
	}								\
									\
							\
	__init_rodata : AT(ADDR(__init_rodata) - LOAD_OFFSET) {		\
		*(.ref.rodata)						\
		MEM_KEEP(init.rodata)					\
		MEM_KEEP(exit.rodata)					\
	}								\
									\
					\
	__param : AT(ADDR(__param) - LOAD_OFFSET) {			\
		VMLINUX_SYMBOL(__start___param) = .;			\
		KEEP(*(__param))					\
		VMLINUX_SYMBOL(__stop___param) = .;			\
	}								\
									\
						\
	__modver : AT(ADDR(__modver) - LOAD_OFFSET) {			\
		VMLINUX_SYMBOL(__start___modver) = .;			\
		*(__modver)						\
		VMLINUX_SYMBOL(__stop___modver) = .;			\
		. = ALIGN((align));					\
		VMLINUX_SYMBOL(__end_rodata) = .;			\
	}								\
	. = ALIGN((align));


#define RODATA          RO_DATA_SECTION(4096)
#define RO_DATA(align)  RO_DATA_SECTION(align)

#define SECURITY_INIT							\
	.security_initcall.init : AT(ADDR(.security_initcall.init) - LOAD_OFFSET) { \
		VMLINUX_SYMBOL(__security_initcall_start) = .;		\
		KEEP(*(.security_initcall.init))			\
		VMLINUX_SYMBOL(__security_initcall_end) = .;		\
	}


#define TEXT_TEXT							\
		ALIGN_FUNCTION();					\
		*(.text.hot .text.hot.*)				\
		*(TEXT_MAIN .text.fixup)				\
		*(.text.unlikely .text.unlikely.*)			\
		*(.text.unknown .text.unknown.*)			\
		*(.text..ftrace)					\
		*(TEXT_CFI_MAIN) 					\
		*(.ref.text)						\
		*(.text.asan.* .text.tsan.*)				\
	MEM_KEEP(init.text)						\
	MEM_KEEP(exit.text)						\



#define SCHED_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__sched_text_start) = .;			\
		*(.sched.text)						\
		VMLINUX_SYMBOL(__sched_text_end) = .;


#define LOCK_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__lock_text_start) = .;			\
		*(.spinlock.text)					\
		VMLINUX_SYMBOL(__lock_text_end) = .;

#define CPUIDLE_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__cpuidle_text_start) = .;		\
		*(.cpuidle.text)					\
		VMLINUX_SYMBOL(__cpuidle_text_end) = .;

#define KPROBES_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__kprobes_text_start) = .;		\
		*(.kprobes.text)					\
		VMLINUX_SYMBOL(__kprobes_text_end) = .;

#define ENTRY_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__entry_text_start) = .;			\
		*(.entry.text)						\
		VMLINUX_SYMBOL(__entry_text_end) = .;

#define IRQENTRY_TEXT							\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__irqentry_text_start) = .;		\
		*(.irqentry.text)					\
		VMLINUX_SYMBOL(__irqentry_text_end) = .;

#define SOFTIRQENTRY_TEXT						\
		ALIGN_FUNCTION();					\
		VMLINUX_SYMBOL(__softirqentry_text_start) = .;		\
		*(.softirqentry.text)					\
		VMLINUX_SYMBOL(__softirqentry_text_end) = .;


#define HEAD_TEXT  KEEP(*(.head.text))

#define HEAD_TEXT_SECTION							\
	.head.text : AT(ADDR(.head.text) - LOAD_OFFSET) {		\
		HEAD_TEXT						\
	}


#define EXCEPTION_TABLE(align)						\
	. = ALIGN(align);						\
	__ex_table : AT(ADDR(__ex_table) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__start___ex_table) = .;			\
		KEEP(*(__ex_table))					\
		VMLINUX_SYMBOL(__stop___ex_table) = .;			\
	}


#define INIT_TASK_DATA_SECTION(align)					\
	. = ALIGN(align);						\
	.data..init_task :  AT(ADDR(.data..init_task) - LOAD_OFFSET) {	\
		INIT_TASK_DATA(align)					\
	}

#ifdef CONFIG_CONSTRUCTORS
#define KERNEL_CTORS()	. = ALIGN(8);			   \
			VMLINUX_SYMBOL(__ctors_start) = .; \
			*(.ctors)			   \
			*(SORT(.init_array.*))		   \
			*(.init_array)			   \
			VMLINUX_SYMBOL(__ctors_end) = .;
#else
#define KERNEL_CTORS()
#endif


#define INIT_DATA							\
	KEEP(*(SORT(___kentry+*)))					\
	*(.init.data)							\
	MEM_DISCARD(init.data)						\
	KERNEL_CTORS()							\
	MCOUNT_REC()							\
	*(.init.rodata .init.rodata.*)					\
	FTRACE_EVENTS()							\
	TRACE_SYSCALLS()						\
	KPROBE_BLACKLIST()						\
	MEM_DISCARD(init.rodata)					\
	CLK_OF_TABLES()							\
	RESERVEDMEM_OF_TABLES()						\
	CLKSRC_OF_TABLES()						\
	IOMMU_OF_TABLES()						\
	CPU_METHOD_OF_TABLES()						\
	CPUIDLE_METHOD_OF_TABLES()					\
	KERNEL_DTB()							\
	IRQCHIP_OF_MATCH_TABLE()					\
	ACPI_PROBE_TABLE(irqchip)					\
	ACPI_PROBE_TABLE(clksrc)					\
	EARLYCON_TABLE()

#define INIT_TEXT							\
	*(.init.text .init.text.*)					\
	*(.text.startup)						\
	MEM_DISCARD(init.text)

#define EXIT_DATA							\
	*(.exit.data)							\
	*(.fini_array)							\
	*(.dtors)							\
	MEM_DISCARD(exit.data)						\
	MEM_DISCARD(exit.rodata)

#define EXIT_TEXT							\
	*(.exit.text)							\
	*(.text.exit)							\
	MEM_DISCARD(exit.text)

#define EXIT_CALL							\
	KEEP(*(.exitcall.exit))


#define SBSS(sbss_align)						\
	. = ALIGN(sbss_align);						\
	.sbss : AT(ADDR(.sbss) - LOAD_OFFSET) {				\
		*(.sbss)						\
		*(.scommon)						\
	}


#ifndef BSS_FIRST_SECTIONS
#define BSS_FIRST_SECTIONS
#endif

#define BSS(bss_align)							\
	. = ALIGN(bss_align);						\
	.bss : AT(ADDR(.bss) - LOAD_OFFSET) {				\
		BSS_FIRST_SECTIONS					\
		. = ALIGN(PAGE_SIZE);					\
		*(.bss..page_aligned)					\
		. = ALIGN(PAGE_SIZE);					\
		*(.dynbss)						\
		*(BSS_MAIN)						\
		*(COMMON)						\
	}


#define DWARF_DEBUG							\
								\
		.debug          0 : { *(.debug) }			\
		.line           0 : { *(.line) }			\
						\
		.debug_srcinfo  0 : { *(.debug_srcinfo) }		\
		.debug_sfnames  0 : { *(.debug_sfnames) }		\
						\
		.debug_aranges  0 : { *(.debug_aranges) }		\
		.debug_pubnames 0 : { *(.debug_pubnames) }		\
								\
		.debug_info     0 : { *(.debug_info			\
				.gnu.linkonce.wi.*) }			\
		.debug_abbrev   0 : { *(.debug_abbrev) }		\
		.debug_line     0 : { *(.debug_line) }			\
		.debug_frame    0 : { *(.debug_frame) }			\
		.debug_str      0 : { *(.debug_str) }			\
		.debug_loc      0 : { *(.debug_loc) }			\
		.debug_macinfo  0 : { *(.debug_macinfo) }		\
					\
		.debug_weaknames 0 : { *(.debug_weaknames) }		\
		.debug_funcnames 0 : { *(.debug_funcnames) }		\
		.debug_typenames 0 : { *(.debug_typenames) }		\
		.debug_varnames  0 : { *(.debug_varnames) }		\

		
#define STABS_DEBUG							\
		.stab 0 : { *(.stab) }					\
		.stabstr 0 : { *(.stabstr) }				\
		.stab.excl 0 : { *(.stab.excl) }			\
		.stab.exclstr 0 : { *(.stab.exclstr) }			\
		.stab.index 0 : { *(.stab.index) }			\
		.stab.indexstr 0 : { *(.stab.indexstr) }		\
		.comment 0 : { *(.comment) }

#ifdef CONFIG_GENERIC_BUG
#define BUG_TABLE							\
	. = ALIGN(8);							\
	__bug_table : AT(ADDR(__bug_table) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__start___bug_table) = .;		\
		KEEP(*(__bug_table))					\
		VMLINUX_SYMBOL(__stop___bug_table) = .;			\
	}
#else
#define BUG_TABLE
#endif

#ifdef CONFIG_PM_TRACE
#define TRACEDATA							\
	. = ALIGN(4);							\
	.tracedata : AT(ADDR(.tracedata) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(__tracedata_start) = .;			\
		*(.tracedata)						\
		VMLINUX_SYMBOL(__tracedata_end) = .;			\
	}
#else
#define TRACEDATA
#endif

#define NOTES								\
	.notes : AT(ADDR(.notes) - LOAD_OFFSET) {			\
		VMLINUX_SYMBOL(__start_notes) = .;			\
		*(.note.*)						\
		VMLINUX_SYMBOL(__stop_notes) = .;			\
	}

#define INIT_SETUP(initsetup_align)					\
		. = ALIGN(initsetup_align);				\
		VMLINUX_SYMBOL(__setup_start) = .;			\
		KEEP(*(.init.setup))					\
		VMLINUX_SYMBOL(__setup_end) = .;

#define INIT_CALLS_LEVEL(level)						\
		VMLINUX_SYMBOL(__initcall##level##_start) = .;		\
		KEEP(*(.initcall##level##.init))			\
		KEEP(*(.initcall##level##s.init))			\

#ifdef CONFIG_DEFERRED_INITCALLS
#define DEFERRED_INITCALLS(level)					\
		VMLINUX_SYMBOL(__deferred_initcall_start) = .;		\
		KEEP(*(.deferred_initcall##level##.init))		\
		KEEP(*(.deferred_initcall##level##s.init))		\
		VMLINUX_SYMBOL(__deferred_initcall_end) = .;
#else
#define DEFERRED_INITCALLS(level)
#endif

#define INIT_CALLS							\
		VMLINUX_SYMBOL(__initcall_start) = .;			\
		KEEP(*(.initcallearly.init))				\
		INIT_CALLS_LEVEL(0)					\
		INIT_CALLS_LEVEL(1)					\
		INIT_CALLS_LEVEL(2)					\
		INIT_CALLS_LEVEL(3)					\
		INIT_CALLS_LEVEL(4)					\
		INIT_CALLS_LEVEL(5)					\
		INIT_CALLS_LEVEL(rootfs)				\
		INIT_CALLS_LEVEL(6)					\
		INIT_CALLS_LEVEL(7)					\
		VMLINUX_SYMBOL(__initcall_end) = .;			\
		DEFERRED_INITCALLS(0)

#define CON_INITCALL							\
		VMLINUX_SYMBOL(__con_initcall_start) = .;		\
		KEEP(*(.con_initcall.init))				\
		VMLINUX_SYMBOL(__con_initcall_end) = .;

#define SECURITY_INITCALL						\
		VMLINUX_SYMBOL(__security_initcall_start) = .;		\
		KEEP(*(.security_initcall.init))			\
		VMLINUX_SYMBOL(__security_initcall_end) = .;

#ifdef CONFIG_BLK_DEV_INITRD
#define INIT_RAM_FS							\
	. = ALIGN(4);							\
	VMLINUX_SYMBOL(__initramfs_start) = .;				\
	KEEP(*(.init.ramfs))						\
	. = ALIGN(8);							\
	KEEP(*(.init.ramfs.info))
#else
#define INIT_RAM_FS
#endif


#define DISCARDS							\
	/DISCARD/ : {							\
	EXIT_TEXT							\
	EXIT_DATA							\
	EXIT_CALL							\
	*(.discard)							\
	*(.discard.*)							\
	}


#define PERCPU_INPUT(cacheline)						\
	VMLINUX_SYMBOL(__per_cpu_start) = .;				\
	VMLINUX_SYMBOL(__per_cpu_user_mapped_start) = .;		\
	*(.data..percpu..first)						\
	. = ALIGN(cacheline);						\
	*(.data..percpu..user_mapped)					\
	*(.data..percpu..user_mapped..shared_aligned)			\
	. = ALIGN(PAGE_SIZE);						\
	*(.data..percpu..user_mapped..page_aligned)			\
	VMLINUX_SYMBOL(__per_cpu_user_mapped_end) = .;			\
	. = ALIGN(PAGE_SIZE);						\
	*(.data..percpu..page_aligned)					\
	. = ALIGN(cacheline);						\
	*(.data..percpu..read_mostly)					\
	. = ALIGN(cacheline);						\
	*(.data..percpu)						\
	*(.data..percpu..shared_aligned)				\
	VMLINUX_SYMBOL(__per_cpu_end) = .;


#define PERCPU_VADDR(cacheline, vaddr, phdr)				\
	VMLINUX_SYMBOL(__per_cpu_load) = .;				\
	.data..percpu vaddr : AT(VMLINUX_SYMBOL(__per_cpu_load)		\
				- LOAD_OFFSET) {			\
		PERCPU_INPUT(cacheline)					\
	} phdr								\
	. = VMLINUX_SYMBOL(__per_cpu_load) + SIZEOF(.data..percpu);


#define PERCPU_SECTION(cacheline)					\
	. = ALIGN(PAGE_SIZE);						\
	.data..percpu	: AT(ADDR(.data..percpu) - LOAD_OFFSET) {	\
		VMLINUX_SYMBOL(__per_cpu_load) = .;			\
		PERCPU_INPUT(cacheline)					\
	}






#define RW_DATA_SECTION(cacheline, pagealigned, inittask)		\
	. = ALIGN(PAGE_SIZE);						\
	.data : AT(ADDR(.data) - LOAD_OFFSET) {				\
		INIT_TASK_DATA(inittask)				\
		NOSAVE_DATA						\
		PAGE_ALIGNED_DATA(pagealigned)				\
		CACHELINE_ALIGNED_DATA(cacheline)			\
		READ_MOSTLY_DATA(cacheline)				\
		DATA_DATA						\
		CONSTRUCTORS						\
	}

#define INIT_TEXT_SECTION(inittext_align)				\
	. = ALIGN(inittext_align);					\
	.init.text : AT(ADDR(.init.text) - LOAD_OFFSET) {		\
		VMLINUX_SYMBOL(_sinittext) = .;				\
		INIT_TEXT						\
		VMLINUX_SYMBOL(_einittext) = .;				\
	}

#define INIT_DATA_SECTION(initsetup_align)				\
	.init.data : AT(ADDR(.init.data) - LOAD_OFFSET) {		\
		INIT_DATA						\
		INIT_SETUP(initsetup_align)				\
		INIT_CALLS						\
		CON_INITCALL						\
		SECURITY_INITCALL					\
		INIT_RAM_FS						\
	}

#define BSS_SECTION(sbss_align, bss_align, stop_align)			\
	. = ALIGN(sbss_align);						\
	VMLINUX_SYMBOL(__bss_start) = .;				\
	SBSS(sbss_align)						\
	BSS(bss_align)							\
	. = ALIGN(stop_align);						\
	VMLINUX_SYMBOL(__bss_stop) = .;
