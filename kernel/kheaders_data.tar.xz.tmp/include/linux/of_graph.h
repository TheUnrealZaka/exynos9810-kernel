
#ifndef __LINUX_OF_GRAPH_H
#define __LINUX_OF_GRAPH_H

#include <linux/types.h>
#include <linux/errno.h>


struct of_endpoint {
	unsigned int port;
	unsigned int id;
	const struct device_node *local_node;
};


#define for_each_endpoint_of_node(parent, child) \
	for (child = of_graph_get_next_endpoint(parent, NULL); child != NULL; \
	     child = of_graph_get_next_endpoint(parent, child))

#ifdef CONFIG_OF
int of_graph_parse_endpoint(const struct device_node *node,
				struct of_endpoint *endpoint);
struct device_node *of_graph_get_port_by_id(struct device_node *node, u32 id);
struct device_node *of_graph_get_next_endpoint(const struct device_node *parent,
					struct device_node *previous);
struct device_node *of_graph_get_endpoint_by_regs(
		const struct device_node *parent, int port_reg, int reg);
struct device_node *of_graph_get_remote_port_parent(
					const struct device_node *node);
struct device_node *of_graph_get_remote_port(const struct device_node *node);
#else

static inline int of_graph_parse_endpoint(const struct device_node *node,
					struct of_endpoint *endpoint)
{
	return -ENOSYS;
}

static inline struct device_node *of_graph_get_port_by_id(
					struct device_node *node, u32 id)
{
	return NULL;
}

static inline struct device_node *of_graph_get_next_endpoint(
					const struct device_node *parent,
					struct device_node *previous)
{
	return NULL;
}

static inline struct device_node *of_graph_get_endpoint_by_regs(
		const struct device_node *parent, int port_reg, int reg)
{
	return NULL;
}

static inline struct device_node *of_graph_get_remote_port_parent(
					const struct device_node *node)
{
	return NULL;
}

static inline struct device_node *of_graph_get_remote_port(
					const struct device_node *node)
{
	return NULL;
}

#endif 

#endif 
