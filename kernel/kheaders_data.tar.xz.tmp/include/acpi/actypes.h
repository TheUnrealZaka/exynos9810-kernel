



#ifndef __ACTYPES_H__
#define __ACTYPES_H__




#ifndef ACPI_MACHINE_WIDTH
#error ACPI_MACHINE_WIDTH not defined
#endif


#define ACPI_UINT8_MAX                  (u8) (~((u8)  0))	
#define ACPI_UINT16_MAX                 (u16)(~((u16) 0))	
#define ACPI_UINT32_MAX                 (u32)(~((u32) 0))	
#define ACPI_UINT64_MAX                 (u64)(~((u64) 0))	
#define ACPI_ASCII_MAX                  0x7F





#ifndef ACPI_USE_SYSTEM_INTTYPES

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef COMPILER_DEPENDENT_UINT64 u64;
typedef COMPILER_DEPENDENT_INT64 s64;

#endif				


#define acpi_thread_id                  u64



#if ACPI_MACHINE_WIDTH == 64

#ifndef ACPI_USE_SYSTEM_INTTYPES

typedef unsigned int u32;
typedef int s32;

#endif				

typedef s64 acpi_native_int;

typedef u64 acpi_size;
typedef u64 acpi_io_address;
typedef u64 acpi_physical_address;

#define ACPI_MAX_PTR                    ACPI_UINT64_MAX
#define ACPI_SIZE_MAX                   ACPI_UINT64_MAX

#define ACPI_USE_NATIVE_DIVIDE	


#if defined (__IA64__) || defined (__ia64__)
#define ACPI_MISALIGNMENT_NOT_SUPPORTED
#endif



#elif ACPI_MACHINE_WIDTH == 32

#ifndef ACPI_USE_SYSTEM_INTTYPES

typedef unsigned int u32;
typedef int s32;

#endif				

typedef s32 acpi_native_int;

typedef u32 acpi_size;

#ifdef ACPI_32BIT_PHYSICAL_ADDRESS


typedef u32 acpi_io_address;
typedef u32 acpi_physical_address;

#else				


typedef u64 acpi_io_address;
typedef u64 acpi_physical_address;

#endif				

#define ACPI_MAX_PTR                    ACPI_UINT32_MAX
#define ACPI_SIZE_MAX                   ACPI_UINT32_MAX

#else



#error unknown ACPI_MACHINE_WIDTH
#endif





#ifndef acpi_cpu_flags
#define acpi_cpu_flags			acpi_size
#endif



#ifndef acpi_cache_t
#ifdef ACPI_USE_LOCAL_CACHE
#define acpi_cache_t                    struct acpi_memory_list
#else
#define acpi_cache_t                    void *
#endif
#endif


#if (ACPI_MUTEX_TYPE == ACPI_BINARY_SEMAPHORE)

#define acpi_mutex                      acpi_semaphore
#define acpi_os_create_mutex(out_handle) acpi_os_create_semaphore (1, 1, out_handle)
#define acpi_os_delete_mutex(handle)    (void) acpi_os_delete_semaphore (handle)
#define acpi_os_acquire_mutex(handle,time) acpi_os_wait_semaphore (handle, 1, time)
#define acpi_os_release_mutex(handle)   (void) acpi_os_signal_semaphore (handle, 1)
#endif



#ifndef acpi_spinlock
#define acpi_spinlock                   void *
#endif

#ifndef acpi_semaphore
#define acpi_semaphore                  void *
#endif

#ifndef acpi_mutex
#define acpi_mutex                      void *
#endif





#ifndef acpi_uintptr_t
#define acpi_uintptr_t                  void *
#endif


#ifndef ACPI_PRINTF_LIKE
#define ACPI_PRINTF_LIKE(c)
#endif


#ifndef ACPI_UNUSED_VAR
#define ACPI_UNUSED_VAR
#endif


#ifndef ACPI_EXPORT_SYMBOL_INIT
#define ACPI_EXPORT_SYMBOL_INIT(symbol)
#endif

#ifndef ACPI_EXPORT_SYMBOL
#define ACPI_EXPORT_SYMBOL(symbol)
#endif


#ifndef ACPI_DEBUG_INITIALIZE
#define ACPI_DEBUG_INITIALIZE()
#endif



#ifdef ACPI_NO_MEM_ALLOCATIONS

#define ACPI_ALLOCATE(a)                NULL
#define ACPI_ALLOCATE_ZEROED(a)         NULL
#define ACPI_FREE(a)
#define ACPI_MEM_TRACKING(a)

#else				

#ifdef ACPI_DBG_TRACK_ALLOCATIONS

#define ACPI_MEM_PARAMETERS             _COMPONENT, _acpi_module_name, __LINE__
#define ACPI_ALLOCATE(a)                acpi_ut_allocate_and_track ((acpi_size) (a), ACPI_MEM_PARAMETERS)
#define ACPI_ALLOCATE_ZEROED(a)         acpi_ut_allocate_zeroed_and_track ((acpi_size) (a), ACPI_MEM_PARAMETERS)
#define ACPI_FREE(a)                    acpi_ut_free_and_track (a, ACPI_MEM_PARAMETERS)
#define ACPI_MEM_TRACKING(a)            a

#else

#define ACPI_ALLOCATE(a)                acpi_os_allocate ((acpi_size) (a))
#define ACPI_ALLOCATE_ZEROED(a)         acpi_os_allocate_zeroed ((acpi_size) (a))
#define ACPI_FREE(a)                    acpi_os_free (a)
#define ACPI_MEM_TRACKING(a)

#endif				

#endif				





#define ACPI_MAX_GPE_BLOCKS             2



#define ACPI_GPE_REGISTER_WIDTH         8
#define ACPI_PM1_REGISTER_WIDTH         16
#define ACPI_PM2_REGISTER_WIDTH         8
#define ACPI_PM_TIMER_WIDTH             32
#define ACPI_RESET_REGISTER_WIDTH       8



#define ACPI_NAME_SIZE                  4
#define ACPI_PATH_SEGMENT_LENGTH        5	
#define ACPI_PATH_SEPARATOR             '.'



#define ACPI_OEM_ID_SIZE                6
#define ACPI_OEM_TABLE_ID_SIZE          8



#define PCI_ROOT_HID_STRING             "PNP0A03"
#define PCI_EXPRESS_ROOT_HID_STRING     "PNP0A08"



#define ACPI_PM_TIMER_FREQUENCY         3579545





#ifdef FALSE
#undef FALSE
#endif
#define FALSE                           (1 == 0)

#ifdef TRUE
#undef TRUE
#endif
#define TRUE                            (1 == 1)

#ifndef NULL
#define NULL                            (void *) 0
#endif


typedef u32 acpi_status;	
typedef u32 acpi_name;		
typedef char *acpi_string;	
typedef void *acpi_handle;	



#define ACPI_MSEC_PER_SEC               1000L

#define ACPI_USEC_PER_MSEC              1000L
#define ACPI_USEC_PER_SEC               1000000L

#define ACPI_100NSEC_PER_USEC           10L
#define ACPI_100NSEC_PER_MSEC           10000L
#define ACPI_100NSEC_PER_SEC            10000000L

#define ACPI_NSEC_PER_USEC              1000L
#define ACPI_NSEC_PER_MSEC              1000000L
#define ACPI_NSEC_PER_SEC               1000000000L



typedef u8 acpi_owner_id;
#define ACPI_OWNER_ID_MAX               0xFF

#define ACPI_INTEGER_BIT_SIZE           64
#define ACPI_MAX_DECIMAL_DIGITS         20	
#define ACPI_MAX64_DECIMAL_DIGITS       20
#define ACPI_MAX32_DECIMAL_DIGITS       10
#define ACPI_MAX16_DECIMAL_DIGITS        5
#define ACPI_MAX8_DECIMAL_DIGITS         3


#define ACPI_ROOT_OBJECT                ACPI_ADD_PTR (acpi_handle, NULL, ACPI_MAX_PTR)
#define ACPI_WAIT_FOREVER               0xFFFF	
#define ACPI_DO_NOT_WAIT                0


typedef u64 acpi_integer;
#define ACPI_INTEGER_MAX                ACPI_UINT64_MAX





#define ACPI_LOBYTE(integer)            ((u8)   (u16)(integer))
#define ACPI_HIBYTE(integer)            ((u8) (((u16)(integer)) >> 8))
#define ACPI_LOWORD(integer)            ((u16)  (u32)(integer))
#define ACPI_HIWORD(integer)            ((u16)(((u32)(integer)) >> 16))
#define ACPI_LODWORD(integer64)         ((u32)  (u64)(integer64))
#define ACPI_HIDWORD(integer64)         ((u32)(((u64)(integer64)) >> 32))

#define ACPI_SET_BIT(target,bit)        ((target) |= (bit))
#define ACPI_CLEAR_BIT(target,bit)      ((target) &= ~(bit))
#define ACPI_MIN(a,b)                   (((a)<(b))?(a):(b))
#define ACPI_MAX(a,b)                   (((a)>(b))?(a):(b))



#define ACPI_ARRAY_LENGTH(x)            (sizeof(x) / sizeof((x)[0]))



#define ACPI_CAST_PTR(t, p)             ((t *) (acpi_uintptr_t) (p))
#define ACPI_CAST_INDIRECT_PTR(t, p)    ((t **) (acpi_uintptr_t) (p))
#define ACPI_ADD_PTR(t, a, b)           ACPI_CAST_PTR (t, (ACPI_CAST_PTR (u8, (a)) + (acpi_size)(b)))
#define ACPI_SUB_PTR(t, a, b)           ACPI_CAST_PTR (t, (ACPI_CAST_PTR (u8, (a)) - (acpi_size)(b)))
#define ACPI_PTR_DIFF(a, b)             (acpi_size) (ACPI_CAST_PTR (u8, (a)) - ACPI_CAST_PTR (u8, (b)))



#define ACPI_TO_POINTER(i)              ACPI_ADD_PTR (void, (void *) NULL,(acpi_size) i)
#define ACPI_TO_INTEGER(p)              ACPI_PTR_DIFF (p, (void *) NULL)
#define ACPI_OFFSET(d, f)               ACPI_PTR_DIFF (&(((d *) 0)->f), (void *) NULL)
#define ACPI_PHYSADDR_TO_PTR(i)         ACPI_TO_POINTER(i)
#define ACPI_PTR_TO_PHYSADDR(i)         ACPI_TO_INTEGER(i)



#ifndef ACPI_MISALIGNMENT_NOT_SUPPORTED
#define ACPI_COMPARE_NAME(a,b)          (*ACPI_CAST_PTR (u32, (a)) == *ACPI_CAST_PTR (u32, (b)))
#define ACPI_MOVE_NAME(dest,src)        (*ACPI_CAST_PTR (u32, (dest)) = *ACPI_CAST_PTR (u32, (src)))
#else
#define ACPI_COMPARE_NAME(a,b)          (!strncmp (ACPI_CAST_PTR (char, (a)), ACPI_CAST_PTR (char, (b)), ACPI_NAME_SIZE))
#define ACPI_MOVE_NAME(dest,src)        (strncpy (ACPI_CAST_PTR (char, (dest)), ACPI_CAST_PTR (char, (src)), ACPI_NAME_SIZE))
#endif



#define ACPI_VALIDATE_RSDP_SIG(a)       (!strncmp (ACPI_CAST_PTR (char, (a)), ACPI_SIG_RSDP, 8))
#define ACPI_MAKE_RSDP_SIG(dest)        (memcpy (ACPI_CAST_PTR (char, (dest)), ACPI_SIG_RSDP, 8))

#define ACPI_ACCESS_BYTE_WIDTH(size)    (1 << ((size) - 1))




#define ACPI_FULL_INITIALIZATION        0x00
#define ACPI_NO_ADDRESS_SPACE_INIT      0x01
#define ACPI_NO_HARDWARE_INIT           0x02
#define ACPI_NO_EVENT_INIT              0x04
#define ACPI_NO_HANDLER_INIT            0x08
#define ACPI_NO_ACPI_ENABLE             0x10
#define ACPI_NO_DEVICE_INIT             0x20
#define ACPI_NO_OBJECT_INIT             0x40
#define ACPI_NO_FACS_INIT               0x80


#define ACPI_SUBSYSTEM_INITIALIZE       0x01
#define ACPI_INITIALIZED_OK             0x02


#define ACPI_STATE_UNKNOWN              (u8) 0xFF

#define ACPI_STATE_S0                   (u8) 0
#define ACPI_STATE_S1                   (u8) 1
#define ACPI_STATE_S2                   (u8) 2
#define ACPI_STATE_S3                   (u8) 3
#define ACPI_STATE_S4                   (u8) 4
#define ACPI_STATE_S5                   (u8) 5
#define ACPI_S_STATES_MAX               ACPI_STATE_S5
#define ACPI_S_STATE_COUNT              6

#define ACPI_STATE_D0                   (u8) 0
#define ACPI_STATE_D1                   (u8) 1
#define ACPI_STATE_D2                   (u8) 2
#define ACPI_STATE_D3_HOT               (u8) 3
#define ACPI_STATE_D3                   (u8) 4
#define ACPI_STATE_D3_COLD              ACPI_STATE_D3
#define ACPI_D_STATES_MAX               ACPI_STATE_D3
#define ACPI_D_STATE_COUNT              5

#define ACPI_STATE_C0                   (u8) 0
#define ACPI_STATE_C1                   (u8) 1
#define ACPI_STATE_C2                   (u8) 2
#define ACPI_STATE_C3                   (u8) 3
#define ACPI_C_STATES_MAX               ACPI_STATE_C3
#define ACPI_C_STATE_COUNT              4


#define ACPI_SLEEP_TYPE_MAX             0x7
#define ACPI_SLEEP_TYPE_INVALID         0xFF


#define ACPI_NOTIFY_BUS_CHECK           (u8) 0x00
#define ACPI_NOTIFY_DEVICE_CHECK        (u8) 0x01
#define ACPI_NOTIFY_DEVICE_WAKE         (u8) 0x02
#define ACPI_NOTIFY_EJECT_REQUEST       (u8) 0x03
#define ACPI_NOTIFY_DEVICE_CHECK_LIGHT  (u8) 0x04
#define ACPI_NOTIFY_FREQUENCY_MISMATCH  (u8) 0x05
#define ACPI_NOTIFY_BUS_MODE_MISMATCH   (u8) 0x06
#define ACPI_NOTIFY_POWER_FAULT         (u8) 0x07
#define ACPI_NOTIFY_CAPABILITIES_CHECK  (u8) 0x08
#define ACPI_NOTIFY_DEVICE_PLD_CHECK    (u8) 0x09
#define ACPI_NOTIFY_RESERVED            (u8) 0x0A
#define ACPI_NOTIFY_LOCALITY_UPDATE     (u8) 0x0B
#define ACPI_NOTIFY_SHUTDOWN_REQUEST    (u8) 0x0C
#define ACPI_NOTIFY_AFFINITY_UPDATE     (u8) 0x0D

#define ACPI_GENERIC_NOTIFY_MAX         0x0D
#define ACPI_SPECIFIC_NOTIFY_MAX        0x84


typedef u32 acpi_object_type;

#define ACPI_TYPE_ANY                   0x00
#define ACPI_TYPE_INTEGER               0x01	
#define ACPI_TYPE_STRING                0x02
#define ACPI_TYPE_BUFFER                0x03
#define ACPI_TYPE_PACKAGE               0x04	
#define ACPI_TYPE_FIELD_UNIT            0x05
#define ACPI_TYPE_DEVICE                0x06	
#define ACPI_TYPE_EVENT                 0x07
#define ACPI_TYPE_METHOD                0x08	
#define ACPI_TYPE_MUTEX                 0x09
#define ACPI_TYPE_REGION                0x0A
#define ACPI_TYPE_POWER                 0x0B	
#define ACPI_TYPE_PROCESSOR             0x0C	
#define ACPI_TYPE_THERMAL               0x0D	
#define ACPI_TYPE_BUFFER_FIELD          0x0E
#define ACPI_TYPE_DDB_HANDLE            0x0F
#define ACPI_TYPE_DEBUG_OBJECT          0x10

#define ACPI_TYPE_EXTERNAL_MAX          0x10
#define ACPI_NUM_TYPES                  (ACPI_TYPE_EXTERNAL_MAX + 1)


#define ACPI_TYPE_LOCAL_REGION_FIELD    0x11
#define ACPI_TYPE_LOCAL_BANK_FIELD      0x12
#define ACPI_TYPE_LOCAL_INDEX_FIELD     0x13
#define ACPI_TYPE_LOCAL_REFERENCE       0x14	
#define ACPI_TYPE_LOCAL_ALIAS           0x15
#define ACPI_TYPE_LOCAL_METHOD_ALIAS    0x16
#define ACPI_TYPE_LOCAL_NOTIFY          0x17
#define ACPI_TYPE_LOCAL_ADDRESS_HANDLER 0x18
#define ACPI_TYPE_LOCAL_RESOURCE        0x19
#define ACPI_TYPE_LOCAL_RESOURCE_FIELD  0x1A
#define ACPI_TYPE_LOCAL_SCOPE           0x1B	

#define ACPI_TYPE_NS_NODE_MAX           0x1B	
#define ACPI_TOTAL_TYPES                (ACPI_TYPE_NS_NODE_MAX + 1)


#define ACPI_TYPE_LOCAL_EXTRA           0x1C
#define ACPI_TYPE_LOCAL_DATA            0x1D

#define ACPI_TYPE_LOCAL_MAX             0x1D



#define ACPI_TYPE_INVALID               0x1E
#define ACPI_TYPE_NOT_FOUND             0xFF

#define ACPI_NUM_NS_TYPES               (ACPI_TYPE_INVALID + 1)


#define ACPI_READ                       0
#define ACPI_WRITE                      1
#define ACPI_IO_MASK                    1


typedef u32 acpi_event_type;


#define ACPI_EVENT_PMTIMER              0
#define ACPI_EVENT_GLOBAL               1
#define ACPI_EVENT_POWER_BUTTON         2
#define ACPI_EVENT_SLEEP_BUTTON         3
#define ACPI_EVENT_RTC                  4
#define ACPI_EVENT_MAX                  4
#define ACPI_NUM_FIXED_EVENTS           ACPI_EVENT_MAX + 1


typedef u32 acpi_event_status;

#define ACPI_EVENT_FLAG_DISABLED        (acpi_event_status) 0x00
#define ACPI_EVENT_FLAG_ENABLED         (acpi_event_status) 0x01
#define ACPI_EVENT_FLAG_WAKE_ENABLED    (acpi_event_status) 0x02
#define ACPI_EVENT_FLAG_STATUS_SET      (acpi_event_status) 0x04
#define ACPI_EVENT_FLAG_ENABLE_SET      (acpi_event_status) 0x08
#define ACPI_EVENT_FLAG_HAS_HANDLER     (acpi_event_status) 0x10
#define ACPI_EVENT_FLAG_MASKED          (acpi_event_status) 0x20
#define ACPI_EVENT_FLAG_SET             ACPI_EVENT_FLAG_STATUS_SET



#define ACPI_GPE_ENABLE                 0
#define ACPI_GPE_DISABLE                1
#define ACPI_GPE_CONDITIONAL_ENABLE     2


#define ACPI_GPE_DISPATCH_NONE          (u8) 0x00
#define ACPI_GPE_DISPATCH_METHOD        (u8) 0x01
#define ACPI_GPE_DISPATCH_HANDLER       (u8) 0x02
#define ACPI_GPE_DISPATCH_NOTIFY        (u8) 0x03
#define ACPI_GPE_DISPATCH_RAW_HANDLER   (u8) 0x04
#define ACPI_GPE_DISPATCH_MASK          (u8) 0x07
#define ACPI_GPE_DISPATCH_TYPE(flags)   ((u8) ((flags) & ACPI_GPE_DISPATCH_MASK))

#define ACPI_GPE_LEVEL_TRIGGERED        (u8) 0x08
#define ACPI_GPE_EDGE_TRIGGERED         (u8) 0x00
#define ACPI_GPE_XRUPT_TYPE_MASK        (u8) 0x08

#define ACPI_GPE_CAN_WAKE               (u8) 0x10


#define ACPI_NOT_ISR                    0x1
#define ACPI_ISR                        0x0



#define ACPI_SYSTEM_NOTIFY              0x1
#define ACPI_DEVICE_NOTIFY              0x2
#define ACPI_ALL_NOTIFY                 (ACPI_SYSTEM_NOTIFY | ACPI_DEVICE_NOTIFY)
#define ACPI_MAX_NOTIFY_HANDLER_TYPE    0x3
#define ACPI_NUM_NOTIFY_TYPES           2

#define ACPI_MAX_SYS_NOTIFY             0x7F
#define ACPI_MAX_DEVICE_SPECIFIC_NOTIFY 0xBF

#define ACPI_SYSTEM_HANDLER_LIST        0	
#define ACPI_DEVICE_HANDLER_LIST        1	



typedef u8 acpi_adr_space_type;

#define ACPI_ADR_SPACE_SYSTEM_MEMORY    (acpi_adr_space_type) 0
#define ACPI_ADR_SPACE_SYSTEM_IO        (acpi_adr_space_type) 1
#define ACPI_ADR_SPACE_PCI_CONFIG       (acpi_adr_space_type) 2
#define ACPI_ADR_SPACE_EC               (acpi_adr_space_type) 3
#define ACPI_ADR_SPACE_SMBUS            (acpi_adr_space_type) 4
#define ACPI_ADR_SPACE_CMOS             (acpi_adr_space_type) 5
#define ACPI_ADR_SPACE_PCI_BAR_TARGET   (acpi_adr_space_type) 6
#define ACPI_ADR_SPACE_IPMI             (acpi_adr_space_type) 7
#define ACPI_ADR_SPACE_GPIO             (acpi_adr_space_type) 8
#define ACPI_ADR_SPACE_GSBUS            (acpi_adr_space_type) 9
#define ACPI_ADR_SPACE_PLATFORM_COMM    (acpi_adr_space_type) 10

#define ACPI_NUM_PREDEFINED_REGIONS     11


#define ACPI_ADR_SPACE_DATA_TABLE       (acpi_adr_space_type) 0x7E	
#define ACPI_ADR_SPACE_FIXED_HARDWARE   (acpi_adr_space_type) 0x7F



#define ACPI_REG_DISCONNECT             0
#define ACPI_REG_CONNECT                1





#define ACPI_BITREG_TIMER_STATUS                0x00
#define ACPI_BITREG_BUS_MASTER_STATUS           0x01
#define ACPI_BITREG_GLOBAL_LOCK_STATUS          0x02
#define ACPI_BITREG_POWER_BUTTON_STATUS         0x03
#define ACPI_BITREG_SLEEP_BUTTON_STATUS         0x04
#define ACPI_BITREG_RT_CLOCK_STATUS             0x05
#define ACPI_BITREG_WAKE_STATUS                 0x06
#define ACPI_BITREG_PCIEXP_WAKE_STATUS          0x07



#define ACPI_BITREG_TIMER_ENABLE                0x08
#define ACPI_BITREG_GLOBAL_LOCK_ENABLE          0x09
#define ACPI_BITREG_POWER_BUTTON_ENABLE         0x0A
#define ACPI_BITREG_SLEEP_BUTTON_ENABLE         0x0B
#define ACPI_BITREG_RT_CLOCK_ENABLE             0x0C
#define ACPI_BITREG_PCIEXP_WAKE_DISABLE         0x0D



#define ACPI_BITREG_SCI_ENABLE                  0x0E
#define ACPI_BITREG_BUS_MASTER_RLD              0x0F
#define ACPI_BITREG_GLOBAL_LOCK_RELEASE         0x10
#define ACPI_BITREG_SLEEP_TYPE                  0x11
#define ACPI_BITREG_SLEEP_ENABLE                0x12



#define ACPI_BITREG_ARB_DISABLE                 0x13

#define ACPI_BITREG_MAX                         0x13
#define ACPI_NUM_BITREG                         ACPI_BITREG_MAX + 1



#define ACPI_CLEAR_STATUS                       1



#define ACPI_ENABLE_EVENT                       1
#define ACPI_DISABLE_EVENT                      0



typedef acpi_status (*acpi_sleep_function) (u8 sleep_state);

struct acpi_sleep_functions {
	acpi_sleep_function legacy_function;
	acpi_sleep_function extended_function;
};




union acpi_object {
	acpi_object_type type;	
	struct {
		acpi_object_type type;	
		u64 value;	
	} integer;

	struct {
		acpi_object_type type;	
		u32 length;	
		char *pointer;	
	} string;

	struct {
		acpi_object_type type;	
		u32 length;	
		u8 *pointer;	
	} buffer;

	struct {
		acpi_object_type type;	
		u32 count;	
		union acpi_object *elements;	
	} package;

	struct {
		acpi_object_type type;	
		acpi_object_type actual_type;	
		acpi_handle handle;	
	} reference;

	struct {
		acpi_object_type type;	
		u32 proc_id;
		acpi_io_address pblk_address;
		u32 pblk_length;
	} processor;

	struct {
		acpi_object_type type;	
		u32 system_level;
		u32 resource_order;
	} power_resource;
};


struct acpi_object_list {
	u32 count;
	union acpi_object *pointer;
};


#define ACPI_NO_BUFFER              0

#ifdef ACPI_NO_MEM_ALLOCATIONS

#define ACPI_ALLOCATE_BUFFER        (acpi_size) (0)
#define ACPI_ALLOCATE_LOCAL_BUFFER  (acpi_size) (0)

#else				

#define ACPI_ALLOCATE_BUFFER        (acpi_size) (-1)	
#define ACPI_ALLOCATE_LOCAL_BUFFER  (acpi_size) (-2)	

#endif				

struct acpi_buffer {
	acpi_size length;	
	void *pointer;		
};


#define ACPI_FULL_PATHNAME              0
#define ACPI_SINGLE_NAME                1
#define ACPI_FULL_PATHNAME_NO_TRAILING  2
#define ACPI_NAME_TYPE_MAX              2


struct acpi_predefined_names {
	const char *name;
	u8 type;
	char *val;
};


#define ACPI_SYS_MODE_UNKNOWN           0x0000
#define ACPI_SYS_MODE_ACPI              0x0001
#define ACPI_SYS_MODE_LEGACY            0x0002
#define ACPI_SYS_MODES_MASK             0x0003


struct acpi_system_info {
	u32 acpi_ca_version;
	u32 flags;
	u32 timer_resolution;
	u32 reserved1;
	u32 reserved2;
	u32 debug_level;
	u32 debug_layer;
};


struct acpi_statistics {
	u32 sci_count;
	u32 gpe_count;
	u32 fixed_event_count[ACPI_NUM_FIXED_EVENTS];
	u32 method_count;
};


typedef u32
 (ACPI_SYSTEM_XFACE * acpi_osd_handler) (void *context);

typedef void
 (ACPI_SYSTEM_XFACE * acpi_osd_exec_callback) (void *context);


typedef
u32 (*acpi_sci_handler) (void *context);

typedef
void (*acpi_gbl_event_handler) (u32 event_type,
			       acpi_handle device,
			       u32 event_number, void *context);

#define ACPI_EVENT_TYPE_GPE         0
#define ACPI_EVENT_TYPE_FIXED       1

typedef
u32(*acpi_event_handler) (void *context);

typedef
u32 (*acpi_gpe_handler) (acpi_handle gpe_device, u32 gpe_number, void *context);

typedef
void (*acpi_notify_handler) (acpi_handle device, u32 value, void *context);

typedef
void (*acpi_object_handler) (acpi_handle object, void *data);

typedef
acpi_status (*acpi_init_handler) (acpi_handle object, u32 function);

#define ACPI_INIT_DEVICE_INI        1

typedef
acpi_status (*acpi_exception_handler) (acpi_status aml_status,
				       acpi_name name,
				       u16 opcode,
				       u32 aml_offset, void *context);



typedef
acpi_status (*acpi_table_handler) (u32 event, void *table, void *context);



#define ACPI_TABLE_EVENT_LOAD           0x0
#define ACPI_TABLE_EVENT_UNLOAD         0x1
#define ACPI_TABLE_EVENT_INSTALL        0x2
#define ACPI_TABLE_EVENT_UNINSTALL      0x3
#define ACPI_NUM_TABLE_EVENTS           4



typedef
acpi_status (*acpi_adr_space_handler) (u32 function,
				       acpi_physical_address address,
				       u32 bit_width,
				       u64 *value,
				       void *handler_context,
				       void *region_context);

#define ACPI_DEFAULT_HANDLER            NULL



struct acpi_connection_info {
	u8 *connection;
	u16 length;
	u8 access_length;
};

typedef
acpi_status (*acpi_adr_space_setup) (acpi_handle region_handle,
				     u32 function,
				     void *handler_context,
				     void **region_context);

#define ACPI_REGION_ACTIVATE    0
#define ACPI_REGION_DEACTIVATE  1

typedef
acpi_status (*acpi_walk_callback) (acpi_handle object,
				   u32 nesting_level,
				   void *context, void **return_value);

typedef
u32 (*acpi_interface_handler) (acpi_string interface_name, u32 supported);



#define ACPI_INTERRUPT_NOT_HANDLED      0x00
#define ACPI_INTERRUPT_HANDLED          0x01



#define ACPI_REENABLE_GPE               0x80



#define ACPI_EISAID_STRING_SIZE         8	



#define ACPI_UUID_LENGTH                16



#define ACPI_PCICLS_STRING_SIZE         7	



struct acpi_pnp_device_id {
	u32 length;		
	char *string;
};

struct acpi_pnp_device_id_list {
	u32 count;		
	u32 list_size;		
	struct acpi_pnp_device_id ids[1];	
};


struct acpi_device_info {
	u32 info_size;		
	u32 name;		
	acpi_object_type type;	
	u8 param_count;		
	u16 valid;		
	u8 flags;		
	u8 highest_dstates[4];	
	u8 lowest_dstates[5];	
	u32 current_status;	
	u64 address;	
	struct acpi_pnp_device_id hardware_id;	
	struct acpi_pnp_device_id unique_id;	
	struct acpi_pnp_device_id class_code;	
	struct acpi_pnp_device_id_list compatible_id_list;	
};



#define ACPI_PCI_ROOT_BRIDGE            0x01



#define ACPI_VALID_STA                  0x0001
#define ACPI_VALID_ADR                  0x0002
#define ACPI_VALID_HID                  0x0004
#define ACPI_VALID_UID                  0x0008
#define ACPI_VALID_CID                  0x0020
#define ACPI_VALID_CLS                  0x0040
#define ACPI_VALID_SXDS                 0x0100
#define ACPI_VALID_SXWS                 0x0200



#define ACPI_STA_DEVICE_PRESENT         0x01
#define ACPI_STA_DEVICE_ENABLED         0x02
#define ACPI_STA_DEVICE_UI              0x04
#define ACPI_STA_DEVICE_FUNCTIONING     0x08
#define ACPI_STA_DEVICE_OK              0x08	
#define ACPI_STA_BATTERY_PRESENT        0x10



struct acpi_pci_id {
	u16 segment;
	u16 bus;
	u16 device;
	u16 function;
};

struct acpi_mem_space_context {
	u32 length;
	acpi_physical_address address;
	acpi_physical_address mapped_physical_address;
	u8 *mapped_logical_address;
	acpi_size mapped_length;
};


struct acpi_memory_list {
	const char *list_name;
	void *list_head;
	u16 object_size;
	u16 max_depth;
	u16 current_depth;

#ifdef ACPI_DBG_TRACK_ALLOCATIONS

	

	u32 total_allocated;
	u32 total_freed;
	u32 max_occupied;
	u32 total_size;
	u32 current_total_size;
	u32 requests;
	u32 hits;
#endif
};



typedef enum {
	ACPI_TRACE_AML_METHOD,
	ACPI_TRACE_AML_OPCODE,
	ACPI_TRACE_AML_REGION
} acpi_trace_event_type;



#define ACPI_VENDOR_STRINGS                 0x01
#define ACPI_FEATURE_STRINGS                0x02
#define ACPI_ENABLE_INTERFACES              0x00
#define ACPI_DISABLE_INTERFACES             0x04

#define ACPI_DISABLE_ALL_VENDOR_STRINGS     (ACPI_DISABLE_INTERFACES | ACPI_VENDOR_STRINGS)
#define ACPI_DISABLE_ALL_FEATURE_STRINGS    (ACPI_DISABLE_INTERFACES | ACPI_FEATURE_STRINGS)
#define ACPI_DISABLE_ALL_STRINGS            (ACPI_DISABLE_INTERFACES | ACPI_VENDOR_STRINGS | ACPI_FEATURE_STRINGS)
#define ACPI_ENABLE_ALL_VENDOR_STRINGS      (ACPI_ENABLE_INTERFACES | ACPI_VENDOR_STRINGS)
#define ACPI_ENABLE_ALL_FEATURE_STRINGS     (ACPI_ENABLE_INTERFACES | ACPI_FEATURE_STRINGS)
#define ACPI_ENABLE_ALL_STRINGS             (ACPI_ENABLE_INTERFACES | ACPI_VENDOR_STRINGS | ACPI_FEATURE_STRINGS)

#define ACPI_OSI_WIN_2000               0x01
#define ACPI_OSI_WIN_XP                 0x02
#define ACPI_OSI_WIN_XP_SP1             0x03
#define ACPI_OSI_WINSRV_2003            0x04
#define ACPI_OSI_WIN_XP_SP2             0x05
#define ACPI_OSI_WINSRV_2003_SP1        0x06
#define ACPI_OSI_WIN_VISTA              0x07
#define ACPI_OSI_WINSRV_2008            0x08
#define ACPI_OSI_WIN_VISTA_SP1          0x09
#define ACPI_OSI_WIN_VISTA_SP2          0x0A
#define ACPI_OSI_WIN_7                  0x0B
#define ACPI_OSI_WIN_8                  0x0C
#define ACPI_OSI_WIN_10                 0x0D



#define ACPI_OPT_END                    -1

#endif				
