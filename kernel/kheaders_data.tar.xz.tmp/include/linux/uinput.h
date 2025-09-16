
#ifndef __UINPUT_H_
#define __UINPUT_H_

#include <uapi/linux/uinput.h>

#define UINPUT_NAME		"uinput"
#define UINPUT_BUFFER_SIZE	16
#define UINPUT_NUM_REQUESTS	16

enum uinput_state { UIST_NEW_DEVICE, UIST_SETUP_COMPLETE, UIST_CREATED };

struct uinput_request {
	unsigned int		id;
	unsigned int		code;	

	int			retval;
	struct completion	done;

	union {
		unsigned int	effect_id;
		struct {
			struct ff_effect *effect;
			struct ff_effect *old;
		} upload;
	} u;
};

struct uinput_device {
	struct input_dev	*dev;
	struct mutex		mutex;
	enum uinput_state	state;
	wait_queue_head_t	waitq;
	unsigned char		ready;
	unsigned char		head;
	unsigned char		tail;
	struct input_event	buff[UINPUT_BUFFER_SIZE];
	unsigned int		ff_effects_max;

	struct uinput_request	*requests[UINPUT_NUM_REQUESTS];
	wait_queue_head_t	requests_waitq;
	spinlock_t		requests_lock;
};
#endif	
