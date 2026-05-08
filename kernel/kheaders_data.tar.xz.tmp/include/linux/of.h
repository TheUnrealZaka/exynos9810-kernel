#ifndef _LINUX_OF_H
#define _LINUX_OF_H

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/mod_devicetable.h>
#include <linux/spinlock.h>
#include <linux/topology.h>
#include <linux/notifier.h>
#include <linux/property.h>
#include <linux/list.h>

#include <asm/byteorder.h>
#include <asm/errno.h>

typedef u32 phandle;
typedef u32 ihandle;

struct property {
	char	*name;
	int	length;
	void	*value;
	struct property *next;
	unsigned long _flags;
	unsigned int unique_id;
	struct bin_attribute attr;
};

#if defined(CONFIG_SPARC)
struct of_irq_controller;
#endif

struct device_node {
	const char *name;
	const char *type;
	phandle phandle;
	const char *full_name;
	struct fwnode_handle fwnode;

	struct	property *properties;
	struct	property *deadprops;	
	struct	device_node *parent;
	struct	device_node *child;
	struct	device_node *sibling;
	struct	kobject kobj;
	unsigned long _flags;
	void	*data;
#if defined(CONFIG_SPARC)
	const char *path_component_name;
	unsigned int unique_id;
	struct of_irq_controller *irq_trans;
#endif
};

#define MAX_PHANDLE_ARGS 16
struct of_phandle_args {
	struct device_node *np;
	int args_count;
	uint32_t args[MAX_PHANDLE_ARGS];
};

struct of_phandle_iterator {
	
	const char *cells_name;
	int cell_count;
	const struct device_node *parent;

	
	const __be32 *list_end;
	const __be32 *phandle_end;

	
	const __be32 *cur;
	uint32_t cur_count;
	phandle phandle;
	struct device_node *node;
};

struct of_reconfig_data {
	struct device_node	*dn;
	struct property		*prop;
	struct property		*old_prop;
};


extern struct kobj_type of_node_ktype;
static inline void of_node_init(struct device_node *node)
{
	kobject_init(&node->kobj, &of_node_ktype);
	node->fwnode.type = FWNODE_OF;
}


static inline int of_node_is_initialized(struct device_node *node)
{
	return node && node->kobj.state_initialized;
}


static inline int of_node_is_attached(struct device_node *node)
{
	return node && node->kobj.state_in_sysfs;
}

#ifdef CONFIG_OF_DYNAMIC
extern struct device_node *of_node_get(struct device_node *node);
extern void of_node_put(struct device_node *node);
#else 

static inline struct device_node *of_node_get(struct device_node *node)
{
	return node;
}
static inline void of_node_put(struct device_node *node) { }
#endif 


extern struct device_node *of_root;
extern struct device_node *of_chosen;
extern struct device_node *of_aliases;
extern struct device_node *of_stdout;
extern raw_spinlock_t devtree_lock;


#define OF_DYNAMIC	1 
#define OF_DETACHED	2 
#define OF_POPULATED	3 
#define OF_POPULATED_BUS	4 

#define OF_BAD_ADDR	((u64)-1)

#ifdef CONFIG_OF
void of_core_init(void);

static inline bool is_of_node(const struct fwnode_handle *fwnode)
{
	return !IS_ERR_OR_NULL(fwnode) && fwnode->type == FWNODE_OF;
}

#define to_of_node(__fwnode)						\
	({								\
		typeof(__fwnode) __to_of_node_fwnode = (__fwnode);	\
									\
		is_of_node(__to_of_node_fwnode) ?			\
			container_of(__to_of_node_fwnode,		\
				     struct device_node, fwnode) :	\
			NULL;						\
	})

static inline bool of_have_populated_dt(void)
{
	return of_root != NULL;
}

static inline bool of_node_is_root(const struct device_node *node)
{
	return node && (node->parent == NULL);
}

static inline int of_node_check_flag(struct device_node *n, unsigned long flag)
{
	return test_bit(flag, &n->_flags);
}

static inline int of_node_test_and_set_flag(struct device_node *n,
					    unsigned long flag)
{
	return test_and_set_bit(flag, &n->_flags);
}

static inline void of_node_set_flag(struct device_node *n, unsigned long flag)
{
	set_bit(flag, &n->_flags);
}

static inline void of_node_clear_flag(struct device_node *n, unsigned long flag)
{
	clear_bit(flag, &n->_flags);
}

static inline int of_property_check_flag(struct property *p, unsigned long flag)
{
	return test_bit(flag, &p->_flags);
}

static inline void of_property_set_flag(struct property *p, unsigned long flag)
{
	set_bit(flag, &p->_flags);
}

static inline void of_property_clear_flag(struct property *p, unsigned long flag)
{
	clear_bit(flag, &p->_flags);
}

extern struct device_node *__of_find_all_nodes(struct device_node *prev);
extern struct device_node *of_find_all_nodes(struct device_node *prev);




static inline u64 of_read_number(const __be32 *cell, int size)
{
	u64 r = 0;
	for (; size--; cell++)
		r = (r << 32) | be32_to_cpu(*cell);
	return r;
}


static inline unsigned long of_read_ulong(const __be32 *cell, int size)
{
	
	return of_read_number(cell, size);
}

#if defined(CONFIG_SPARC)
#include <asm/prom.h>
#endif


#if !defined(OF_ROOT_NODE_ADDR_CELLS_DEFAULT)
#define OF_ROOT_NODE_ADDR_CELLS_DEFAULT 1
#define OF_ROOT_NODE_SIZE_CELLS_DEFAULT 1
#endif

#define OF_IS_DYNAMIC(x) test_bit(OF_DYNAMIC, &x->_flags)
#define OF_MARK_DYNAMIC(x) set_bit(OF_DYNAMIC, &x->_flags)

static inline const char *of_node_full_name(const struct device_node *np)
{
	return np ? np->full_name : "<no-node>";
}

#define for_each_of_allnodes_from(from, dn) \
	for (dn = __of_find_all_nodes(from); dn; dn = __of_find_all_nodes(dn))
#define for_each_of_allnodes(dn) for_each_of_allnodes_from(NULL, dn)
extern struct device_node *of_find_node_by_name(struct device_node *from,
	const char *name);
extern struct device_node *of_find_node_by_type(struct device_node *from,
	const char *type);
extern struct device_node *of_find_compatible_node(struct device_node *from,
	const char *type, const char *compat);
extern struct device_node *of_find_matching_node_and_match(
	struct device_node *from,
	const struct of_device_id *matches,
	const struct of_device_id **match);

extern struct device_node *of_find_node_opts_by_path(const char *path,
	const char **opts);
static inline struct device_node *of_find_node_by_path(const char *path)
{
	return of_find_node_opts_by_path(path, NULL);
}

extern struct device_node *of_find_node_by_phandle(phandle handle);
extern struct device_node *of_get_parent(const struct device_node *node);
extern struct device_node *of_get_next_parent(struct device_node *node);
extern struct device_node *of_get_next_child(const struct device_node *node,
					     struct device_node *prev);
extern struct device_node *of_get_next_available_child(
	const struct device_node *node, struct device_node *prev);

extern struct device_node *of_get_compatible_child(const struct device_node *parent,
					const char *compatible);
extern struct device_node *of_get_child_by_name(const struct device_node *node,
					const char *name);


extern struct device_node *of_find_next_cache_node(const struct device_node *);
extern struct device_node *of_find_node_with_property(
	struct device_node *from, const char *prop_name);

extern struct property *of_find_property(const struct device_node *np,
					 const char *name,
					 int *lenp);
extern int of_property_count_elems_of_size(const struct device_node *np,
				const char *propname, int elem_size);
extern int of_property_read_u32_index(const struct device_node *np,
				       const char *propname,
				       u32 index, u32 *out_value);
extern int of_property_read_variable_u8_array(const struct device_node *np,
					const char *propname, u8 *out_values,
					size_t sz_min, size_t sz_max);
extern int of_property_read_variable_u16_array(const struct device_node *np,
					const char *propname, u16 *out_values,
					size_t sz_min, size_t sz_max);
extern int of_property_read_variable_u32_array(const struct device_node *np,
					const char *propname,
					u32 *out_values,
					size_t sz_min,
					size_t sz_max);
extern int of_property_read_u64(const struct device_node *np,
				const char *propname, u64 *out_value);
extern int of_property_read_variable_u64_array(const struct device_node *np,
					const char *propname,
					u64 *out_values,
					size_t sz_min,
					size_t sz_max);

extern int of_property_read_string(const struct device_node *np,
				   const char *propname,
				   const char **out_string);
extern int of_property_match_string(const struct device_node *np,
				    const char *propname,
				    const char *string);
extern int of_property_read_string_helper(const struct device_node *np,
					      const char *propname,
					      const char **out_strs, size_t sz, int index);
extern int of_device_is_compatible(const struct device_node *device,
				   const char *);
extern int of_device_compatible_match(struct device_node *device,
				      const char *const *compat);
extern bool of_device_is_available(const struct device_node *device);
extern bool of_device_is_big_endian(const struct device_node *device);
extern const void *of_get_property(const struct device_node *node,
				const char *name,
				int *lenp);
extern struct device_node *of_get_cpu_node(int cpu, unsigned int *thread);
#define for_each_property_of_node(dn, pp) \
	for (pp = dn->properties; pp != NULL; pp = pp->next)

extern int of_n_addr_cells(struct device_node *np);
extern int of_n_size_cells(struct device_node *np);
extern const struct of_device_id *of_match_node(
	const struct of_device_id *matches, const struct device_node *node);
extern int of_modalias_node(struct device_node *node, char *modalias, int len);
extern void of_print_phandle_args(const char *msg, const struct of_phandle_args *args);
extern struct device_node *of_parse_phandle(const struct device_node *np,
					    const char *phandle_name,
					    int index);
extern int of_parse_phandle_with_args(const struct device_node *np,
	const char *list_name, const char *cells_name, int index,
	struct of_phandle_args *out_args);
extern int of_parse_phandle_with_fixed_args(const struct device_node *np,
	const char *list_name, int cells_count, int index,
	struct of_phandle_args *out_args);
extern int of_count_phandle_with_args(const struct device_node *np,
	const char *list_name, const char *cells_name);


extern int of_phandle_iterator_init(struct of_phandle_iterator *it,
				    const struct device_node *np,
				    const char *list_name,
				    const char *cells_name,
				    int cell_count);

extern int of_phandle_iterator_next(struct of_phandle_iterator *it);
extern int of_phandle_iterator_args(struct of_phandle_iterator *it,
				    uint32_t *args,
				    int size);

extern void of_alias_scan(void * (*dt_alloc)(u64 size, u64 align));
extern int of_alias_get_id(struct device_node *np, const char *stem);
extern int of_alias_get_highest_id(const char *stem);

extern int of_machine_is_compatible(const char *compat);

extern int of_add_property(struct device_node *np, struct property *prop);
extern int of_remove_property(struct device_node *np, struct property *prop);
extern int of_update_property(struct device_node *np, struct property *newprop);


#define OF_RECONFIG_ATTACH_NODE		0x0001
#define OF_RECONFIG_DETACH_NODE		0x0002
#define OF_RECONFIG_ADD_PROPERTY	0x0003
#define OF_RECONFIG_REMOVE_PROPERTY	0x0004
#define OF_RECONFIG_UPDATE_PROPERTY	0x0005

extern int of_attach_node(struct device_node *);
extern int of_detach_node(struct device_node *);

#define of_match_ptr(_ptr)	(_ptr)


static inline int of_property_read_u8_array(const struct device_node *np,
					    const char *propname,
					    u8 *out_values, size_t sz)
{
	int ret = of_property_read_variable_u8_array(np, propname, out_values,
						     sz, 0);
	if (ret >= 0)
		return 0;
	else
		return ret;
}


static inline int of_property_read_u16_array(const struct device_node *np,
					     const char *propname,
					     u16 *out_values, size_t sz)
{
	int ret = of_property_read_variable_u16_array(np, propname, out_values,
						      sz, 0);
	if (ret >= 0)
		return 0;
	else
		return ret;
}


static inline int of_property_read_u32_array(const struct device_node *np,
					     const char *propname,
					     u32 *out_values, size_t sz)
{
	int ret = of_property_read_variable_u32_array(np, propname, out_values,
						      sz, 0);
	if (ret >= 0)
		return 0;
	else
		return ret;
}


static inline int of_property_read_u64_array(const struct device_node *np,
					     const char *propname,
					     u64 *out_values, size_t sz)
{
	int ret = of_property_read_variable_u64_array(np, propname, out_values,
						      sz, 0);
	if (ret >= 0)
		return 0;
	else
		return ret;
}


const __be32 *of_prop_next_u32(struct property *prop, const __be32 *cur,
			       u32 *pu);

const char *of_prop_next_string(struct property *prop, const char *cur);

bool of_console_check(struct device_node *dn, char *name, int index);

#else 

static inline void of_core_init(void)
{
}

static inline bool is_of_node(const struct fwnode_handle *fwnode)
{
	return false;
}

static inline struct device_node *to_of_node(const struct fwnode_handle *fwnode)
{
	return NULL;
}

static inline const char* of_node_full_name(const struct device_node *np)
{
	return "<no-node>";
}

static inline struct device_node *of_find_node_by_name(struct device_node *from,
	const char *name)
{
	return NULL;
}

static inline struct device_node *of_find_node_by_type(struct device_node *from,
	const char *type)
{
	return NULL;
}

static inline struct device_node *of_find_matching_node_and_match(
	struct device_node *from,
	const struct of_device_id *matches,
	const struct of_device_id **match)
{
	return NULL;
}

static inline struct device_node *of_find_node_by_path(const char *path)
{
	return NULL;
}

static inline struct device_node *of_find_node_opts_by_path(const char *path,
	const char **opts)
{
	return NULL;
}

static inline struct device_node *of_find_node_by_phandle(phandle handle)
{
	return NULL;
}

static inline struct device_node *of_get_parent(const struct device_node *node)
{
	return NULL;
}

static inline struct device_node *of_get_next_child(
	const struct device_node *node, struct device_node *prev)
{
	return NULL;
}

static inline struct device_node *of_get_next_available_child(
	const struct device_node *node, struct device_node *prev)
{
	return NULL;
}

static inline struct device_node *of_find_node_with_property(
	struct device_node *from, const char *prop_name)
{
	return NULL;
}

static inline bool of_have_populated_dt(void)
{
	return false;
}

static inline struct device_node *of_get_compatible_child(const struct device_node *parent,
					const char *compatible)
{
	return NULL;
}

static inline struct device_node *of_get_child_by_name(
					const struct device_node *node,
					const char *name)
{
	return NULL;
}

static inline int of_device_is_compatible(const struct device_node *device,
					  const char *name)
{
	return 0;
}

static inline bool of_device_is_available(const struct device_node *device)
{
	return false;
}

static inline bool of_device_is_big_endian(const struct device_node *device)
{
	return false;
}

static inline struct property *of_find_property(const struct device_node *np,
						const char *name,
						int *lenp)
{
	return NULL;
}

static inline struct device_node *of_find_compatible_node(
						struct device_node *from,
						const char *type,
						const char *compat)
{
	return NULL;
}

static inline int of_property_count_elems_of_size(const struct device_node *np,
			const char *propname, int elem_size)
{
	return -ENOSYS;
}

static inline int of_property_read_u32_index(const struct device_node *np,
			const char *propname, u32 index, u32 *out_value)
{
	return -ENOSYS;
}

static inline int of_property_read_u8_array(const struct device_node *np,
			const char *propname, u8 *out_values, size_t sz)
{
	return -ENOSYS;
}

static inline int of_property_read_u16_array(const struct device_node *np,
			const char *propname, u16 *out_values, size_t sz)
{
	return -ENOSYS;
}

static inline int of_property_read_u32_array(const struct device_node *np,
					     const char *propname,
					     u32 *out_values, size_t sz)
{
	return -ENOSYS;
}

static inline int of_property_read_u64_array(const struct device_node *np,
					     const char *propname,
					     u64 *out_values, size_t sz)
{
	return -ENOSYS;
}

static inline int of_property_read_string(const struct device_node *np,
					  const char *propname,
					  const char **out_string)
{
	return -ENOSYS;
}

static inline int of_property_read_string_helper(const struct device_node *np,
						 const char *propname,
						 const char **out_strs, size_t sz, int index)
{
	return -ENOSYS;
}

static inline const void *of_get_property(const struct device_node *node,
				const char *name,
				int *lenp)
{
	return NULL;
}

static inline struct device_node *of_get_cpu_node(int cpu,
					unsigned int *thread)
{
	return NULL;
}

static inline int of_property_read_u64(const struct device_node *np,
				       const char *propname, u64 *out_value)
{
	return -ENOSYS;
}

static inline int of_property_match_string(const struct device_node *np,
					   const char *propname,
					   const char *string)
{
	return -ENOSYS;
}

static inline struct device_node *of_parse_phandle(const struct device_node *np,
						   const char *phandle_name,
						   int index)
{
	return NULL;
}

static inline int of_parse_phandle_with_args(const struct device_node *np,
					     const char *list_name,
					     const char *cells_name,
					     int index,
					     struct of_phandle_args *out_args)
{
	return -ENOSYS;
}

static inline int of_parse_phandle_with_fixed_args(const struct device_node *np,
	const char *list_name, int cells_count, int index,
	struct of_phandle_args *out_args)
{
	return -ENOSYS;
}

static inline int of_count_phandle_with_args(struct device_node *np,
					     const char *list_name,
					     const char *cells_name)
{
	return -ENOSYS;
}

static inline int of_phandle_iterator_init(struct of_phandle_iterator *it,
					   const struct device_node *np,
					   const char *list_name,
					   const char *cells_name,
					   int cell_count)
{
	return -ENOSYS;
}

static inline int of_phandle_iterator_next(struct of_phandle_iterator *it)
{
	return -ENOSYS;
}

static inline int of_phandle_iterator_args(struct of_phandle_iterator *it,
					   uint32_t *args,
					   int size)
{
	return 0;
}

static inline int of_alias_get_id(struct device_node *np, const char *stem)
{
	return -ENOSYS;
}

static inline int of_alias_get_highest_id(const char *stem)
{
	return -ENOSYS;
}

static inline int of_machine_is_compatible(const char *compat)
{
	return 0;
}

static inline bool of_console_check(const struct device_node *dn, const char *name, int index)
{
	return false;
}

static inline const __be32 *of_prop_next_u32(struct property *prop,
		const __be32 *cur, u32 *pu)
{
	return NULL;
}

static inline const char *of_prop_next_string(struct property *prop,
		const char *cur)
{
	return NULL;
}

static inline int of_node_check_flag(struct device_node *n, unsigned long flag)
{
	return 0;
}

static inline int of_node_test_and_set_flag(struct device_node *n,
					    unsigned long flag)
{
	return 0;
}

static inline void of_node_set_flag(struct device_node *n, unsigned long flag)
{
}

static inline void of_node_clear_flag(struct device_node *n, unsigned long flag)
{
}

static inline int of_property_check_flag(struct property *p, unsigned long flag)
{
	return 0;
}

static inline void of_property_set_flag(struct property *p, unsigned long flag)
{
}

static inline void of_property_clear_flag(struct property *p, unsigned long flag)
{
}

#define of_match_ptr(_ptr)	NULL
#define of_match_node(_matches, _node)	NULL
#endif 


#if !defined(of_compat_cmp)
#define of_compat_cmp(s1, s2, l)	strcasecmp((s1), (s2))
#define of_prop_cmp(s1, s2)		strcmp((s1), (s2))
#define of_node_cmp(s1, s2)		strcasecmp((s1), (s2))
#endif

#if defined(CONFIG_OF) && defined(CONFIG_NUMA)
extern int of_node_to_nid(struct device_node *np);
#else
static inline int of_node_to_nid(struct device_node *device)
{
	return NUMA_NO_NODE;
}
#endif

#ifdef CONFIG_OF_NUMA
extern int of_numa_init(void);
#else
static inline int of_numa_init(void)
{
	return -ENOSYS;
}
#endif

static inline struct device_node *of_find_matching_node(
	struct device_node *from,
	const struct of_device_id *matches)
{
	return of_find_matching_node_and_match(from, matches, NULL);
}


static inline int of_property_count_u8_elems(const struct device_node *np,
				const char *propname)
{
	return of_property_count_elems_of_size(np, propname, sizeof(u8));
}


static inline int of_property_count_u16_elems(const struct device_node *np,
				const char *propname)
{
	return of_property_count_elems_of_size(np, propname, sizeof(u16));
}


static inline int of_property_count_u32_elems(const struct device_node *np,
				const char *propname)
{
	return of_property_count_elems_of_size(np, propname, sizeof(u32));
}


static inline int of_property_count_u64_elems(const struct device_node *np,
				const char *propname)
{
	return of_property_count_elems_of_size(np, propname, sizeof(u64));
}


static inline int of_property_read_string_array(const struct device_node *np,
						const char *propname, const char **out_strs,
						size_t sz)
{
	return of_property_read_string_helper(np, propname, out_strs, sz, 0);
}


static inline int of_property_count_strings(const struct device_node *np,
					    const char *propname)
{
	return of_property_read_string_helper(np, propname, NULL, 0, 0);
}


static inline int of_property_read_string_index(const struct device_node *np,
						const char *propname,
						int index, const char **output)
{
	int rc = of_property_read_string_helper(np, propname, output, 1, index);
	return rc < 0 ? rc : 0;
}


static inline bool of_property_read_bool(const struct device_node *np,
					 const char *propname)
{
	struct property *prop = of_find_property(np, propname, NULL);

	return prop ? true : false;
}

static inline int of_property_read_u8(const struct device_node *np,
				       const char *propname,
				       u8 *out_value)
{
	return of_property_read_u8_array(np, propname, out_value, 1);
}

static inline int of_property_read_u16(const struct device_node *np,
				       const char *propname,
				       u16 *out_value)
{
	return of_property_read_u16_array(np, propname, out_value, 1);
}

static inline int of_property_read_u32(const struct device_node *np,
				       const char *propname,
				       u32 *out_value)
{
	return of_property_read_u32_array(np, propname, out_value, 1);
}

static inline int of_property_read_s32(const struct device_node *np,
				       const char *propname,
				       s32 *out_value)
{
	return of_property_read_u32(np, propname, (u32*) out_value);
}

#define of_for_each_phandle(it, err, np, ln, cn, cc)			\
	for (of_phandle_iterator_init((it), (np), (ln), (cn), (cc)),	\
	     err = of_phandle_iterator_next(it);			\
	     err == 0;							\
	     err = of_phandle_iterator_next(it))

#define of_property_for_each_u32(np, propname, prop, p, u)	\
	for (prop = of_find_property(np, propname, NULL),	\
		p = of_prop_next_u32(prop, NULL, &u);		\
		p;						\
		p = of_prop_next_u32(prop, p, &u))

#define of_property_for_each_string(np, propname, prop, s)	\
	for (prop = of_find_property(np, propname, NULL),	\
		s = of_prop_next_string(prop, NULL);		\
		s;						\
		s = of_prop_next_string(prop, s))

#define for_each_node_by_name(dn, name) \
	for (dn = of_find_node_by_name(NULL, name); dn; \
	     dn = of_find_node_by_name(dn, name))
#define for_each_node_by_type(dn, type) \
	for (dn = of_find_node_by_type(NULL, type); dn; \
	     dn = of_find_node_by_type(dn, type))
#define for_each_compatible_node(dn, type, compatible) \
	for (dn = of_find_compatible_node(NULL, type, compatible); dn; \
	     dn = of_find_compatible_node(dn, type, compatible))
#define for_each_matching_node(dn, matches) \
	for (dn = of_find_matching_node(NULL, matches); dn; \
	     dn = of_find_matching_node(dn, matches))
#define for_each_matching_node_and_match(dn, matches, match) \
	for (dn = of_find_matching_node_and_match(NULL, matches, match); \
	     dn; dn = of_find_matching_node_and_match(dn, matches, match))

#define for_each_child_of_node(parent, child) \
	for (child = of_get_next_child(parent, NULL); child != NULL; \
	     child = of_get_next_child(parent, child))
#define for_each_available_child_of_node(parent, child) \
	for (child = of_get_next_available_child(parent, NULL); child != NULL; \
	     child = of_get_next_available_child(parent, child))

#define for_each_node_with_property(dn, prop_name) \
	for (dn = of_find_node_with_property(NULL, prop_name); dn; \
	     dn = of_find_node_with_property(dn, prop_name))

static inline int of_get_child_count(const struct device_node *np)
{
	struct device_node *child;
	int num = 0;

	for_each_child_of_node(np, child)
		num++;

	return num;
}

static inline int of_get_available_child_count(const struct device_node *np)
{
	struct device_node *child;
	int num = 0;

	for_each_available_child_of_node(np, child)
		num++;

	return num;
}

#if defined(CONFIG_OF) && !defined(MODULE)
#define _OF_DECLARE(table, name, compat, fn, fn_type)			\
	static const struct of_device_id __of_table_##name		\
		__used __section(__##table##_of_table)			\
		__aligned(__alignof__(struct of_device_id))		\
		 = { .compatible = compat,				\
		     .data = (fn == (fn_type)NULL) ? fn : fn  }
#else
#define _OF_DECLARE(table, name, compat, fn, fn_type)			\
	static const struct of_device_id __of_table_##name		\
		__attribute__((unused))					\
		 = { .compatible = compat,				\
		     .data = (fn == (fn_type)NULL) ? fn : fn }
#endif

typedef int (*of_init_fn_2)(struct device_node *, struct device_node *);
typedef int (*of_init_fn_1_ret)(struct device_node *);
typedef void (*of_init_fn_1)(struct device_node *);

#define OF_DECLARE_1(table, name, compat, fn) \
		_OF_DECLARE(table, name, compat, fn, of_init_fn_1)
#define OF_DECLARE_1_RET(table, name, compat, fn) \
		_OF_DECLARE(table, name, compat, fn, of_init_fn_1_ret)
#define OF_DECLARE_2(table, name, compat, fn) \
		_OF_DECLARE(table, name, compat, fn, of_init_fn_2)


struct of_changeset_entry {
	struct list_head node;
	unsigned long action;
	struct device_node *np;
	struct property *prop;
	struct property *old_prop;
};


struct of_changeset {
	struct list_head entries;
};

enum of_reconfig_change {
	OF_RECONFIG_NO_CHANGE = 0,
	OF_RECONFIG_CHANGE_ADD,
	OF_RECONFIG_CHANGE_REMOVE,
};

#ifdef CONFIG_OF_DYNAMIC
extern int of_reconfig_notifier_register(struct notifier_block *);
extern int of_reconfig_notifier_unregister(struct notifier_block *);
extern int of_reconfig_notify(unsigned long, struct of_reconfig_data *rd);
extern int of_reconfig_get_state_change(unsigned long action,
					struct of_reconfig_data *arg);

extern void of_changeset_init(struct of_changeset *ocs);
extern void of_changeset_destroy(struct of_changeset *ocs);
extern int of_changeset_apply(struct of_changeset *ocs);
extern int of_changeset_revert(struct of_changeset *ocs);
extern int of_changeset_action(struct of_changeset *ocs,
		unsigned long action, struct device_node *np,
		struct property *prop);

static inline int of_changeset_attach_node(struct of_changeset *ocs,
		struct device_node *np)
{
	return of_changeset_action(ocs, OF_RECONFIG_ATTACH_NODE, np, NULL);
}

static inline int of_changeset_detach_node(struct of_changeset *ocs,
		struct device_node *np)
{
	return of_changeset_action(ocs, OF_RECONFIG_DETACH_NODE, np, NULL);
}

static inline int of_changeset_add_property(struct of_changeset *ocs,
		struct device_node *np, struct property *prop)
{
	return of_changeset_action(ocs, OF_RECONFIG_ADD_PROPERTY, np, prop);
}

static inline int of_changeset_remove_property(struct of_changeset *ocs,
		struct device_node *np, struct property *prop)
{
	return of_changeset_action(ocs, OF_RECONFIG_REMOVE_PROPERTY, np, prop);
}

static inline int of_changeset_update_property(struct of_changeset *ocs,
		struct device_node *np, struct property *prop)
{
	return of_changeset_action(ocs, OF_RECONFIG_UPDATE_PROPERTY, np, prop);
}
#else 
static inline int of_reconfig_notifier_register(struct notifier_block *nb)
{
	return -EINVAL;
}
static inline int of_reconfig_notifier_unregister(struct notifier_block *nb)
{
	return -EINVAL;
}
static inline int of_reconfig_notify(unsigned long action,
				     struct of_reconfig_data *arg)
{
	return -EINVAL;
}
static inline int of_reconfig_get_state_change(unsigned long action,
						struct of_reconfig_data *arg)
{
	return -EINVAL;
}
#endif 


extern int of_resolve_phandles(struct device_node *tree);


static inline bool of_device_is_system_power_controller(const struct device_node *np)
{
	return of_property_read_bool(np, "system-power-controller");
}



#ifdef CONFIG_OF_OVERLAY


int of_overlay_create(struct device_node *tree);
int of_overlay_destroy(int id);
int of_overlay_destroy_all(void);

#else

static inline int of_overlay_create(struct device_node *tree)
{
	return -ENOTSUPP;
}

static inline int of_overlay_destroy(int id)
{
	return -ENOTSUPP;
}

static inline int of_overlay_destroy_all(void)
{
	return -ENOTSUPP;
}

#endif

#endif 
