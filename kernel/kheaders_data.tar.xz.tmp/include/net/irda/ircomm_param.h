

#ifndef IRCOMM_PARAMS_H
#define IRCOMM_PARAMS_H

#include <net/irda/parameters.h>


#define IRCOMM_SERVICE_TYPE     0x00
#define IRCOMM_PORT_TYPE        0x01 
#define IRCOMM_PORT_NAME        0x02 


#define IRCOMM_DATA_RATE        0x10
#define IRCOMM_DATA_FORMAT      0x11
#define IRCOMM_FLOW_CONTROL     0x12
#define IRCOMM_XON_XOFF         0x13
#define IRCOMM_ENQ_ACK          0x14
#define IRCOMM_LINE_STATUS      0x15
#define IRCOMM_BREAK            0x16


#define IRCOMM_DTE              0x20
#define IRCOMM_DCE              0x21
#define IRCOMM_POLL             0x22


#define IRCOMM_3_WIRE_RAW       0x01
#define IRCOMM_3_WIRE           0x02
#define IRCOMM_9_WIRE           0x04
#define IRCOMM_CENTRONICS       0x08


#define IRCOMM_SERIAL           0x00
#define IRCOMM_PARALLEL         0x01


#define IRCOMM_WSIZE_5          0x00
#define IRCOMM_WSIZE_6          0x01
#define IRCOMM_WSIZE_7          0x02
#define IRCOMM_WSIZE_8          0x03

#define IRCOMM_1_STOP_BIT       0x00
#define IRCOMM_2_STOP_BIT       0x04 

#define IRCOMM_PARITY_DISABLE   0x00
#define IRCOMM_PARITY_ENABLE    0x08

#define IRCOMM_PARITY_ODD       0x00
#define IRCOMM_PARITY_EVEN      0x10
#define IRCOMM_PARITY_MARK      0x20
#define IRCOMM_PARITY_SPACE     0x30


#define IRCOMM_XON_XOFF_IN      0x01
#define IRCOMM_XON_XOFF_OUT     0x02
#define IRCOMM_RTS_CTS_IN       0x04
#define IRCOMM_RTS_CTS_OUT      0x08
#define IRCOMM_DSR_DTR_IN       0x10
#define IRCOMM_DSR_DTR_OUT      0x20
#define IRCOMM_ENQ_ACK_IN       0x40
#define IRCOMM_ENQ_ACK_OUT      0x80


#define IRCOMM_OVERRUN_ERROR    0x02
#define IRCOMM_PARITY_ERROR     0x04
#define IRCOMM_FRAMING_ERROR    0x08


#define IRCOMM_DELTA_DTR        0x01
#define IRCOMM_DELTA_RTS        0x02
#define IRCOMM_DTR              0x04
#define IRCOMM_RTS              0x08


#define IRCOMM_DELTA_CTS        0x01  
#define IRCOMM_DELTA_DSR        0x02  
#define IRCOMM_DELTA_RI         0x04  
#define IRCOMM_DELTA_CD         0x08  
#define IRCOMM_CTS              0x10  
#define IRCOMM_DSR              0x20  
#define IRCOMM_RI               0x40  
#define IRCOMM_CD               0x80  
#define IRCOMM_DCE_DELTA_ANY    0x0f


struct ircomm_params {
	
	__u8  service_type;
	__u8  port_type;
	char  port_name[32];

	
	__u32 data_rate;         
	__u8  data_format;
	__u8  flow_control;
	char  xonxoff[2];
	char  enqack[2];
	__u8  line_status;
	__u8  _break;

	__u8  null_modem;

	
	__u8 dte;
	__u8 dce;
	__u8 poll;

	
};

struct ircomm_tty_cb; 

int ircomm_param_request(struct ircomm_tty_cb *self, __u8 pi, int flush);

extern pi_param_info_t ircomm_param_info;

#endif 

