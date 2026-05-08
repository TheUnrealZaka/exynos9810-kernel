#ifndef PAGE_FLAGS_LAYOUT_H
#define PAGE_FLAGS_LAYOUT_H

#include <linux/numa.h>
#include <generated/bounds.h>


#if MAX_NR_ZONES < 2
#define ZONES_SHIFT 0
#elif MAX_NR_ZONES <= 2
#define ZONES_SHIFT 1
#elif MAX_NR_ZONES <= 4
#define ZONES_SHIFT 2
#elif MAX_NR_ZONES <= 8
#define ZONES_SHIFT 3
#else
#error ZONES_SHIFT -- too many zones configured adjust calculation
#endif

#ifdef CONFIG_SPARSEMEM
#include <asm/sparsemem.h>


#define SECTIONS_SHIFT	(MAX_PHYSMEM_BITS - SECTION_SIZE_BITS)

#endif 


#if defined(CONFIG_SPARSEMEM) && !defined(CONFIG_SPARSEMEM_VMEMMAP)
#define SECTIONS_WIDTH		SECTIONS_SHIFT
#else
#define SECTIONS_WIDTH		0
#endif

#define ZONES_WIDTH		ZONES_SHIFT

#if SECTIONS_WIDTH+ZONES_WIDTH+NODES_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
#define NODES_WIDTH		NODES_SHIFT
#else
#ifdef CONFIG_SPARSEMEM_VMEMMAP
#error "Vmemmap: No space for nodes field in page flags"
#endif
#define NODES_WIDTH		0
#endif

#ifdef CONFIG_NUMA_BALANCING
#define LAST__PID_SHIFT 8
#define LAST__PID_MASK  ((1 << LAST__PID_SHIFT)-1)

#define LAST__CPU_SHIFT NR_CPUS_BITS
#define LAST__CPU_MASK  ((1 << LAST__CPU_SHIFT)-1)

#define LAST_CPUPID_SHIFT (LAST__PID_SHIFT+LAST__CPU_SHIFT)
#else
#define LAST_CPUPID_SHIFT 0
#endif

#if SECTIONS_WIDTH+ZONES_WIDTH+NODES_SHIFT+LAST_CPUPID_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
#define LAST_CPUPID_WIDTH LAST_CPUPID_SHIFT
#else
#define LAST_CPUPID_WIDTH 0
#endif


#if !(NODES_WIDTH > 0 || NODES_SHIFT == 0)
#define NODE_NOT_IN_PAGE_FLAGS
#endif

#if defined(CONFIG_NUMA_BALANCING) && LAST_CPUPID_WIDTH == 0
#define LAST_CPUPID_NOT_IN_PAGE_FLAGS
#endif

#endif 
