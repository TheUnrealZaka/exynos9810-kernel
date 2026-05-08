
#ifndef _AGP_H
#define _AGP_H 1

#include <linux/mutex.h>
#include <linux/agp_backend.h>
#include <uapi/linux/agpgart.h>

#define AGPGART_MINOR 175

struct agp_info {
	struct agp_version version;	
	u32 bridge_id;		
	u32 agp_mode;		
	unsigned long aper_base;
	size_t aper_size;	
	size_t pg_total;	
	size_t pg_system;	
	size_t pg_used;		
};

struct agp_setup {
	u32 agp_mode;		
};


struct agp_segment {
	off_t pg_start;		
	size_t pg_count;	
	int prot;		
};

struct agp_segment_priv {
	off_t pg_start;
	size_t pg_count;
	pgprot_t prot;
};

struct agp_region {
	pid_t pid;		
	size_t seg_count;	
	struct agp_segment *seg_list;
};

struct agp_allocate {
	int key;		
	size_t pg_count;	
	u32 type;		
	u32 physical;           
};

struct agp_bind {
	int key;		
	off_t pg_start;		
};

struct agp_unbind {
	int key;		
	u32 priority;		
};

struct agp_client {
	struct agp_client *next;
	struct agp_client *prev;
	pid_t pid;
	int num_segments;
	struct agp_segment_priv **segments;
};

struct agp_controller {
	struct agp_controller *next;
	struct agp_controller *prev;
	pid_t pid;
	int num_clients;
	struct agp_memory *pool;
	struct agp_client *clients;
};

#define AGP_FF_ALLOW_CLIENT		0
#define AGP_FF_ALLOW_CONTROLLER 	1
#define AGP_FF_IS_CLIENT		2
#define AGP_FF_IS_CONTROLLER		3
#define AGP_FF_IS_VALID 		4

struct agp_file_private {
	struct agp_file_private *next;
	struct agp_file_private *prev;
	pid_t my_pid;
	unsigned long access_flags;	
};

struct agp_front_data {
	struct mutex agp_mutex;
	struct agp_controller *current_controller;
	struct agp_controller *controllers;
	struct agp_file_private *file_priv_list;
	bool used_by_controller;
	bool backend_acquired;
};

#endif				
