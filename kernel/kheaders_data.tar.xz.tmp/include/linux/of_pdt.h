

#ifndef _LINUX_OF_PDT_H
#define _LINUX_OF_PDT_H


struct of_pdt_ops {
	
	int (*nextprop)(phandle node, char *prev, char *buf);

	
	int (*getproplen)(phandle node, const char *prop);
	int (*getproperty)(phandle node, const char *prop, char *buf,
			int bufsize);

	
	phandle (*getchild)(phandle parent);
	phandle (*getsibling)(phandle node);

	
	int (*pkg2path)(phandle node, char *buf, const int buflen, int *len);
};

extern void *prom_early_alloc(unsigned long size);


extern void of_pdt_build_devicetree(phandle root_node, struct of_pdt_ops *ops);

extern void (*of_pdt_build_more)(struct device_node *dp);

#endif 
