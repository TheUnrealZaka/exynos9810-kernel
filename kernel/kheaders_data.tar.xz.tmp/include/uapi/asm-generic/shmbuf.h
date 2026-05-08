#ifndef __ASM_GENERIC_SHMBUF_H
#define __ASM_GENERIC_SHMBUF_H

#include <asm/bitsperlong.h>



struct shmid64_ds {
	struct ipc64_perm	shm_perm;	
	size_t			shm_segsz;	
	__kernel_time_t		shm_atime;	
#if __BITS_PER_LONG != 64
	unsigned long		__unused1;
#endif
	__kernel_time_t		shm_dtime;	
#if __BITS_PER_LONG != 64
	unsigned long		__unused2;
#endif
	__kernel_time_t		shm_ctime;	
#if __BITS_PER_LONG != 64
	unsigned long		__unused3;
#endif
	__kernel_pid_t		shm_cpid;	
	__kernel_pid_t		shm_lpid;	
	__kernel_ulong_t	shm_nattch;	
	__kernel_ulong_t	__unused4;
	__kernel_ulong_t	__unused5;
};

struct shminfo64 {
	__kernel_ulong_t	shmmax;
	__kernel_ulong_t	shmmin;
	__kernel_ulong_t	shmmni;
	__kernel_ulong_t	shmseg;
	__kernel_ulong_t	shmall;
	__kernel_ulong_t	__unused1;
	__kernel_ulong_t	__unused2;
	__kernel_ulong_t	__unused3;
	__kernel_ulong_t	__unused4;
};

#endif 
