

#ifndef __V4L2_DV_TIMINGS_H
#define __V4L2_DV_TIMINGS_H

#include <linux/videodev2.h>


extern const struct v4l2_dv_timings v4l2_dv_timings_presets[];


typedef bool v4l2_check_dv_timings_fnc(const struct v4l2_dv_timings *t, void *handle);


bool v4l2_valid_dv_timings(const struct v4l2_dv_timings *t,
			   const struct v4l2_dv_timings_cap *cap,
			   v4l2_check_dv_timings_fnc fnc,
			   void *fnc_handle);


int v4l2_enum_dv_timings_cap(struct v4l2_enum_dv_timings *t,
			     const struct v4l2_dv_timings_cap *cap,
			     v4l2_check_dv_timings_fnc fnc,
			     void *fnc_handle);


bool v4l2_find_dv_timings_cap(struct v4l2_dv_timings *t,
			      const struct v4l2_dv_timings_cap *cap,
			      unsigned pclock_delta,
			      v4l2_check_dv_timings_fnc fnc,
			      void *fnc_handle);


bool v4l2_match_dv_timings(const struct v4l2_dv_timings *measured,
			   const struct v4l2_dv_timings *standard,
			   unsigned pclock_delta, bool match_reduced_fps);


void v4l2_print_dv_timings(const char *dev_prefix, const char *prefix,
			   const struct v4l2_dv_timings *t, bool detailed);


bool v4l2_detect_cvt(unsigned frame_height, unsigned hfreq, unsigned vsync,
		unsigned active_width, u32 polarities, bool interlaced,
		struct v4l2_dv_timings *fmt);


bool v4l2_detect_gtf(unsigned frame_height, unsigned hfreq, unsigned vsync,
		u32 polarities, bool interlaced, struct v4l2_fract aspect,
		struct v4l2_dv_timings *fmt);


struct v4l2_fract v4l2_calc_aspect_ratio(u8 hor_landscape, u8 vert_portrait);


static inline  bool can_reduce_fps(struct v4l2_bt_timings *bt)
{
	if ((bt->standards & V4L2_DV_BT_STD_CVT) && (bt->vsync == 8))
		return true;

	if ((bt->standards & V4L2_DV_BT_STD_CEA861) &&
	    (bt->flags & V4L2_DV_FL_CAN_REDUCE_FPS))
		return true;

	return false;
}


#endif
