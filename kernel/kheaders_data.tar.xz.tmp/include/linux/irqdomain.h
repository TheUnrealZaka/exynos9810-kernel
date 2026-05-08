

#ifndef _LINUX_IRQDOMAIN_H
#define _LINUX_IRQDOMAIN_H

#include <linux/types.h>
#include <linux/irqhandler.h>
#include <linux/of.h>
#include <linux/radix-tree.h>

struct device_node;
struct irq_domain;
struct of_device_id;
struct irq_chip;
struct irq_data;
struct cpumask;


#define NUM_ISA_INTERRUPTS	16

#define IRQ_DOMAIN_IRQ_SPEC_PARAMS 16


struct irq_fwspec {
	struct fwnode_handle *fwnode;
	int param_count;
	u32 param[IRQ_DOMAIN_IRQ_SPEC_PARAMS];
};


enum irq_domain_bus_token {
	DOMAIN_BUS_ANY		= 0,
	DOMAIN_BUS_WIRED,
	DOMAIN_BUS_PCI_MSI,
	DOMAIN_BUS_PLATFORM_MSI,
	DOMAIN_BUS_NEXUS,
	DOMAIN_BUS_IPI,
	DOMAIN_BUS_FSL_MC_MSI,
};


struct irq_domain_ops {
	int (*match)(struct irq_domain *d, struct device_node *node,
		     enum irq_domain_bus_token bus_token);
	int (*select)(struct irq_domain *d, struct irq_fwspec *fwspec,
		      enum irq_domain_bus_token bus_token);
	int (*map)(struct irq_domain *d, unsigned int virq, irq_hw_number_t hw);
	void (*unmap)(struct irq_domain *d, unsigned int virq);
	int (*xlate)(struct irq_domain *d, struct device_node *node,
		     const u32 *intspec, unsigned int intsize,
		     unsigned long *out_hwirq, unsigned int *out_type);

#ifdef	CONFIG_IRQ_DOMAIN_HIERARCHY
	
	int (*alloc)(struct irq_domain *d, unsigned int virq,
		     unsigned int nr_irqs, void *arg);
	void (*free)(struct irq_domain *d, unsigned int virq,
		     unsigned int nr_irqs);
	void (*activate)(struct irq_domain *d, struct irq_data *irq_data);
	void (*deactivate)(struct irq_domain *d, struct irq_data *irq_data);
	int (*translate)(struct irq_domain *d, struct irq_fwspec *fwspec,
			 unsigned long *out_hwirq, unsigned int *out_type);
#endif
};

extern struct irq_domain_ops irq_generic_chip_ops;

struct irq_domain_chip_generic;


struct irq_domain {
	struct list_head link;
	const char *name;
	const struct irq_domain_ops *ops;
	void *host_data;
	unsigned int flags;

	
	struct fwnode_handle *fwnode;
	enum irq_domain_bus_token bus_token;
	struct irq_domain_chip_generic *gc;
#ifdef	CONFIG_IRQ_DOMAIN_HIERARCHY
	struct irq_domain *parent;
#endif

	
	irq_hw_number_t hwirq_max;
	unsigned int revmap_direct_max_irq;
	unsigned int revmap_size;
	struct radix_tree_root revmap_tree;
	unsigned int linear_revmap[];
};


enum {
	
	IRQ_DOMAIN_FLAG_HIERARCHY	= (1 << 0),

	
	IRQ_DOMAIN_FLAG_AUTO_RECURSIVE	= (1 << 1),

	
	IRQ_DOMAIN_FLAG_IPI_PER_CPU	= (1 << 2),

	
	IRQ_DOMAIN_FLAG_IPI_SINGLE	= (1 << 3),

	
	IRQ_DOMAIN_FLAG_NONCORE		= (1 << 16),
};

static inline struct device_node *irq_domain_get_of_node(struct irq_domain *d)
{
	return to_of_node(d->fwnode);
}

#ifdef CONFIG_IRQ_DOMAIN
struct fwnode_handle *irq_domain_alloc_fwnode(void *data);
void irq_domain_free_fwnode(struct fwnode_handle *fwnode);
struct irq_domain *__irq_domain_add(struct fwnode_handle *fwnode, int size,
				    irq_hw_number_t hwirq_max, int direct_max,
				    const struct irq_domain_ops *ops,
				    void *host_data);
struct irq_domain *irq_domain_add_simple(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
					 const struct irq_domain_ops *ops,
					 void *host_data);
struct irq_domain *irq_domain_add_legacy(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
					 irq_hw_number_t first_hwirq,
					 const struct irq_domain_ops *ops,
					 void *host_data);
extern struct irq_domain *irq_find_matching_fwspec(struct irq_fwspec *fwspec,
						   enum irq_domain_bus_token bus_token);
extern void irq_set_default_host(struct irq_domain *host);
extern int irq_domain_alloc_descs(int virq, unsigned int nr_irqs,
				  irq_hw_number_t hwirq, int node,
				  const struct cpumask *affinity);

static inline struct fwnode_handle *of_node_to_fwnode(struct device_node *node)
{
	return node ? &node->fwnode : NULL;
}

static inline bool is_fwnode_irqchip(struct fwnode_handle *fwnode)
{
	return fwnode && fwnode->type == FWNODE_IRQCHIP;
}

static inline
struct irq_domain *irq_find_matching_fwnode(struct fwnode_handle *fwnode,
					    enum irq_domain_bus_token bus_token)
{
	struct irq_fwspec fwspec = {
		.fwnode = fwnode,
	};

	return irq_find_matching_fwspec(&fwspec, bus_token);
}

static inline struct irq_domain *irq_find_matching_host(struct device_node *node,
							enum irq_domain_bus_token bus_token)
{
	return irq_find_matching_fwnode(of_node_to_fwnode(node), bus_token);
}

static inline struct irq_domain *irq_find_host(struct device_node *node)
{
	return irq_find_matching_host(node, DOMAIN_BUS_ANY);
}


static inline struct irq_domain *irq_domain_add_linear(struct device_node *of_node,
					 unsigned int size,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	return __irq_domain_add(of_node_to_fwnode(of_node), size, size, 0, ops, host_data);
}
static inline struct irq_domain *irq_domain_add_nomap(struct device_node *of_node,
					 unsigned int max_irq,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	return __irq_domain_add(of_node_to_fwnode(of_node), 0, max_irq, max_irq, ops, host_data);
}
static inline struct irq_domain *irq_domain_add_legacy_isa(
				struct device_node *of_node,
				const struct irq_domain_ops *ops,
				void *host_data)
{
	return irq_domain_add_legacy(of_node, NUM_ISA_INTERRUPTS, 0, 0, ops,
				     host_data);
}
static inline struct irq_domain *irq_domain_add_tree(struct device_node *of_node,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	return __irq_domain_add(of_node_to_fwnode(of_node), 0, ~0, 0, ops, host_data);
}

static inline struct irq_domain *irq_domain_create_linear(struct fwnode_handle *fwnode,
					 unsigned int size,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	return __irq_domain_add(fwnode, size, size, 0, ops, host_data);
}

static inline struct irq_domain *irq_domain_create_tree(struct fwnode_handle *fwnode,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	return __irq_domain_add(fwnode, 0, ~0, 0, ops, host_data);
}

extern void irq_domain_remove(struct irq_domain *host);

extern int irq_domain_associate(struct irq_domain *domain, unsigned int irq,
					irq_hw_number_t hwirq);
extern void irq_domain_associate_many(struct irq_domain *domain,
				      unsigned int irq_base,
				      irq_hw_number_t hwirq_base, int count);
extern void irq_domain_disassociate(struct irq_domain *domain,
				    unsigned int irq);

extern unsigned int irq_create_mapping(struct irq_domain *host,
				       irq_hw_number_t hwirq);
extern unsigned int irq_create_fwspec_mapping(struct irq_fwspec *fwspec);
extern void irq_dispose_mapping(unsigned int virq);


static inline unsigned int irq_linear_revmap(struct irq_domain *domain,
					     irq_hw_number_t hwirq)
{
	return hwirq < domain->revmap_size ? domain->linear_revmap[hwirq] : 0;
}
extern unsigned int irq_find_mapping(struct irq_domain *host,
				     irq_hw_number_t hwirq);
extern unsigned int irq_create_direct_mapping(struct irq_domain *host);
extern int irq_create_strict_mappings(struct irq_domain *domain,
				      unsigned int irq_base,
				      irq_hw_number_t hwirq_base, int count);

static inline int irq_create_identity_mapping(struct irq_domain *host,
					      irq_hw_number_t hwirq)
{
	return irq_create_strict_mappings(host, hwirq, hwirq, 1);
}

extern const struct irq_domain_ops irq_domain_simple_ops;


int irq_domain_xlate_onecell(struct irq_domain *d, struct device_node *ctrlr,
			const u32 *intspec, unsigned int intsize,
			irq_hw_number_t *out_hwirq, unsigned int *out_type);
int irq_domain_xlate_twocell(struct irq_domain *d, struct device_node *ctrlr,
			const u32 *intspec, unsigned int intsize,
			irq_hw_number_t *out_hwirq, unsigned int *out_type);
int irq_domain_xlate_onetwocell(struct irq_domain *d, struct device_node *ctrlr,
			const u32 *intspec, unsigned int intsize,
			irq_hw_number_t *out_hwirq, unsigned int *out_type);


int irq_reserve_ipi(struct irq_domain *domain, const struct cpumask *dest);
int irq_destroy_ipi(unsigned int irq, const struct cpumask *dest);


extern struct irq_data *irq_domain_get_irq_data(struct irq_domain *domain,
						unsigned int virq);
extern void irq_domain_set_info(struct irq_domain *domain, unsigned int virq,
				irq_hw_number_t hwirq, struct irq_chip *chip,
				void *chip_data, irq_flow_handler_t handler,
				void *handler_data, const char *handler_name);
#ifdef	CONFIG_IRQ_DOMAIN_HIERARCHY
extern struct irq_domain *irq_domain_create_hierarchy(struct irq_domain *parent,
			unsigned int flags, unsigned int size,
			struct fwnode_handle *fwnode,
			const struct irq_domain_ops *ops, void *host_data);

static inline struct irq_domain *irq_domain_add_hierarchy(struct irq_domain *parent,
					    unsigned int flags,
					    unsigned int size,
					    struct device_node *node,
					    const struct irq_domain_ops *ops,
					    void *host_data)
{
	return irq_domain_create_hierarchy(parent, flags, size,
					   of_node_to_fwnode(node),
					   ops, host_data);
}

extern int __irq_domain_alloc_irqs(struct irq_domain *domain, int irq_base,
				   unsigned int nr_irqs, int node, void *arg,
				   bool realloc, const struct cpumask *affinity);
extern void irq_domain_free_irqs(unsigned int virq, unsigned int nr_irqs);
extern void irq_domain_activate_irq(struct irq_data *irq_data);
extern void irq_domain_deactivate_irq(struct irq_data *irq_data);

static inline int irq_domain_alloc_irqs(struct irq_domain *domain,
			unsigned int nr_irqs, int node, void *arg)
{
	return __irq_domain_alloc_irqs(domain, -1, nr_irqs, node, arg, false,
				       NULL);
}

extern int irq_domain_alloc_irqs_recursive(struct irq_domain *domain,
					   unsigned int irq_base,
					   unsigned int nr_irqs, void *arg);
extern int irq_domain_set_hwirq_and_chip(struct irq_domain *domain,
					 unsigned int virq,
					 irq_hw_number_t hwirq,
					 struct irq_chip *chip,
					 void *chip_data);
extern void irq_domain_reset_irq_data(struct irq_data *irq_data);
extern void irq_domain_free_irqs_common(struct irq_domain *domain,
					unsigned int virq,
					unsigned int nr_irqs);
extern void irq_domain_free_irqs_top(struct irq_domain *domain,
				     unsigned int virq, unsigned int nr_irqs);

extern int irq_domain_alloc_irqs_parent(struct irq_domain *domain,
					unsigned int irq_base,
					unsigned int nr_irqs, void *arg);

extern void irq_domain_free_irqs_parent(struct irq_domain *domain,
					unsigned int irq_base,
					unsigned int nr_irqs);

static inline bool irq_domain_is_hierarchy(struct irq_domain *domain)
{
	return domain->flags & IRQ_DOMAIN_FLAG_HIERARCHY;
}

static inline bool irq_domain_is_ipi(struct irq_domain *domain)
{
	return domain->flags &
		(IRQ_DOMAIN_FLAG_IPI_PER_CPU | IRQ_DOMAIN_FLAG_IPI_SINGLE);
}

static inline bool irq_domain_is_ipi_per_cpu(struct irq_domain *domain)
{
	return domain->flags & IRQ_DOMAIN_FLAG_IPI_PER_CPU;
}

static inline bool irq_domain_is_ipi_single(struct irq_domain *domain)
{
	return domain->flags & IRQ_DOMAIN_FLAG_IPI_SINGLE;
}
#else	
static inline void irq_domain_activate_irq(struct irq_data *data) { }
static inline void irq_domain_deactivate_irq(struct irq_data *data) { }
static inline int irq_domain_alloc_irqs(struct irq_domain *domain,
			unsigned int nr_irqs, int node, void *arg)
{
	return -1;
}

static inline void irq_domain_free_irqs(unsigned int virq,
					unsigned int nr_irqs) { }

static inline bool irq_domain_is_hierarchy(struct irq_domain *domain)
{
	return false;
}

static inline bool irq_domain_is_ipi(struct irq_domain *domain)
{
	return false;
}

static inline bool irq_domain_is_ipi_per_cpu(struct irq_domain *domain)
{
	return false;
}

static inline bool irq_domain_is_ipi_single(struct irq_domain *domain)
{
	return false;
}
#endif	

#else 
static inline void irq_dispose_mapping(unsigned int virq) { }
static inline void irq_domain_activate_irq(struct irq_data *data) { }
static inline void irq_domain_deactivate_irq(struct irq_data *data) { }
static inline struct irq_domain *irq_find_matching_fwnode(
	struct fwnode_handle *fwnode, enum irq_domain_bus_token bus_token)
{
	return NULL;
}
#endif 

#endif 
