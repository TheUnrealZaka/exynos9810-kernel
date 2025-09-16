

#ifndef __HID_H
#define __HID_H


#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mod_devicetable.h> 
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#include <linux/semaphore.h>
#include <linux/power_supply.h>
#include <uapi/linux/hid.h>



struct hid_item {
	unsigned  format;
	__u8      size;
	__u8      type;
	__u8      tag;
	union {
	    __u8   u8;
	    __s8   s8;
	    __u16  u16;
	    __s16  s16;
	    __u32  u32;
	    __s32  s32;
	    __u8  *longdata;
	} data;
};



#define HID_ITEM_FORMAT_SHORT	0
#define HID_ITEM_FORMAT_LONG	1



#define HID_ITEM_TAG_LONG	15



#define HID_ITEM_TYPE_MAIN		0
#define HID_ITEM_TYPE_GLOBAL		1
#define HID_ITEM_TYPE_LOCAL		2
#define HID_ITEM_TYPE_RESERVED		3



#define HID_MAIN_ITEM_TAG_INPUT			8
#define HID_MAIN_ITEM_TAG_OUTPUT		9
#define HID_MAIN_ITEM_TAG_FEATURE		11
#define HID_MAIN_ITEM_TAG_BEGIN_COLLECTION	10
#define HID_MAIN_ITEM_TAG_END_COLLECTION	12



#define HID_MAIN_ITEM_CONSTANT		0x001
#define HID_MAIN_ITEM_VARIABLE		0x002
#define HID_MAIN_ITEM_RELATIVE		0x004
#define HID_MAIN_ITEM_WRAP		0x008
#define HID_MAIN_ITEM_NONLINEAR		0x010
#define HID_MAIN_ITEM_NO_PREFERRED	0x020
#define HID_MAIN_ITEM_NULL_STATE	0x040
#define HID_MAIN_ITEM_VOLATILE		0x080
#define HID_MAIN_ITEM_BUFFERED_BYTE	0x100



#define HID_COLLECTION_PHYSICAL		0
#define HID_COLLECTION_APPLICATION	1
#define HID_COLLECTION_LOGICAL		2



#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE		0
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM	1
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM	2
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM	3
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM	4
#define HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT	5
#define HID_GLOBAL_ITEM_TAG_UNIT		6
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE		7
#define HID_GLOBAL_ITEM_TAG_REPORT_ID		8
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT	9
#define HID_GLOBAL_ITEM_TAG_PUSH		10
#define HID_GLOBAL_ITEM_TAG_POP			11



#define HID_LOCAL_ITEM_TAG_USAGE		0
#define HID_LOCAL_ITEM_TAG_USAGE_MINIMUM	1
#define HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM	2
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_INDEX	3
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MINIMUM	4
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MAXIMUM	5
#define HID_LOCAL_ITEM_TAG_STRING_INDEX		7
#define HID_LOCAL_ITEM_TAG_STRING_MINIMUM	8
#define HID_LOCAL_ITEM_TAG_STRING_MAXIMUM	9
#define HID_LOCAL_ITEM_TAG_DELIMITER		10



#define HID_USAGE_PAGE		0xffff0000

#define HID_UP_UNDEFINED	0x00000000
#define HID_UP_GENDESK		0x00010000
#define HID_UP_SIMULATION	0x00020000
#define HID_UP_GENDEVCTRLS	0x00060000
#define HID_UP_KEYBOARD		0x00070000
#define HID_UP_LED		0x00080000
#define HID_UP_BUTTON		0x00090000
#define HID_UP_ORDINAL		0x000a0000
#define HID_UP_TELEPHONY	0x000b0000
#define HID_UP_CONSUMER		0x000c0000
#define HID_UP_DIGITIZER	0x000d0000
#define HID_UP_PID		0x000f0000
#define HID_UP_HPVENDOR         0xff7f0000
#define HID_UP_HPVENDOR2        0xff010000
#define HID_UP_MSVENDOR		0xff000000
#define HID_UP_CUSTOM		0x00ff0000
#define HID_UP_LOGIVENDOR	0xffbc0000
#define HID_UP_LOGIVENDOR2   0xff090000
#define HID_UP_LOGIVENDOR3   0xff430000
#define HID_UP_LNVENDOR		0xffa00000
#define HID_UP_SENSOR		0x00200000

#define HID_USAGE		0x0000ffff

#define HID_GD_POINTER		0x00010001
#define HID_GD_MOUSE		0x00010002
#define HID_GD_JOYSTICK		0x00010004
#define HID_GD_GAMEPAD		0x00010005
#define HID_GD_KEYBOARD		0x00010006
#define HID_GD_KEYPAD		0x00010007
#define HID_GD_MULTIAXIS	0x00010008
#define HID_GD_X		0x00010030
#define HID_GD_Y		0x00010031
#define HID_GD_Z		0x00010032
#define HID_GD_RX		0x00010033
#define HID_GD_RY		0x00010034
#define HID_GD_RZ		0x00010035
#define HID_GD_SLIDER		0x00010036
#define HID_GD_DIAL		0x00010037
#define HID_GD_WHEEL		0x00010038
#define HID_GD_HATSWITCH	0x00010039
#define HID_GD_BUFFER		0x0001003a
#define HID_GD_BYTECOUNT	0x0001003b
#define HID_GD_MOTION		0x0001003c
#define HID_GD_START		0x0001003d
#define HID_GD_SELECT		0x0001003e
#define HID_GD_VX		0x00010040
#define HID_GD_VY		0x00010041
#define HID_GD_VZ		0x00010042
#define HID_GD_VBRX		0x00010043
#define HID_GD_VBRY		0x00010044
#define HID_GD_VBRZ		0x00010045
#define HID_GD_VNO		0x00010046
#define HID_GD_FEATURE		0x00010047
#define HID_GD_SYSTEM_CONTROL	0x00010080
#define HID_GD_UP		0x00010090
#define HID_GD_DOWN		0x00010091
#define HID_GD_RIGHT		0x00010092
#define HID_GD_LEFT		0x00010093

#define HID_DC_BATTERYSTRENGTH	0x00060020

#define HID_CP_CONSUMER_CONTROL	0x000c0001

#define HID_DG_DIGITIZER	0x000d0001
#define HID_DG_PEN		0x000d0002
#define HID_DG_LIGHTPEN		0x000d0003
#define HID_DG_TOUCHSCREEN	0x000d0004
#define HID_DG_TOUCHPAD		0x000d0005
#define HID_DG_STYLUS		0x000d0020
#define HID_DG_PUCK		0x000d0021
#define HID_DG_FINGER		0x000d0022
#define HID_DG_TIPPRESSURE	0x000d0030
#define HID_DG_BARRELPRESSURE	0x000d0031
#define HID_DG_INRANGE		0x000d0032
#define HID_DG_TOUCH		0x000d0033
#define HID_DG_UNTOUCH		0x000d0034
#define HID_DG_TAP		0x000d0035
#define HID_DG_TABLETFUNCTIONKEY	0x000d0039
#define HID_DG_PROGRAMCHANGEKEY	0x000d003a
#define HID_DG_INVERT		0x000d003c
#define HID_DG_TIPSWITCH	0x000d0042
#define HID_DG_TIPSWITCH2	0x000d0043
#define HID_DG_BARRELSWITCH	0x000d0044
#define HID_DG_ERASER		0x000d0045
#define HID_DG_TABLETPICK	0x000d0046

#define HID_CP_CONSUMERCONTROL	0x000c0001
#define HID_CP_NUMERICKEYPAD	0x000c0002
#define HID_CP_PROGRAMMABLEBUTTONS	0x000c0003
#define HID_CP_MICROPHONE	0x000c0004
#define HID_CP_HEADPHONE	0x000c0005
#define HID_CP_GRAPHICEQUALIZER	0x000c0006
#define HID_CP_FUNCTIONBUTTONS	0x000c0036
#define HID_CP_SELECTION	0x000c0080
#define HID_CP_MEDIASELECTION	0x000c0087
#define HID_CP_SELECTDISC	0x000c00ba
#define HID_CP_VOLUMEUP		0x000c00e9
#define HID_CP_VOLUMEDOWN	0x000c00ea
#define HID_CP_PLAYBACKSPEED	0x000c00f1
#define HID_CP_PROXIMITY	0x000c0109
#define HID_CP_SPEAKERSYSTEM	0x000c0160
#define HID_CP_CHANNELLEFT	0x000c0161
#define HID_CP_CHANNELRIGHT	0x000c0162
#define HID_CP_CHANNELCENTER	0x000c0163
#define HID_CP_CHANNELFRONT	0x000c0164
#define HID_CP_CHANNELCENTERFRONT	0x000c0165
#define HID_CP_CHANNELSIDE	0x000c0166
#define HID_CP_CHANNELSURROUND	0x000c0167
#define HID_CP_CHANNELLOWFREQUENCYENHANCEMENT	0x000c0168
#define HID_CP_CHANNELTOP	0x000c0169
#define HID_CP_CHANNELUNKNOWN	0x000c016a
#define HID_CP_APPLICATIONLAUNCHBUTTONS	0x000c0180
#define HID_CP_GENERICGUIAPPLICATIONCONTROLS	0x000c0200

#define HID_DG_CONFIDENCE	0x000d0047
#define HID_DG_WIDTH		0x000d0048
#define HID_DG_HEIGHT		0x000d0049
#define HID_DG_CONTACTID	0x000d0051
#define HID_DG_INPUTMODE	0x000d0052
#define HID_DG_DEVICEINDEX	0x000d0053
#define HID_DG_CONTACTCOUNT	0x000d0054
#define HID_DG_CONTACTMAX	0x000d0055
#define HID_DG_BUTTONTYPE	0x000d0059
#define HID_DG_BARRELSWITCH2	0x000d005a
#define HID_DG_TOOLSERIALNUMBER	0x000d005b



#define HID_INPUT_REPORT	0
#define HID_OUTPUT_REPORT	1
#define HID_FEATURE_REPORT	2

#define HID_REPORT_TYPES	3



#define HID_CONNECT_HIDINPUT		0x01
#define HID_CONNECT_HIDINPUT_FORCE	0x02
#define HID_CONNECT_HIDRAW		0x04
#define HID_CONNECT_HIDDEV		0x08
#define HID_CONNECT_HIDDEV_FORCE	0x10
#define HID_CONNECT_FF			0x20
#define HID_CONNECT_DRIVER		0x40
#define HID_CONNECT_DEFAULT	(HID_CONNECT_HIDINPUT|HID_CONNECT_HIDRAW| \
		HID_CONNECT_HIDDEV|HID_CONNECT_FF)




#define MAX_USBHID_BOOT_QUIRKS 4

#define HID_QUIRK_INVERT			0x00000001
#define HID_QUIRK_NOTOUCH			0x00000002
#define HID_QUIRK_IGNORE			0x00000004
#define HID_QUIRK_NOGET				0x00000008
#define HID_QUIRK_HIDDEV_FORCE			0x00000010
#define HID_QUIRK_BADPAD			0x00000020
#define HID_QUIRK_MULTI_INPUT			0x00000040
#define HID_QUIRK_HIDINPUT_FORCE		0x00000080
#define HID_QUIRK_NO_EMPTY_INPUT		0x00000100
#define HID_QUIRK_NO_INIT_INPUT_REPORTS		0x00000200
#define HID_QUIRK_ALWAYS_POLL			0x00000400
#define HID_QUIRK_SKIP_OUTPUT_REPORTS		0x00010000
#define HID_QUIRK_SKIP_OUTPUT_REPORT_ID		0x00020000
#define HID_QUIRK_NO_OUTPUT_REPORTS_ON_INTR_EP	0x00040000
#define HID_QUIRK_FULLSPEED_INTERVAL		0x10000000
#define HID_QUIRK_NO_INIT_REPORTS		0x20000000
#define HID_QUIRK_NO_IGNORE			0x40000000
#define HID_QUIRK_NO_INPUT_SYNC			0x80000000


#define HID_GROUP_GENERIC			0x0001
#define HID_GROUP_MULTITOUCH			0x0002
#define HID_GROUP_SENSOR_HUB			0x0003
#define HID_GROUP_MULTITOUCH_WIN_8		0x0004


#define HID_GROUP_RMI				0x0100
#define HID_GROUP_WACOM				0x0101
#define HID_GROUP_LOGITECH_DJ_DEVICE		0x0102
#define HID_GROUP_STEAM				0x0103



struct hid_global {
	unsigned usage_page;
	__s32    logical_minimum;
	__s32    logical_maximum;
	__s32    physical_minimum;
	__s32    physical_maximum;
	__s32    unit_exponent;
	unsigned unit;
	unsigned report_id;
	unsigned report_size;
	unsigned report_count;
};



#define HID_MAX_USAGES			12288
#define HID_DEFAULT_NUM_COLLECTIONS	16

struct hid_local {
	unsigned usage[HID_MAX_USAGES]; 
	u8 usage_size[HID_MAX_USAGES]; 
	unsigned collection_index[HID_MAX_USAGES]; 
	unsigned usage_index;
	unsigned usage_minimum;
	unsigned delimiter_depth;
	unsigned delimiter_branch;
};



struct hid_collection {
	unsigned type;
	unsigned usage;
	unsigned level;
};

struct hid_usage {
	unsigned  hid;			
	unsigned  collection_index;	
	unsigned  usage_index;		
	
	__u16     code;			
	__u8      type;			
	__s8	  hat_min;		
	__s8	  hat_max;		
	__s8	  hat_dir;		
};

struct hid_input;

struct hid_field {
	unsigned  physical;		
	unsigned  logical;		
	unsigned  application;		
	struct hid_usage *usage;	
	unsigned  maxusage;		
	unsigned  flags;		
	unsigned  report_offset;	
	unsigned  report_size;		
	unsigned  report_count;		
	unsigned  report_type;		
	__s32    *value;		
	__s32     logical_minimum;
	__s32     logical_maximum;
	__s32     physical_minimum;
	__s32     physical_maximum;
	__s32     unit_exponent;
	unsigned  unit;
	struct hid_report *report;	
	unsigned index;			
	
	struct hid_input *hidinput;	
	__u16 dpad;			
};

#define HID_MAX_FIELDS 256

struct hid_report {
	struct list_head list;
	unsigned id;					
	unsigned type;					
	struct hid_field *field[HID_MAX_FIELDS];	
	unsigned maxfield;				
	unsigned size;					
	struct hid_device *device;			
};

#define HID_MAX_IDS 256

struct hid_report_enum {
	unsigned numbered;
	struct list_head report_list;
	struct hid_report *report_id_hash[HID_MAX_IDS];
};

#define HID_MIN_BUFFER_SIZE	64		
#define HID_MAX_BUFFER_SIZE	8192		
#define HID_CONTROL_FIFO_SIZE	256		
#define HID_OUTPUT_FIFO_SIZE	64

struct hid_control_fifo {
	unsigned char dir;
	struct hid_report *report;
	char *raw_report;
};

struct hid_output_fifo {
	struct hid_report *report;
	char *raw_report;
};

#define HID_CLAIMED_INPUT	1
#define HID_CLAIMED_HIDDEV	2
#define HID_CLAIMED_HIDRAW	4
#define HID_CLAIMED_DRIVER	8

#define HID_STAT_ADDED		1
#define HID_STAT_PARSED		2

struct hid_input {
	struct list_head list;
	struct hid_report *report;
	struct input_dev *input;
};

enum hid_type {
	HID_TYPE_OTHER = 0,
	HID_TYPE_USBMOUSE,
	HID_TYPE_USBNONE
};

struct hid_driver;
struct hid_ll_driver;

struct hid_device {							
	__u8 *dev_rdesc;
	unsigned dev_rsize;
	__u8 *rdesc;
	unsigned rsize;
	struct hid_collection *collection;				
	unsigned collection_size;					
	unsigned maxcollection;						
	unsigned maxapplication;					
	__u16 bus;							
	__u16 group;							
	__u32 vendor;							
	__u32 product;							
	__u32 version;							
	enum hid_type type;						
	unsigned country;						
	struct hid_report_enum report_enum[HID_REPORT_TYPES];
	struct work_struct led_work;					

	struct semaphore driver_lock;					
	struct semaphore driver_input_lock;				
	struct device dev;						
	struct hid_driver *driver;
	struct hid_ll_driver *ll_driver;

#ifdef CONFIG_HID_BATTERY_STRENGTH
	
	struct power_supply *battery;
	__s32 battery_capacity;
	__s32 battery_min;
	__s32 battery_max;
	__s32 battery_report_type;
	__s32 battery_report_id;
	bool battery_reported;
#endif

	unsigned int status;						
	unsigned claimed;						
	unsigned quirks;						
	bool io_started;						

	struct list_head inputs;					
	void *hiddev;							
	void *hidraw;
	void *hidovr;
	void *hidtvr;
	int minor;							

	int open;							
	char name[128];							
	char phys[64];							
	char uniq[64];							

	void *driver_data;

	
	int (*ff_init)(struct hid_device *);

	
	int (*hiddev_connect)(struct hid_device *, unsigned int);
	void (*hiddev_disconnect)(struct hid_device *);
	void (*hiddev_hid_event) (struct hid_device *, struct hid_field *field,
				  struct hid_usage *, __s32);
	void (*hiddev_report_event) (struct hid_device *, struct hid_report *);

	
	unsigned short debug;
	struct dentry *debug_dir;
	struct dentry *debug_rdesc;
	struct dentry *debug_events;
	struct list_head debug_list;
	spinlock_t  debug_list_lock;
	wait_queue_head_t debug_wait;
};

#define to_hid_device(pdev) \
	container_of(pdev, struct hid_device, dev)

static inline void *hid_get_drvdata(struct hid_device *hdev)
{
	return dev_get_drvdata(&hdev->dev);
}

static inline void hid_set_drvdata(struct hid_device *hdev, void *data)
{
	dev_set_drvdata(&hdev->dev, data);
}

#define HID_GLOBAL_STACK_SIZE 4
#define HID_COLLECTION_STACK_SIZE 4

#define HID_SCAN_FLAG_MT_WIN_8			BIT(0)
#define HID_SCAN_FLAG_VENDOR_SPECIFIC		BIT(1)
#define HID_SCAN_FLAG_GD_POINTER		BIT(2)

struct hid_parser {
	struct hid_global     global;
	struct hid_global     global_stack[HID_GLOBAL_STACK_SIZE];
	unsigned              global_stack_ptr;
	struct hid_local      local;
	unsigned              collection_stack[HID_COLLECTION_STACK_SIZE];
	unsigned              collection_stack_ptr;
	struct hid_device    *device;
	unsigned              scan_flags;
};

struct hid_class_descriptor {
	__u8  bDescriptorType;
	__le16 wDescriptorLength;
} __attribute__ ((packed));

struct hid_descriptor {
	__u8  bLength;
	__u8  bDescriptorType;
	__le16 bcdHID;
	__u8  bCountryCode;
	__u8  bNumDescriptors;

	struct hid_class_descriptor desc[1];
} __attribute__ ((packed));

#define HID_DEVICE(b, g, ven, prod)					\
	.bus = (b), .group = (g), .vendor = (ven), .product = (prod)
#define HID_USB_DEVICE(ven, prod)				\
	.bus = BUS_USB, .vendor = (ven), .product = (prod)
#define HID_BLUETOOTH_DEVICE(ven, prod)					\
	.bus = BUS_BLUETOOTH, .vendor = (ven), .product = (prod)
#define HID_I2C_DEVICE(ven, prod)				\
	.bus = BUS_I2C, .vendor = (ven), .product = (prod)

#define HID_REPORT_ID(rep) \
	.report_type = (rep)
#define HID_USAGE_ID(uhid, utype, ucode) \
	.usage_hid = (uhid), .usage_type = (utype), .usage_code = (ucode)

#define HID_TERMINATOR		(HID_ANY_ID - 1)

struct hid_report_id {
	__u32 report_type;
};
struct hid_usage_id {
	__u32 usage_hid;
	__u32 usage_type;
	__u32 usage_code;
};


struct hid_driver {
	char *name;
	const struct hid_device_id *id_table;

	struct list_head dyn_list;
	spinlock_t dyn_lock;

	int (*probe)(struct hid_device *dev, const struct hid_device_id *id);
	void (*remove)(struct hid_device *dev);

	const struct hid_report_id *report_table;
	int (*raw_event)(struct hid_device *hdev, struct hid_report *report,
			u8 *data, int size);
	const struct hid_usage_id *usage_table;
	int (*event)(struct hid_device *hdev, struct hid_field *field,
			struct hid_usage *usage, __s32 value);
	void (*report)(struct hid_device *hdev, struct hid_report *report);

	__u8 *(*report_fixup)(struct hid_device *hdev, __u8 *buf,
			unsigned int *size);

	int (*input_mapping)(struct hid_device *hdev,
			struct hid_input *hidinput, struct hid_field *field,
			struct hid_usage *usage, unsigned long **bit, int *max);
	int (*input_mapped)(struct hid_device *hdev,
			struct hid_input *hidinput, struct hid_field *field,
			struct hid_usage *usage, unsigned long **bit, int *max);
	int (*input_configured)(struct hid_device *hdev,
				struct hid_input *hidinput);
	void (*feature_mapping)(struct hid_device *hdev,
			struct hid_field *field,
			struct hid_usage *usage);
#ifdef CONFIG_PM
	int (*suspend)(struct hid_device *hdev, pm_message_t message);
	int (*resume)(struct hid_device *hdev);
	int (*reset_resume)(struct hid_device *hdev);
#endif

	struct device_driver driver;
};

#define to_hid_driver(pdrv) \
	container_of(pdrv, struct hid_driver, driver)


struct hid_ll_driver {
	int (*start)(struct hid_device *hdev);
	void (*stop)(struct hid_device *hdev);

	int (*open)(struct hid_device *hdev);
	void (*close)(struct hid_device *hdev);

	int (*power)(struct hid_device *hdev, int level);

	int (*parse)(struct hid_device *hdev);

	void (*request)(struct hid_device *hdev,
			struct hid_report *report, int reqtype);

	int (*wait)(struct hid_device *hdev);

	int (*raw_request) (struct hid_device *hdev, unsigned char reportnum,
			    __u8 *buf, size_t len, unsigned char rtype,
			    int reqtype);

	int (*output_report) (struct hid_device *hdev, __u8 *buf, size_t len);

	int (*idle)(struct hid_device *hdev, int report, int idle, int reqtype);
};

extern struct hid_ll_driver i2c_hid_ll_driver;
extern struct hid_ll_driver hidp_hid_driver;
extern struct hid_ll_driver uhid_hid_driver;
extern struct hid_ll_driver usb_hid_driver;

static inline bool hid_is_using_ll_driver(struct hid_device *hdev,
		struct hid_ll_driver *driver)
{
	return hdev->ll_driver == driver;
}

static inline bool hid_is_usb(struct hid_device *hdev)
{
	return hid_is_using_ll_driver(hdev, &usb_hid_driver);
}

#define	PM_HINT_FULLON	1<<5
#define PM_HINT_NORMAL	1<<1



#define IS_INPUT_APPLICATION(a) (((a >= 0x00010000) && (a <= 0x00010008)) || (a == 0x00010080) || (a == 0x000c0001) || ((a >= 0x000d0002) && (a <= 0x000d0006)))



extern int hid_debug;

extern bool hid_ignore(struct hid_device *);
extern int hid_add_device(struct hid_device *);
extern void hid_destroy_device(struct hid_device *);

extern int __must_check __hid_register_driver(struct hid_driver *,
		struct module *, const char *mod_name);


#define hid_register_driver(driver) \
	__hid_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)

extern void hid_unregister_driver(struct hid_driver *);


#define module_hid_driver(__hid_driver) \
	module_driver(__hid_driver, hid_register_driver, \
		      hid_unregister_driver)

extern void hidinput_hid_event(struct hid_device *, struct hid_field *, struct hid_usage *, __s32);
extern void hidinput_report_event(struct hid_device *hid, struct hid_report *report);
extern int hidinput_connect(struct hid_device *hid, unsigned int force);
extern void hidinput_disconnect(struct hid_device *);

int hid_set_field(struct hid_field *, unsigned, __s32);
int hid_input_report(struct hid_device *, int type, u8 *, u32, int);
int hidinput_find_field(struct hid_device *hid, unsigned int type, unsigned int code, struct hid_field **field);
struct hid_field *hidinput_get_led_field(struct hid_device *hid);
unsigned int hidinput_count_leds(struct hid_device *hid);
__s32 hidinput_calc_abs_res(const struct hid_field *field, __u16 code);
void hid_output_report(struct hid_report *report, __u8 *data);
void __hid_request(struct hid_device *hid, struct hid_report *rep, int reqtype);
u8 *hid_alloc_report_buf(struct hid_report *report, gfp_t flags);
struct hid_device *hid_allocate_device(void);
struct hid_report *hid_register_report(struct hid_device *device, unsigned type, unsigned id);
int hid_parse_report(struct hid_device *hid, __u8 *start, unsigned size);
struct hid_report *hid_validate_values(struct hid_device *hid,
				       unsigned int type, unsigned int id,
				       unsigned int field_index,
				       unsigned int report_counts);
int hid_open_report(struct hid_device *device);
int hid_check_keys_pressed(struct hid_device *hid);
int hid_connect(struct hid_device *hid, unsigned int connect_mask);
void hid_disconnect(struct hid_device *hid);
const struct hid_device_id *hid_match_id(struct hid_device *hdev,
					 const struct hid_device_id *id);
s32 hid_snto32(__u32 value, unsigned n);
__u32 hid_field_extract(const struct hid_device *hid, __u8 *report,
		     unsigned offset, unsigned n);


static inline void hid_device_io_start(struct hid_device *hid) {
	if (hid->io_started) {
		dev_warn(&hid->dev, "io already started\n");
		return;
	}
	hid->io_started = true;
	up(&hid->driver_input_lock);
}


static inline void hid_device_io_stop(struct hid_device *hid) {
	if (!hid->io_started) {
		dev_warn(&hid->dev, "io already stopped\n");
		return;
	}
	hid->io_started = false;
	down(&hid->driver_input_lock);
}


static inline void hid_map_usage(struct hid_input *hidinput,
		struct hid_usage *usage, unsigned long **bit, int *max,
		__u8 type, unsigned int c)
{
	struct input_dev *input = hidinput->input;
	unsigned long *bmap = NULL;
	unsigned int limit = 0;

	switch (type) {
	case EV_ABS:
		bmap = input->absbit;
		limit = ABS_MAX;
		break;
	case EV_REL:
		bmap = input->relbit;
		limit = REL_MAX;
		break;
	case EV_KEY:
		bmap = input->keybit;
		limit = KEY_MAX;
		break;
	case EV_LED:
		bmap = input->ledbit;
		limit = LED_MAX;
		break;
	}

	if (unlikely(c > limit || !bmap)) {
		pr_warn_ratelimited("%s: Invalid code %d type %d\n",
				    input->name, c, type);
		*bit = NULL;
		return;
	}

	usage->type = type;
	usage->code = c;
	*max = limit;
	*bit = bmap;
}


static inline void hid_map_usage_clear(struct hid_input *hidinput,
		struct hid_usage *usage, unsigned long **bit, int *max,
		__u8 type, __u16 c)
{
	hid_map_usage(hidinput, usage, bit, max, type, c);
	if (*bit)
		clear_bit(usage->code, *bit);
}


static inline int __must_check hid_parse(struct hid_device *hdev)
{
	return hid_open_report(hdev);
}


static inline int __must_check hid_hw_start(struct hid_device *hdev,
		unsigned int connect_mask)
{
	int ret = hdev->ll_driver->start(hdev);
	if (ret || !connect_mask)
		return ret;
	ret = hid_connect(hdev, connect_mask);
	if (ret)
		hdev->ll_driver->stop(hdev);
	return ret;
}


static inline void hid_hw_stop(struct hid_device *hdev)
{
	hid_disconnect(hdev);
	hdev->ll_driver->stop(hdev);
}


static inline int __must_check hid_hw_open(struct hid_device *hdev)
{
	return hdev->ll_driver->open(hdev);
}


static inline void hid_hw_close(struct hid_device *hdev)
{
	hdev->ll_driver->close(hdev);
}



static inline int hid_hw_power(struct hid_device *hdev, int level)
{
	return hdev->ll_driver->power ? hdev->ll_driver->power(hdev, level) : 0;
}



static inline void hid_hw_request(struct hid_device *hdev,
				  struct hid_report *report, int reqtype)
{
	if (hdev->ll_driver->request)
		return hdev->ll_driver->request(hdev, report, reqtype);

	__hid_request(hdev, report, reqtype);
}


static inline int hid_hw_raw_request(struct hid_device *hdev,
				  unsigned char reportnum, __u8 *buf,
				  size_t len, unsigned char rtype, int reqtype)
{
	if (len < 1 || len > HID_MAX_BUFFER_SIZE || !buf)
		return -EINVAL;

	return hdev->ll_driver->raw_request(hdev, reportnum, buf, len,
						    rtype, reqtype);
}


static inline int hid_hw_output_report(struct hid_device *hdev, __u8 *buf,
					size_t len)
{
	if (len < 1 || len > HID_MAX_BUFFER_SIZE || !buf)
		return -EINVAL;

	if (hdev->ll_driver->output_report)
		return hdev->ll_driver->output_report(hdev, buf, len);

	return -ENOSYS;
}


static inline int hid_hw_idle(struct hid_device *hdev, int report, int idle,
		int reqtype)
{
	if (hdev->ll_driver->idle)
		return hdev->ll_driver->idle(hdev, report, idle, reqtype);

	return 0;
}


static inline void hid_hw_wait(struct hid_device *hdev)
{
	if (hdev->ll_driver->wait)
		hdev->ll_driver->wait(hdev);
}


static inline u32 hid_report_len(struct hid_report *report)
{
	return DIV_ROUND_UP(report->size, 8) + (report->id > 0);
}

int hid_report_raw_event(struct hid_device *hid, int type, u8 *data, u32 size,
		int interrupt);


u32 usbhid_lookup_quirk(const u16 idVendor, const u16 idProduct);
int usbhid_quirks_init(char **quirks_param);
void usbhid_quirks_exit(void);

#ifdef CONFIG_HID_PID
int hid_pidff_init(struct hid_device *hid);
#else
#define hid_pidff_init NULL
#endif

#define dbg_hid(format, arg...)						\
do {									\
	if (hid_debug)							\
		printk(KERN_DEBUG "%s: " format, __FILE__, ##arg);	\
} while (0)

#define hid_printk(level, hid, fmt, arg...)		\
	dev_printk(level, &(hid)->dev, fmt, ##arg)
#define hid_emerg(hid, fmt, arg...)			\
	dev_emerg(&(hid)->dev, fmt, ##arg)
#define hid_crit(hid, fmt, arg...)			\
	dev_crit(&(hid)->dev, fmt, ##arg)
#define hid_alert(hid, fmt, arg...)			\
	dev_alert(&(hid)->dev, fmt, ##arg)
#define hid_err(hid, fmt, arg...)			\
	dev_err(&(hid)->dev, fmt, ##arg)
#define hid_notice(hid, fmt, arg...)			\
	dev_notice(&(hid)->dev, fmt, ##arg)
#define hid_warn(hid, fmt, arg...)			\
	dev_warn(&(hid)->dev, fmt, ##arg)
#define hid_info(hid, fmt, arg...)			\
	dev_info(&(hid)->dev, fmt, ##arg)
#define hid_dbg(hid, fmt, arg...)			\
	dev_dbg(&(hid)->dev, fmt, ##arg)

#endif
