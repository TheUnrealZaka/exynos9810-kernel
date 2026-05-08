

#ifndef DRM_RECT_H
#define DRM_RECT_H




struct drm_rect {
	int x1, y1, x2, y2;
};


static inline void drm_rect_adjust_size(struct drm_rect *r, int dw, int dh)
{
	r->x1 -= dw >> 1;
	r->y1 -= dh >> 1;
	r->x2 += (dw + 1) >> 1;
	r->y2 += (dh + 1) >> 1;
}


static inline void drm_rect_translate(struct drm_rect *r, int dx, int dy)
{
	r->x1 += dx;
	r->y1 += dy;
	r->x2 += dx;
	r->y2 += dy;
}


static inline void drm_rect_downscale(struct drm_rect *r, int horz, int vert)
{
	r->x1 /= horz;
	r->y1 /= vert;
	r->x2 /= horz;
	r->y2 /= vert;
}


static inline int drm_rect_width(const struct drm_rect *r)
{
	return r->x2 - r->x1;
}


static inline int drm_rect_height(const struct drm_rect *r)
{
	return r->y2 - r->y1;
}


static inline bool drm_rect_visible(const struct drm_rect *r)
{
	return drm_rect_width(r) > 0 && drm_rect_height(r) > 0;
}


static inline bool drm_rect_equals(const struct drm_rect *r1,
				   const struct drm_rect *r2)
{
	return r1->x1 == r2->x1 && r1->x2 == r2->x2 &&
		r1->y1 == r2->y1 && r1->y2 == r2->y2;
}

bool drm_rect_intersect(struct drm_rect *r, const struct drm_rect *clip);
bool drm_rect_clip_scaled(struct drm_rect *src, struct drm_rect *dst,
			  const struct drm_rect *clip,
			  int hscale, int vscale);
int drm_rect_calc_hscale(const struct drm_rect *src,
			 const struct drm_rect *dst,
			 int min_hscale, int max_hscale);
int drm_rect_calc_vscale(const struct drm_rect *src,
			 const struct drm_rect *dst,
			 int min_vscale, int max_vscale);
int drm_rect_calc_hscale_relaxed(struct drm_rect *src,
				 struct drm_rect *dst,
				 int min_hscale, int max_hscale);
int drm_rect_calc_vscale_relaxed(struct drm_rect *src,
				 struct drm_rect *dst,
				 int min_vscale, int max_vscale);
void drm_rect_debug_print(const char *prefix,
			  const struct drm_rect *r, bool fixed_point);
void drm_rect_rotate(struct drm_rect *r,
		     int width, int height,
		     unsigned int rotation);
void drm_rect_rotate_inv(struct drm_rect *r,
			 int width, int height,
			 unsigned int rotation);

#endif
