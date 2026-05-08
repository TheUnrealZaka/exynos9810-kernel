

#ifndef _XEN_ACPI_H
#define _XEN_ACPI_H

#include <linux/types.h>

#ifdef CONFIG_XEN_DOM0
#include <asm/xen/hypervisor.h>
#include <xen/xen.h>
#include <linux/acpi.h>

#define ACPI_MEMORY_DEVICE_CLASS        "memory"
#define ACPI_MEMORY_DEVICE_HID          "PNP0C80"
#define ACPI_MEMORY_DEVICE_NAME         "Hotplug Mem Device"

int xen_stub_memory_device_init(void);
void xen_stub_memory_device_exit(void);

#define ACPI_PROCESSOR_CLASS            "processor"
#define ACPI_PROCESSOR_DEVICE_HID       "ACPI0007"
#define ACPI_PROCESSOR_DEVICE_NAME      "Processor"

int xen_stub_processor_init(void);
void xen_stub_processor_exit(void);

void xen_pcpu_hotplug_sync(void);
int xen_pcpu_id(uint32_t acpi_id);

static inline int xen_acpi_get_pxm(acpi_handle h)
{
	unsigned long long pxm;
	acpi_status status;
	acpi_handle handle;
	acpi_handle phandle = h;

	do {
		handle = phandle;
		status = acpi_evaluate_integer(handle, "_PXM", NULL, &pxm);
		if (ACPI_SUCCESS(status))
			return pxm;
		status = acpi_get_parent(handle, &phandle);
	} while (ACPI_SUCCESS(status));

	return -ENXIO;
}

int xen_acpi_notify_hypervisor_sleep(u8 sleep_state,
				     u32 pm1a_cnt, u32 pm1b_cnd);
int xen_acpi_notify_hypervisor_extended_sleep(u8 sleep_state,
				     u32 val_a, u32 val_b);

static inline int xen_acpi_suspend_lowlevel(void)
{
	
	acpi_enter_sleep_state(ACPI_STATE_S3);
	return 0;
}

static inline void xen_acpi_sleep_register(void)
{
	if (xen_initial_domain()) {
		acpi_os_set_prepare_sleep(
			&xen_acpi_notify_hypervisor_sleep);
		acpi_os_set_prepare_extended_sleep(
			&xen_acpi_notify_hypervisor_extended_sleep);

		acpi_suspend_lowlevel = xen_acpi_suspend_lowlevel;
	}
}
#else
static inline void xen_acpi_sleep_register(void)
{
}
#endif

#endif	
