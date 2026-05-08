



#ifndef __ACLINUXEX_H__
#define __ACLINUXEX_H__

#ifdef __KERNEL__

#ifndef ACPI_USE_NATIVE_DIVIDE

#ifndef ACPI_DIV_64_BY_32
#define ACPI_DIV_64_BY_32(n_hi, n_lo, d32, q32, r32) \
	do { \
		u64 (__n) = ((u64) n_hi) << 32 | (n_lo); \
		(r32) = do_div ((__n), (d32)); \
		(q32) = (u32) (__n); \
	} while (0)
#endif

#ifndef ACPI_SHIFT_RIGHT_64
#define ACPI_SHIFT_RIGHT_64(n_hi, n_lo) \
	do { \
		(n_lo) >>= 1; \
		(n_lo) |= (((n_hi) & 1) << 31); \
		(n_hi) >>= 1; \
	} while (0)
#endif

#endif


acpi_status ACPI_INIT_FUNCTION acpi_os_initialize(void);

acpi_status acpi_os_terminate(void);


static inline void *acpi_os_allocate(acpi_size size)
{
	return kmalloc(size, irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}

static inline void *acpi_os_allocate_zeroed(acpi_size size)
{
	return kzalloc(size, irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}

static inline void acpi_os_free(void *memory)
{
	kfree(memory);
}

static inline void *acpi_os_acquire_object(acpi_cache_t * cache)
{
	return kmem_cache_zalloc(cache,
				 irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}

static inline acpi_thread_id acpi_os_get_thread_id(void)
{
	return (acpi_thread_id) (unsigned long)current;
}


#define acpi_os_create_lock(__handle) \
	({ \
		spinlock_t *lock = ACPI_ALLOCATE(sizeof(*lock)); \
		if (lock) { \
			*(__handle) = lock; \
			spin_lock_init(*(__handle)); \
		} \
		lock ? AE_OK : AE_NO_MEMORY; \
	})

static inline u8 acpi_os_readable(void *pointer, acpi_size length)
{
	return TRUE;
}

static inline acpi_status acpi_os_initialize_command_signals(void)
{
	return AE_OK;
}

static inline void acpi_os_terminate_command_signals(void)
{
	return;
}


void early_acpi_os_unmap_memory(void __iomem * virt, acpi_size size);

#endif				

#endif				
