

#ifndef LINUX_APPLE_GMUX_H
#define LINUX_APPLE_GMUX_H

#include <linux/acpi.h>

#define GMUX_ACPI_HID "APP000B"

#if IS_ENABLED(CONFIG_APPLE_GMUX)


static inline bool apple_gmux_present(void)
{
	return acpi_dev_found(GMUX_ACPI_HID);
}

#else  

static inline bool apple_gmux_present(void)
{
	return false;
}

#endif 

#endif 
