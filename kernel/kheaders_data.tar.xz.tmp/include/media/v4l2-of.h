
#ifndef _V4L2_OF_H
#define _V4L2_OF_H

#include <linux/list.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/of_graph.h>

#include <media/v4l2-mediabus.h>

struct device_node;


struct v4l2_of_bus_mipi_csi2 {
	unsigned int flags;
	unsigned char data_lanes[4];
	unsigned char clock_lane;
	unsigned short num_data_lanes;
	bool lane_polarities[5];
};


struct v4l2_of_bus_parallel {
	unsigned int flags;
	unsigned char bus_width;
	unsigned char data_shift;
};


struct v4l2_of_endpoint {
	struct of_endpoint base;
	
	enum v4l2_mbus_type bus_type;
	union {
		struct v4l2_of_bus_parallel parallel;
		struct v4l2_of_bus_mipi_csi2 mipi_csi2;
	} bus;
	u64 *link_frequencies;
	unsigned int nr_of_link_frequencies;
};


struct v4l2_of_link {
	struct device_node *local_node;
	unsigned int local_port;
	struct device_node *remote_node;
	unsigned int remote_port;
};

#ifdef CONFIG_OF
int v4l2_of_parse_endpoint(const struct device_node *node,
			   struct v4l2_of_endpoint *endpoint);
struct v4l2_of_endpoint *v4l2_of_alloc_parse_endpoint(
	const struct device_node *node);
void v4l2_of_free_endpoint(struct v4l2_of_endpoint *endpoint);
int v4l2_of_parse_link(const struct device_node *node,
		       struct v4l2_of_link *link);
void v4l2_of_put_link(struct v4l2_of_link *link);
#else 

static inline int v4l2_of_parse_endpoint(const struct device_node *node,
					struct v4l2_of_endpoint *link)
{
	return -ENOSYS;
}

static inline struct v4l2_of_endpoint *v4l2_of_alloc_parse_endpoint(
	const struct device_node *node)
{
	return NULL;
}

static inline void v4l2_of_free_endpoint(struct v4l2_of_endpoint *endpoint)
{
}

static inline int v4l2_of_parse_link(const struct device_node *node,
				     struct v4l2_of_link *link)
{
	return -ENOSYS;
}

static inline void v4l2_of_put_link(struct v4l2_of_link *link)
{
}

#endif 

#endif 
