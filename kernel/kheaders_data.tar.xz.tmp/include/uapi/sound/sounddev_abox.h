

#ifndef __SND_DEV_ABOX_H
#define __SND_DEV_ABOX_H

struct snd_pcm_mmap_fd {
	int32_t dir;
	int32_t fd;
	int32_t size;
	int32_t actual_size;
};

#define SNDRV_PCM_IOCTL_MMAP_DATA_FD    _IOWR('U', 0xd2, struct snd_pcm_mmap_fd)

#endif

