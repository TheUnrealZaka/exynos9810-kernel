

#ifndef IRDA_QOS_H
#define IRDA_QOS_H

#include <linux/skbuff.h>

#include <net/irda/parameters.h>

#define PI_BAUD_RATE     0x01
#define PI_MAX_TURN_TIME 0x82
#define PI_DATA_SIZE     0x83
#define PI_WINDOW_SIZE   0x84
#define PI_ADD_BOFS      0x85
#define PI_MIN_TURN_TIME 0x86
#define PI_LINK_DISC     0x08

#define IR_115200_MAX 0x3f


#define IR_2400     0x01
#define IR_9600     0x02
#define IR_19200    0x04
#define IR_38400    0x08
#define IR_57600    0x10
#define IR_115200   0x20
#define IR_576000   0x40
#define IR_1152000  0x80


#define IR_4000000  0x01
#define IR_16000000 0x02


typedef struct {
	__u32 value;
	__u16 bits; 
} qos_value_t;

struct qos_info {
	magic_t magic;

	qos_value_t baud_rate;       
	qos_value_t max_turn_time;
	qos_value_t data_size;
	qos_value_t window_size;
	qos_value_t additional_bofs;
	qos_value_t min_turn_time;
	qos_value_t link_disc_time;
	
	qos_value_t power;
};

extern int sysctl_max_baud_rate;
extern int sysctl_max_inactive_time;

void irda_init_max_qos_capabilies(struct qos_info *qos);
void irda_qos_compute_intersection(struct qos_info *, struct qos_info *);

__u32 irlap_max_line_capacity(__u32 speed, __u32 max_turn_time);

void irda_qos_bits_to_value(struct qos_info *qos);


#define irlap_min_turn_time_in_bytes(speed, min_turn_time) (	\
	speed * min_turn_time / 10000000			\
)
#define irlap_xbofs_in_usec(speed, xbofs) (			\
	xbofs * 10000000 / speed				\
)

#endif

