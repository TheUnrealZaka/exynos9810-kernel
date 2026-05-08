
#ifndef _LINUX_MEMORY_H_
#define _LINUX_MEMORY_H_

#include <linux/node.h>
#include <linux/compiler.h>
#include <linux/mutex.h>
#include <linux/notifier.h>

#define MIN_MEMORY_BLOCK_SIZE     (1UL << SECTION_SIZE_BITS)

struct memory_block {
	unsigned long start_section_nr;
	unsigned long end_section_nr;
	unsigned long state;		
	int section_count;		
	int online_type;		
	int phys_device;		
	void *hw;			
	int (*phys_callback)(struct memory_block *);
	struct device dev;
};

int arch_get_memory_phys_device(unsigned long start_pfn);
unsigned long memory_block_size_bytes(void);


#define	MEM_ONLINE		(1<<0) 
#define	MEM_GOING_OFFLINE	(1<<1) 
#define	MEM_OFFLINE		(1<<2) 
#define	MEM_GOING_ONLINE	(1<<3)
#define	MEM_CANCEL_ONLINE	(1<<4)
#define	MEM_CANCEL_OFFLINE	(1<<5)

struct memory_notify {
	unsigned long start_pfn;
	unsigned long nr_pages;
	int status_change_nid_normal;
	int status_change_nid_high;
	int status_change_nid;
};


#define MEM_ISOLATE_COUNT	(1<<0)

struct memory_isolate_notify {
	unsigned long start_pfn;	
	unsigned int nr_pages;		
	unsigned int pages_found;	
};

struct notifier_block;
struct mem_section;


#define SLAB_CALLBACK_PRI       1
#define IPC_CALLBACK_PRI        10

#ifndef CONFIG_MEMORY_HOTPLUG_SPARSE
static inline int memory_dev_init(void)
{
	return 0;
}
static inline int register_memory_notifier(struct notifier_block *nb)
{
	return 0;
}
static inline void unregister_memory_notifier(struct notifier_block *nb)
{
}
static inline int memory_notify(unsigned long val, void *v)
{
	return 0;
}
static inline int register_memory_isolate_notifier(struct notifier_block *nb)
{
	return 0;
}
static inline void unregister_memory_isolate_notifier(struct notifier_block *nb)
{
}
static inline int memory_isolate_notify(unsigned long val, void *v)
{
	return 0;
}
#else
extern int register_memory_notifier(struct notifier_block *nb);
extern void unregister_memory_notifier(struct notifier_block *nb);
extern int register_memory_isolate_notifier(struct notifier_block *nb);
extern void unregister_memory_isolate_notifier(struct notifier_block *nb);
extern int register_new_memory(int, struct mem_section *);
extern int memory_block_change_state(struct memory_block *mem,
				     unsigned long to_state,
				     unsigned long from_state_req);
#ifdef CONFIG_MEMORY_HOTREMOVE
extern int unregister_memory_section(struct mem_section *);
#endif
extern int memory_dev_init(void);
extern int memory_notify(unsigned long val, void *v);
extern int memory_isolate_notify(unsigned long val, void *v);
extern struct memory_block *find_memory_block_hinted(struct mem_section *,
							struct memory_block *);
extern struct memory_block *find_memory_block(struct mem_section *);
#define CONFIG_MEM_BLOCK_SIZE	(PAGES_PER_SECTION<<PAGE_SHIFT)
#endif 

#ifdef CONFIG_MEMORY_HOTPLUG
#define hotplug_memory_notifier(fn, pri) ({		\
	static __meminitdata struct notifier_block fn##_mem_nb =\
		{ .notifier_call = fn, .priority = pri };\
	register_memory_notifier(&fn##_mem_nb);			\
})
#define register_hotmemory_notifier(nb)		register_memory_notifier(nb)
#define unregister_hotmemory_notifier(nb) 	unregister_memory_notifier(nb)
#else
#define hotplug_memory_notifier(fn, pri)	({ 0; })

#define register_hotmemory_notifier(nb)    ({ (void)(nb); 0; })
#define unregister_hotmemory_notifier(nb)  ({ (void)(nb); })
#endif


extern struct mutex text_mutex;

#endif 
