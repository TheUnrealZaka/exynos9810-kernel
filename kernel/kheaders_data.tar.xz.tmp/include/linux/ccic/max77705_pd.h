

#ifndef __LINUX_MFD_MAX77705_PD_H
#define __LINUX_MFD_MAX77705_PD_H
#include <linux/ccic/max77705.h>
#include <linux/wakelock.h>

#define MAX77705_PD_NAME	"MAX77705_PD"

enum {
	CC_SNK = 0,
	CC_SRC,
	CC_NO_CONN,
};

typedef enum {
	PDO_TYPE_FIXED = 0,
	PDO_TYPE_BATTERY,
	PDO_TYPE_VARIABLE,
	PDO_TYPE_APDO
} pdo_supply_type_t;

typedef union sec_pdo_object {
	uint32_t		data;
	struct {
		uint8_t		bdata[4];
	} BYTES;
	struct {
		uint32_t	reserved:30,
					type:2;
	} BITS_supply;
	struct {
		uint32_t	max_current:10,        
				voltage:10,            
				peak_current:2,
				reserved:2,
				unchuncked_extended_messages_supported:1,
				data_role_data:1,
				usb_communications_capable:1,
				unconstrained_power:1,
				usb_suspend_supported:1,
				dual_role_power:1,
				supply:2;			
	} BITS_pdo_fixed;
	struct {
		uint32_t	max_current:10,		
				min_voltage:10,		
				max_voltage:10,		
				supply:2;		
	} BITS_pdo_variable;
	struct {
		uint32_t	max_allowable_power:10,		
				min_voltage:10,		
				max_voltage:10,		
				supply:2;		
	} BITS_pdo_battery;
	struct {
		uint32_t	max_current:7, 	
				reserved1:1,
				min_voltage:8, 	
				reserved2:1,
				max_voltage:8, 	
				reserved3:2,
				pps_power_limited:1,
				pps_supply:2,
				supply:2;		
	} BITS_pdo_programmable;
} U_SEC_PDO_OBJECT;

struct max77705_pd_data {
	
	int irq_pdmsg;
	int irq_psrdy;
	int irq_datarole;
	int irq_ssacc;
	int irq_fct_id;

	u8 usbc_status1;
	u8 usbc_status2;
	u8 bc_status;
	u8 cc_status0;
	u8 cc_status1;
	u8 pd_status0;
	u8 pd_status1;

	u8 opcode_res;

	
	u8 pdsmg;

	
	enum max77705_data_role current_dr;
	enum max77705_data_role previous_dr;
	
	u8 ssacc;
	
	u8 fct_id;
	enum max77705_ccpd_device device;

	bool pdo_list;
	bool psrdy_received;

	int cc_status;

	
	struct wake_lock pdmsg_wake_lock;
	struct wake_lock datarole_wake_lock;
	struct wake_lock ssacc_wake_lock;
	struct wake_lock fct_id_wake_lock;
};

#endif
