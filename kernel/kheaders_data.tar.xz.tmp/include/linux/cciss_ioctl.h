#ifndef CCISS_IOCTLH
#define CCISS_IOCTLH

#include <uapi/linux/cciss_ioctl.h>

#ifdef CONFIG_COMPAT


typedef struct _IOCTL32_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info;
  WORD			   buf_size;  
  __u32			   buf; 
} IOCTL32_Command_struct;

typedef struct _BIG_IOCTL32_Command_struct {
  LUNAddr_struct	   LUN_info;
  RequestBlock_struct      Request;
  ErrorInfo_struct  	   error_info;
  DWORD			   malloc_size; 
  DWORD			   buf_size;    
  				        
  __u32 		buf;	
} BIG_IOCTL32_Command_struct;

#define CCISS_PASSTHRU32   _IOWR(CCISS_IOC_MAGIC, 11, IOCTL32_Command_struct)
#define CCISS_BIG_PASSTHRU32 _IOWR(CCISS_IOC_MAGIC, 18, BIG_IOCTL32_Command_struct)

#endif 
#endif  
