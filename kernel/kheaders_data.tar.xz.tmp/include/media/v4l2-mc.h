

#ifndef _V4L2_MC_H
#define _V4L2_MC_H

#include <media/media-device.h>
#include <media/v4l2-dev.h>
#include <linux/types.h>


enum tuner_pad_index {
	TUNER_PAD_RF_INPUT,
	TUNER_PAD_OUTPUT,
	TUNER_PAD_AUD_OUT,
	TUNER_NUM_PADS
};


enum if_vid_dec_pad_index {
	IF_VID_DEC_PAD_IF_INPUT,
	IF_VID_DEC_PAD_OUT,
	IF_VID_DEC_PAD_NUM_PADS
};


enum if_aud_dec_pad_index {
	IF_AUD_DEC_PAD_IF_INPUT,
	IF_AUD_DEC_PAD_OUT,
	IF_AUD_DEC_PAD_NUM_PADS
};


enum demod_pad_index {
	DEMOD_PAD_IF_INPUT,
	DEMOD_PAD_VID_OUT,
	DEMOD_PAD_VBI_OUT,
	DEMOD_PAD_AUDIO_OUT,
	DEMOD_NUM_PADS
};


struct pci_dev;
struct usb_device;

#ifdef CONFIG_MEDIA_CONTROLLER

int v4l2_mc_create_media_graph(struct media_device *mdev);


int v4l_enable_media_source(struct video_device *vdev);


void v4l_disable_media_source(struct video_device *vdev);


int v4l_vb2q_enable_media_source(struct vb2_queue *q);



int v4l2_pipeline_pm_use(struct media_entity *entity, int use);



int v4l2_pipeline_link_notify(struct media_link *link, u32 flags,
			      unsigned int notification);

#else 

static inline int v4l2_mc_create_media_graph(struct media_device *mdev)
{
	return 0;
}

static inline int v4l_enable_media_source(struct video_device *vdev)
{
	return 0;
}

static inline void v4l_disable_media_source(struct video_device *vdev)
{
}

static inline int v4l_vb2q_enable_media_source(struct vb2_queue *q)
{
	return 0;
}

static inline int v4l2_pipeline_pm_use(struct media_entity *entity, int use)
{
	return 0;
}

static inline int v4l2_pipeline_link_notify(struct media_link *link, u32 flags,
					    unsigned int notification)
{
	return 0;
}

#endif 
#endif 
