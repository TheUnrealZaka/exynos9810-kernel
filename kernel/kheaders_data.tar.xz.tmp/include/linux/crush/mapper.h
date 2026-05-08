#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H



#include "crush.h"

extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 const __u32 *weights, int weight_max,
			 int *scratch);

#endif
