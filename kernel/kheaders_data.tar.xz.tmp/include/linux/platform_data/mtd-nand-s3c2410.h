

#ifndef __MTD_NAND_S3C2410_H
#define __MTD_NAND_S3C2410_H


struct s3c2410_nand_set {
	unsigned int		disable_ecc:1;
	unsigned int		flash_bbt:1;

	unsigned int		options;
	int			nr_chips;
	int			nr_partitions;
	char			*name;
	int			*nr_map;
	struct mtd_partition	*partitions;
};

struct s3c2410_platform_nand {
	

	int	tacls;	
	int	twrph0;	
	int	twrph1;	

	unsigned int	ignore_unset_ecc:1;

	int			nr_sets;
	struct s3c2410_nand_set *sets;

	void			(*select_chip)(struct s3c2410_nand_set *,
					       int chip);
};


extern void s3c_nand_set_platdata(struct s3c2410_platform_nand *nand);

#endif 
