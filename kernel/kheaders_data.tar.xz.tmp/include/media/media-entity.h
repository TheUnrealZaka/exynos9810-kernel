

#ifndef _MEDIA_ENTITY_H
#define _MEDIA_ENTITY_H

#include <linux/bitmap.h>
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/media.h>




enum media_gobj_type {
	MEDIA_GRAPH_ENTITY,
	MEDIA_GRAPH_PAD,
	MEDIA_GRAPH_LINK,
	MEDIA_GRAPH_INTF_DEVNODE,
};

#define MEDIA_BITS_PER_TYPE		8
#define MEDIA_BITS_PER_ID		(32 - MEDIA_BITS_PER_TYPE)
#define MEDIA_ID_MASK			 GENMASK_ULL(MEDIA_BITS_PER_ID - 1, 0)




struct media_gobj {
	struct media_device	*mdev;
	u32			id;
	struct list_head	list;
};

#define MEDIA_ENTITY_ENUM_MAX_DEPTH	16


struct media_entity_enum {
	unsigned long *bmap;
	int idx_max;
};


struct media_entity_graph {
	struct {
		struct media_entity *entity;
		struct list_head *link;
	} stack[MEDIA_ENTITY_ENUM_MAX_DEPTH];

	struct media_entity_enum ent_enum;
	int top;
};


struct media_pipeline {
	int streaming_count;
	struct media_entity_graph graph;
};


struct media_link {
	struct media_gobj graph_obj;
	struct list_head list;
	union {
		struct media_gobj *gobj0;
		struct media_pad *source;
		struct media_interface *intf;
	};
	union {
		struct media_gobj *gobj1;
		struct media_pad *sink;
		struct media_entity *entity;
	};
	struct media_link *reverse;
	unsigned long flags;
	bool is_backlink;
};


struct media_pad {
	struct media_gobj graph_obj;	
	struct media_entity *entity;
	u16 index;
	unsigned long flags;
};


struct media_entity_operations {
	int (*link_setup)(struct media_entity *entity,
			  const struct media_pad *local,
			  const struct media_pad *remote, u32 flags);
	int (*link_validate)(struct media_link *link);
};


enum media_entity_type {
	MEDIA_ENTITY_TYPE_BASE,
	MEDIA_ENTITY_TYPE_VIDEO_DEVICE,
	MEDIA_ENTITY_TYPE_V4L2_SUBDEV,
};


struct media_entity {
	struct media_gobj graph_obj;	
	const char *name;
	enum media_entity_type obj_type;
	u32 function;
	unsigned long flags;

	u16 num_pads;
	u16 num_links;
	u16 num_backlinks;
	int internal_idx;

	struct media_pad *pads;
	struct list_head links;

	const struct media_entity_operations *ops;

	int stream_count;
	int use_count;

	struct media_pipeline *pipe;

	union {
		struct {
			u32 major;
			u32 minor;
		} dev;
	} info;
};


struct media_interface {
	struct media_gobj		graph_obj;
	struct list_head		links;
	u32				type;
	u32				flags;
};


struct media_intf_devnode {
	struct media_interface		intf;

	
	u32				major;
	u32				minor;
};


static inline u32 media_entity_id(struct media_entity *entity)
{
	return entity->graph_obj.id;
}


static inline enum media_gobj_type media_type(struct media_gobj *gobj)
{
	return gobj->id >> MEDIA_BITS_PER_ID;
}


static inline u32 media_id(struct media_gobj *gobj)
{
	return gobj->id & MEDIA_ID_MASK;
}


static inline u32 media_gobj_gen_id(enum media_gobj_type type, u64 local_id)
{
	u32 id;

	id = type << MEDIA_BITS_PER_ID;
	id |= local_id & MEDIA_ID_MASK;

	return id;
}


static inline bool is_media_entity_v4l2_video_device(struct media_entity *entity)
{
	return entity && entity->obj_type == MEDIA_ENTITY_TYPE_VIDEO_DEVICE;
}


static inline bool is_media_entity_v4l2_subdev(struct media_entity *entity)
{
	return entity && entity->obj_type == MEDIA_ENTITY_TYPE_V4L2_SUBDEV;
}


__must_check int __media_entity_enum_init(struct media_entity_enum *ent_enum,
					  int idx_max);


void media_entity_enum_cleanup(struct media_entity_enum *ent_enum);


static inline void media_entity_enum_zero(struct media_entity_enum *ent_enum)
{
	bitmap_zero(ent_enum->bmap, ent_enum->idx_max);
}


static inline void media_entity_enum_set(struct media_entity_enum *ent_enum,
					 struct media_entity *entity)
{
	if (WARN_ON(entity->internal_idx >= ent_enum->idx_max))
		return;

	__set_bit(entity->internal_idx, ent_enum->bmap);
}


static inline void media_entity_enum_clear(struct media_entity_enum *ent_enum,
					   struct media_entity *entity)
{
	if (WARN_ON(entity->internal_idx >= ent_enum->idx_max))
		return;

	__clear_bit(entity->internal_idx, ent_enum->bmap);
}


static inline bool media_entity_enum_test(struct media_entity_enum *ent_enum,
					  struct media_entity *entity)
{
	if (WARN_ON(entity->internal_idx >= ent_enum->idx_max))
		return true;

	return test_bit(entity->internal_idx, ent_enum->bmap);
}


static inline bool
media_entity_enum_test_and_set(struct media_entity_enum *ent_enum,
			       struct media_entity *entity)
{
	if (WARN_ON(entity->internal_idx >= ent_enum->idx_max))
		return true;

	return __test_and_set_bit(entity->internal_idx, ent_enum->bmap);
}


static inline bool media_entity_enum_empty(struct media_entity_enum *ent_enum)
{
	return bitmap_empty(ent_enum->bmap, ent_enum->idx_max);
}


static inline bool media_entity_enum_intersects(
	struct media_entity_enum *ent_enum1,
	struct media_entity_enum *ent_enum2)
{
	WARN_ON(ent_enum1->idx_max != ent_enum2->idx_max);

	return bitmap_intersects(ent_enum1->bmap, ent_enum2->bmap,
				 min(ent_enum1->idx_max, ent_enum2->idx_max));
}


#define gobj_to_entity(gobj) \
		container_of(gobj, struct media_entity, graph_obj)


#define gobj_to_pad(gobj) \
		container_of(gobj, struct media_pad, graph_obj)


#define gobj_to_link(gobj) \
		container_of(gobj, struct media_link, graph_obj)


#define gobj_to_intf(gobj) \
		container_of(gobj, struct media_interface, graph_obj)


#define intf_to_devnode(intf) \
		container_of(intf, struct media_intf_devnode, intf)


void media_gobj_create(struct media_device *mdev,
		    enum media_gobj_type type,
		    struct media_gobj *gobj);


void media_gobj_destroy(struct media_gobj *gobj);


int media_entity_pads_init(struct media_entity *entity, u16 num_pads,
		      struct media_pad *pads);


static inline void media_entity_cleanup(struct media_entity *entity) {};


__must_check int media_create_pad_link(struct media_entity *source,
			u16 source_pad, struct media_entity *sink,
			u16 sink_pad, u32 flags);


int media_create_pad_links(const struct media_device *mdev,
			   const u32 source_function,
			   struct media_entity *source,
			   const u16 source_pad,
			   const u32 sink_function,
			   struct media_entity *sink,
			   const u16 sink_pad,
			   u32 flags,
			   const bool allow_both_undefined);

void __media_entity_remove_links(struct media_entity *entity);


void media_entity_remove_links(struct media_entity *entity);


int __media_entity_setup_link(struct media_link *link, u32 flags);


int media_entity_setup_link(struct media_link *link, u32 flags);


struct media_link *media_entity_find_link(struct media_pad *source,
		struct media_pad *sink);


struct media_pad *media_entity_remote_pad(struct media_pad *pad);


struct media_entity *media_entity_get(struct media_entity *entity);


__must_check int media_entity_graph_walk_init(
	struct media_entity_graph *graph, struct media_device *mdev);


void media_entity_graph_walk_cleanup(struct media_entity_graph *graph);


void media_entity_put(struct media_entity *entity);


void media_entity_graph_walk_start(struct media_entity_graph *graph,
				   struct media_entity *entity);


struct media_entity *
media_entity_graph_walk_next(struct media_entity_graph *graph);


__must_check int media_entity_pipeline_start(struct media_entity *entity,
					     struct media_pipeline *pipe);

__must_check int __media_entity_pipeline_start(struct media_entity *entity,
					       struct media_pipeline *pipe);


void media_entity_pipeline_stop(struct media_entity *entity);


void __media_entity_pipeline_stop(struct media_entity *entity);


struct media_intf_devnode *
__must_check media_devnode_create(struct media_device *mdev,
				  u32 type, u32 flags,
				  u32 major, u32 minor);

void media_devnode_remove(struct media_intf_devnode *devnode);
struct media_link *


__must_check media_create_intf_link(struct media_entity *entity,
				    struct media_interface *intf,
				    u32 flags);

void __media_remove_intf_link(struct media_link *link);


void media_remove_intf_link(struct media_link *link);


void __media_remove_intf_links(struct media_interface *intf);


void media_remove_intf_links(struct media_interface *intf);



#define media_entity_call(entity, operation, args...)			\
	(((entity)->ops && (entity)->ops->operation) ?			\
	 (entity)->ops->operation((entity) , ##args) : -ENOIOCTLCMD)

#endif
