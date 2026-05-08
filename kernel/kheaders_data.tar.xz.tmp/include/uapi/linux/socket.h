#ifndef _UAPI_LINUX_SOCKET_H
#define _UAPI_LINUX_SOCKET_H


#define _K_SS_MAXSIZE	128	
#define _K_SS_ALIGNSIZE	(__alignof__ (struct sockaddr *))
				

typedef unsigned short __kernel_sa_family_t;

struct __kernel_sockaddr_storage {
	__kernel_sa_family_t	ss_family;		
	
	char		__data[_K_SS_MAXSIZE - sizeof(unsigned short)];
				
				
} __attribute__ ((aligned(_K_SS_ALIGNSIZE)));	

#endif 
