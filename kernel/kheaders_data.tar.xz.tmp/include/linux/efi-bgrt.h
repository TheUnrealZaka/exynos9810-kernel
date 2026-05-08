#ifndef _LINUX_EFI_BGRT_H
#define _LINUX_EFI_BGRT_H

#ifdef CONFIG_ACPI_BGRT

#include <linux/acpi.h>

void efi_bgrt_init(void);


extern void *bgrt_image;
extern size_t bgrt_image_size;
extern struct acpi_table_bgrt *bgrt_tab;

#else 

static inline void efi_bgrt_init(void) {}

#endif 

#endif 
