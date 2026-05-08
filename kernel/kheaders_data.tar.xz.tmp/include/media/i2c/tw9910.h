

#ifndef __TW9910_H__
#define __TW9910_H__

#include <media/soc_camera.h>

enum tw9910_mpout_pin {
	TW9910_MPO_VLOSS,
	TW9910_MPO_HLOCK,
	TW9910_MPO_SLOCK,
	TW9910_MPO_VLOCK,
	TW9910_MPO_MONO,
	TW9910_MPO_DET50,
	TW9910_MPO_FIELD,
	TW9910_MPO_RTCO,
};

struct tw9910_video_info {
	unsigned long		buswidth;
	enum tw9910_mpout_pin	mpout;
};


#endif 
