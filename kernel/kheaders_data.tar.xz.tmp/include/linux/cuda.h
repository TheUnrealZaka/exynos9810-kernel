
#ifndef _LINUX_CUDA_H
#define _LINUX_CUDA_H

#include <uapi/linux/cuda.h>


extern int find_via_cuda(void);
extern int cuda_request(struct adb_request *req,
			void (*done)(struct adb_request *), int nbytes, ...);
extern void cuda_poll(void);

#endif 
