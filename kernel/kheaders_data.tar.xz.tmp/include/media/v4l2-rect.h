

#ifndef _V4L2_RECT_H_
#define _V4L2_RECT_H_

#include <linux/videodev2.h>


static inline void v4l2_rect_set_size_to(struct v4l2_rect *r,
					 const struct v4l2_rect *size)
{
	r->width = size->width;
	r->height = size->height;
}


static inline void v4l2_rect_set_min_size(struct v4l2_rect *r,
					  const struct v4l2_rect *min_size)
{
	if (r->width < min_size->width)
		r->width = min_size->width;
	if (r->height < min_size->height)
		r->height = min_size->height;
}


static inline void v4l2_rect_set_max_size(struct v4l2_rect *r,
					  const struct v4l2_rect *max_size)
{
	if (r->width > max_size->width)
		r->width = max_size->width;
	if (r->height > max_size->height)
		r->height = max_size->height;
}


static inline void v4l2_rect_map_inside(struct v4l2_rect *r,
					const struct v4l2_rect *boundary)
{
	v4l2_rect_set_max_size(r, boundary);
	if (r->left < boundary->left)
		r->left = boundary->left;
	if (r->top < boundary->top)
		r->top = boundary->top;
	if (r->left + r->width > boundary->left + boundary->width)
		r->left = boundary->left + boundary->width - r->width;
	if (r->top + r->height > boundary->top + boundary->height)
		r->top = boundary->top + boundary->height - r->height;
}


static inline bool v4l2_rect_same_size(const struct v4l2_rect *r1,
				       const struct v4l2_rect *r2)
{
	return r1->width == r2->width && r1->height == r2->height;
}


static inline void v4l2_rect_intersect(struct v4l2_rect *r,
				       const struct v4l2_rect *r1,
				       const struct v4l2_rect *r2)
{
	int right, bottom;

	r->top = max(r1->top, r2->top);
	r->left = max(r1->left, r2->left);
	bottom = min(r1->top + r1->height, r2->top + r2->height);
	right = min(r1->left + r1->width, r2->left + r2->width);
	r->height = max(0, bottom - r->top);
	r->width = max(0, right - r->left);
}


static inline void v4l2_rect_scale(struct v4l2_rect *r,
				   const struct v4l2_rect *from,
				   const struct v4l2_rect *to)
{
	if (from->width == 0 || from->height == 0) {
		r->left = r->top = r->width = r->height = 0;
		return;
	}
	r->left = (((r->left - from->left) * to->width) / from->width) & ~1;
	r->width = ((r->width * to->width) / from->width) & ~1;
	r->top = ((r->top - from->top) * to->height) / from->height;
	r->height = (r->height * to->height) / from->height;
}


static inline bool v4l2_rect_overlap(const struct v4l2_rect *r1,
				     const struct v4l2_rect *r2)
{
	
	if (r1->left >= r2->left + r2->width ||
	    r2->left >= r1->left + r1->width)
		return false;
	
	if (r1->top >= r2->top + r2->height ||
	    r2->top >= r1->top + r1->height)
		return false;
	return true;
}

#endif
