



#ifndef __ACPIOSXF_H__
#define __ACPIOSXF_H__

#include <acpi/platform/acenv.h>
#include <acpi/actypes.h>



typedef enum {
	OSL_GLOBAL_LOCK_HANDLER,
	OSL_NOTIFY_HANDLER,
	OSL_GPE_HANDLER,
	OSL_DEBUGGER_MAIN_THREAD,
	OSL_DEBUGGER_EXEC_THREAD,
	OSL_EC_POLL_HANDLER,
	OSL_EC_BURST_HANDLER
} acpi_execute_type;

#define ACPI_NO_UNIT_LIMIT          ((u32) -1)
#define ACPI_MUTEX_SEM              1



#define ACPI_SIGNAL_FATAL           0
#define ACPI_SIGNAL_BREAKPOINT      1

struct acpi_signal_fatal_info {
	u32 type;
	u32 code;
	u32 argument;
};


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_initialize
acpi_status acpi_os_initialize(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_terminate
acpi_status acpi_os_terminate(void);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_root_pointer
acpi_physical_address acpi_os_get_root_pointer(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_predefined_override
acpi_status
acpi_os_predefined_override(const struct acpi_predefined_names *init_val,
			    acpi_string *new_val);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_table_override
acpi_status
acpi_os_table_override(struct acpi_table_header *existing_table,
		       struct acpi_table_header **new_table);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_physical_table_override
acpi_status
acpi_os_physical_table_override(struct acpi_table_header *existing_table,
				acpi_physical_address *new_address,
				u32 *new_table_length);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_create_lock
acpi_status acpi_os_create_lock(acpi_spinlock * out_handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_delete_lock
void acpi_os_delete_lock(acpi_spinlock handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_acquire_lock
acpi_cpu_flags acpi_os_acquire_lock(acpi_spinlock handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_release_lock
void acpi_os_release_lock(acpi_spinlock handle, acpi_cpu_flags flags);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_create_semaphore
acpi_status
acpi_os_create_semaphore(u32 max_units,
			 u32 initial_units, acpi_semaphore * out_handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_delete_semaphore
acpi_status acpi_os_delete_semaphore(acpi_semaphore handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_wait_semaphore
acpi_status
acpi_os_wait_semaphore(acpi_semaphore handle, u32 units, u16 timeout);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_signal_semaphore
acpi_status acpi_os_signal_semaphore(acpi_semaphore handle, u32 units);
#endif


#if (ACPI_MUTEX_TYPE != ACPI_BINARY_SEMAPHORE)

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_create_mutex
acpi_status acpi_os_create_mutex(acpi_mutex * out_handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_delete_mutex
void acpi_os_delete_mutex(acpi_mutex handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_acquire_mutex
acpi_status acpi_os_acquire_mutex(acpi_mutex handle, u16 timeout);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_release_mutex
void acpi_os_release_mutex(acpi_mutex handle);
#endif

#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_allocate
void *acpi_os_allocate(acpi_size size);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_allocate_zeroed
void *acpi_os_allocate_zeroed(acpi_size size);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_free
void acpi_os_free(void *memory);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_map_memory
void *acpi_os_map_memory(acpi_physical_address where, acpi_size length);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_unmap_memory
void acpi_os_unmap_memory(void *logical_address, acpi_size size);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_physical_address
acpi_status
acpi_os_get_physical_address(void *logical_address,
			     acpi_physical_address *physical_address);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_create_cache
acpi_status
acpi_os_create_cache(char *cache_name,
		     u16 object_size,
		     u16 max_depth, acpi_cache_t ** return_cache);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_delete_cache
acpi_status acpi_os_delete_cache(acpi_cache_t * cache);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_purge_cache
acpi_status acpi_os_purge_cache(acpi_cache_t * cache);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_acquire_object
void *acpi_os_acquire_object(acpi_cache_t * cache);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_release_object
acpi_status acpi_os_release_object(acpi_cache_t * cache, void *object);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_install_interrupt_handler
acpi_status
acpi_os_install_interrupt_handler(u32 interrupt_number,
				  acpi_osd_handler service_routine,
				  void *context);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_remove_interrupt_handler
acpi_status
acpi_os_remove_interrupt_handler(u32 interrupt_number,
				 acpi_osd_handler service_routine);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_thread_id
acpi_thread_id acpi_os_get_thread_id(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_execute
acpi_status
acpi_os_execute(acpi_execute_type type,
		acpi_osd_exec_callback function, void *context);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_wait_events_complete
void acpi_os_wait_events_complete(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_sleep
void acpi_os_sleep(u64 milliseconds);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_stall
void acpi_os_stall(u32 microseconds);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_read_port
acpi_status acpi_os_read_port(acpi_io_address address, u32 *value, u32 width);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_write_port
acpi_status acpi_os_write_port(acpi_io_address address, u32 value, u32 width);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_read_memory
acpi_status
acpi_os_read_memory(acpi_physical_address address, u64 *value, u32 width);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_write_memory
acpi_status
acpi_os_write_memory(acpi_physical_address address, u64 value, u32 width);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_read_pci_configuration
acpi_status
acpi_os_read_pci_configuration(struct acpi_pci_id *pci_id,
			       u32 reg, u64 *value, u32 width);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_write_pci_configuration
acpi_status
acpi_os_write_pci_configuration(struct acpi_pci_id *pci_id,
				u32 reg, u64 value, u32 width);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_readable
u8 acpi_os_readable(void *pointer, acpi_size length);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_writable
u8 acpi_os_writable(void *pointer, acpi_size length);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_timer
u64 acpi_os_get_timer(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_signal
acpi_status acpi_os_signal(u32 function, void *info);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_printf
void ACPI_INTERNAL_VAR_XFACE acpi_os_printf(const char *format, ...);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_vprintf
void acpi_os_vprintf(const char *format, va_list args);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_redirect_output
void acpi_os_redirect_output(void *destination);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_line
acpi_status acpi_os_get_line(char *buffer, u32 buffer_length, u32 *bytes_read);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_initialize_command_signals
acpi_status acpi_os_initialize_command_signals(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_terminate_command_signals
void acpi_os_terminate_command_signals(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_wait_command_ready
acpi_status acpi_os_wait_command_ready(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_notify_command_complete
acpi_status acpi_os_notify_command_complete(void);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_trace_point
void
acpi_os_trace_point(acpi_trace_event_type type,
		    u8 begin, u8 *aml, char *pathname);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_name
acpi_status
acpi_os_get_table_by_name(char *signature,
			  u32 instance,
			  struct acpi_table_header **table,
			  acpi_physical_address *address);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_index
acpi_status
acpi_os_get_table_by_index(u32 index,
			   struct acpi_table_header **table,
			   u32 *instance, acpi_physical_address *address);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_address
acpi_status
acpi_os_get_table_by_address(acpi_physical_address address,
			     struct acpi_table_header **table);
#endif


#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_open_directory
void *acpi_os_open_directory(char *pathname,
			     char *wildcard_spec, char requested_file_type);
#endif



#define REQUEST_FILE_ONLY                   0
#define REQUEST_DIR_ONLY                    1

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_next_filename
char *acpi_os_get_next_filename(void *dir_handle);
#endif

#ifndef ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_close_directory
void acpi_os_close_directory(void *dir_handle);
#endif

#endif				
