

#ifndef MTD_PARTITIONS_H
#define MTD_PARTITIONS_H

#include <linux/types.h>




struct mtd_partition {
	const char *name;		
	uint64_t size;			
	uint64_t offset;		
	uint32_t mask_flags;		
};

#define MTDPART_OFS_RETAIN	(-3)
#define MTDPART_OFS_NXTBLK	(-2)
#define MTDPART_OFS_APPEND	(-1)
#define MTDPART_SIZ_FULL	(0)


struct mtd_info;
struct device_node;


struct mtd_part_parser_data {
	unsigned long origin;
};




struct mtd_part_parser {
	struct list_head list;
	struct module *owner;
	const char *name;
	int (*parse_fn)(struct mtd_info *, const struct mtd_partition **,
			struct mtd_part_parser_data *);
	void (*cleanup)(const struct mtd_partition *pparts, int nr_parts);
};


struct mtd_partitions {
	const struct mtd_partition *parts;
	int nr_parts;
	const struct mtd_part_parser *parser;
};

extern int __register_mtd_parser(struct mtd_part_parser *parser,
				 struct module *owner);
#define register_mtd_parser(parser) __register_mtd_parser(parser, THIS_MODULE)

extern void deregister_mtd_parser(struct mtd_part_parser *parser);


#define module_mtd_part_parser(__mtd_part_parser) \
	module_driver(__mtd_part_parser, register_mtd_parser, \
		      deregister_mtd_parser)

int mtd_is_partition(const struct mtd_info *mtd);
int mtd_add_partition(struct mtd_info *master, const char *name,
		      long long offset, long long length);
int mtd_del_partition(struct mtd_info *master, int partno);
uint64_t mtd_get_device_size(const struct mtd_info *mtd);

#endif
