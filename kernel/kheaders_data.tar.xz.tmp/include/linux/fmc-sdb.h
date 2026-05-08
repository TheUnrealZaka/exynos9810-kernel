
#include <linux/sdb.h>


union sdb_record {
	struct sdb_interconnect ic;
	struct sdb_device dev;
	struct sdb_bridge bridge;
	struct sdb_integration integr;
	struct sdb_empty empty;
	struct sdb_synthesis synthesis;
	struct sdb_repo_url repo_url;
};

struct fmc_device;


struct sdb_array {
	int len;
	int level;
	unsigned long baseaddr;
	struct fmc_device *fmc;		
	struct sdb_array *parent;	
	union sdb_record *record;	
	struct sdb_array **subtree;	
};

extern int fmc_scan_sdb_tree(struct fmc_device *fmc, unsigned long address);
extern void fmc_show_sdb_tree(const struct fmc_device *fmc);
extern signed long fmc_find_sdb_device(struct sdb_array *tree, uint64_t vendor,
				       uint32_t device, unsigned long *sz);
extern int fmc_free_sdb_tree(struct fmc_device *fmc);
