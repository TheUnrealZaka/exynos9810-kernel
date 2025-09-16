

#ifndef __LINUX_MTD_PLATRAM_H
#define __LINUX_MTD_PLATRAM_H __FILE__

#define PLATRAM_RO (0)
#define PLATRAM_RW (1)

struct platdata_mtd_ram {
	const char		*mapname;
	const char * const      *map_probes;
	const char * const      *probes;
	struct mtd_partition	*partitions;
	int			 nr_partitions;
	int			 bankwidth;

	

	void	(*set_rw)(struct device *dev, int to);
};

#endif 
