
#ifndef __IMX_IPU_IMAGE_CONVERT_H__
#define __IMX_IPU_IMAGE_CONVERT_H__

#include <video/imx-ipu-v3.h>

struct ipu_image_convert_ctx;


struct ipu_image_convert_run {
	struct ipu_image_convert_ctx *ctx;

	dma_addr_t in_phys;
	dma_addr_t out_phys;

	int status;

	
	struct list_head list;
};


typedef void (*ipu_image_convert_cb_t)(struct ipu_image_convert_run *run,
				       void *ctx);


int ipu_image_convert_enum_format(int index, u32 *fourcc);


void ipu_image_convert_adjust(struct ipu_image *in, struct ipu_image *out,
			      enum ipu_rotate_mode rot_mode);


int ipu_image_convert_verify(struct ipu_image *in, struct ipu_image *out,
			     enum ipu_rotate_mode rot_mode);


struct ipu_image_convert_ctx *
ipu_image_convert_prepare(struct ipu_soc *ipu, enum ipu_ic_task ic_task,
			  struct ipu_image *in, struct ipu_image *out,
			  enum ipu_rotate_mode rot_mode,
			  ipu_image_convert_cb_t complete,
			  void *complete_context);


void ipu_image_convert_unprepare(struct ipu_image_convert_ctx *ctx);


int ipu_image_convert_queue(struct ipu_image_convert_run *run);


void ipu_image_convert_abort(struct ipu_image_convert_ctx *ctx);


struct ipu_image_convert_run *
ipu_image_convert(struct ipu_soc *ipu, enum ipu_ic_task ic_task,
		  struct ipu_image *in, struct ipu_image *out,
		  enum ipu_rotate_mode rot_mode,
		  ipu_image_convert_cb_t complete,
		  void *complete_context);


int ipu_image_convert_sync(struct ipu_soc *ipu, enum ipu_ic_task ic_task,
			   struct ipu_image *in, struct ipu_image *out,
			   enum ipu_rotate_mode rot_mode);


#endif 
