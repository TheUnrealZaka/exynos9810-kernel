#ifndef _UAPI_LINUX_MEMBARRIER_H
#define _UAPI_LINUX_MEMBARRIER_H




enum membarrier_cmd {
	MEMBARRIER_CMD_QUERY = 0,
	MEMBARRIER_CMD_SHARED = (1 << 0),
};

#endif 
