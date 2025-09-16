
#ifndef _LINUX_VIRTIO_CONSOLE_H
#define _LINUX_VIRTIO_CONSOLE_H

#include <uapi/linux/virtio_console.h>

int __init virtio_cons_early_init(int (*put_chars)(u32, const char *, int));
#endif 
