



#ifndef _LINUX_SFI_ACPI_H
#define _LINUX_SFI_ACPI_H

#include <linux/acpi.h>
#include <linux/sfi.h>

#ifdef CONFIG_SFI
extern int sfi_acpi_table_parse(char *signature, char *oem_id,
				char *oem_table_id,
				int (*handler)(struct acpi_table_header *));

static inline int __init acpi_sfi_table_parse(char *signature,
				int (*handler)(struct acpi_table_header *))
{
	if (!acpi_table_parse(signature, handler))
		return 0;

	return sfi_acpi_table_parse(signature, NULL, NULL, handler);
}
#else 
static inline int sfi_acpi_table_parse(char *signature, char *oem_id,
				char *oem_table_id,
				int (*handler)(struct acpi_table_header *))
{
	return -1;
}

static inline int __init acpi_sfi_table_parse(char *signature,
				int (*handler)(struct acpi_table_header *))
{
	return acpi_table_parse(signature, handler);
}
#endif 

#endif 
