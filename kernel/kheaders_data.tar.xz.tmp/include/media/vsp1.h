
#ifndef __MEDIA_VSP1_H__
#define __MEDIA_VSP1_H__

#include <linux/types.h>
#include <linux/videodev2.h>

struct device;

int vsp1_du_init(struct device *dev);

int vsp1_du_setup_lif(struct device *dev, unsigned int width,
		      unsigned int height);

struct vsp1_du_atomic_config {
	u32 pixelformat;
	unsigned int pitch;
	dma_addr_t mem[3];
	struct v4l2_rect src;
	struct v4l2_rect dst;
	unsigned int alpha;
	unsigned int zpos;
};

void vsp1_du_atomic_begin(struct device *dev);
int vsp1_du_atomic_update(struct device *dev, unsigned int rpf,
			  const struct vsp1_du_atomic_config *cfg);
void vsp1_du_atomic_flush(struct device *dev);

#endif 
