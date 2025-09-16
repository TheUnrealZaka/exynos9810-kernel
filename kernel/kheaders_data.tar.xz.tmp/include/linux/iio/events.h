
#ifndef _IIO_EVENTS_H_
#define _IIO_EVENTS_H_

#include <linux/iio/types.h>
#include <uapi/linux/iio/events.h>



#define IIO_EVENT_CODE(chan_type, diff, modifier, direction,		\
		       type, chan, chan1, chan2)			\
	(((u64)type << 56) | ((u64)diff << 55) |			\
	 ((u64)direction << 48) | ((u64)modifier << 40) |		\
	 ((u64)chan_type << 32) | (((u16)chan2) << 16) | ((u16)chan1) | \
	 ((u16)chan))




#define IIO_MOD_EVENT_CODE(chan_type, number, modifier,		\
			   type, direction)				\
	IIO_EVENT_CODE(chan_type, 0, modifier, direction, type, number, 0, 0)



#define IIO_UNMOD_EVENT_CODE(chan_type, number, type, direction)	\
	IIO_EVENT_CODE(chan_type, 0, 0, direction, type, number, 0, 0)

#endif
