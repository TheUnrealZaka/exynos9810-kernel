#ifndef __LINUX_ATA_PLATFORM_H
#define __LINUX_ATA_PLATFORM_H

struct pata_platform_info {
	
	unsigned int ioport_shift;
};

struct scsi_host_template;

extern int __pata_platform_probe(struct device *dev,
				 struct resource *io_res,
				 struct resource *ctl_res,
				 struct resource *irq_res,
				 unsigned int ioport_shift,
				 int __pio_mask,
				 struct scsi_host_template *sht);


struct mv_sata_platform_data {
	int	n_ports; 
};

#endif 
