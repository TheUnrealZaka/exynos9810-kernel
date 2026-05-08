

#ifndef __EXYNOS_SECLOG_H
#define __EXYNOS_SECLOG_H


#define ERROR_INVALID_LOG_LEN			(-1)
#define ERROR_INVALID_LOG_ADDR			(-2)
#define ERROR_INVALID_INTR_NUM			(-3)
#define ERROR_ALREADY_INITIALIZED		(-4)


#define ERROR_LDFW_ALREADY_INITIALIZED		(1)
#define ERROR_LDFW_CALL_FROM_NON_SECURE		(2)
#define ERROR_NOT_SUPPORT_LDFW_ERR_VALUE	(0xE)
#define ERROR_NOT_SUPPORT_LDFW_SEC_LOG		(0xF)

#define BITLEN_LDFW_ERROR			(4)
#define MASK_LDFW_ERROR				(0xF)

#define SHIFT_LDFW_MAGIC			(28)
#define MASK_LDFW_MAGIC				(0xF << SHIFT_LDFW_MAGIC)
#define LDFW_MAGIC				(0xA << SHIFT_LDFW_MAGIC)

#define LDFW_MAX_NUM				(7)


#define MIN_LOG_BUF_LEN				(0x10000)	


#define FOUR_BYTES_SHIFT			(2)
#define FOUR_BYTES_MASK				((1 << FOUR_BYTES_SHIFT) - 1)


#define CHECK_AND_ALIGN_4BYTES(addr)		do {		\
		if ((addr) & FOUR_BYTES_MASK) {			\
			addr &= ~FOUR_BYTES_MASK;		\
			addr += (1 << FOUR_BYTES_SHIFT);	\
		}						\
	} while (0)

#ifndef __ASSEMBLY__

struct seclog_data {
	void *virt_addr;
	unsigned long phys_addr;
	unsigned long size;
};


struct seclog_ctx {
	struct work_struct work;
	
	struct dentry *debug_dir;
	
	bool enabled;
	unsigned int irq;
};


struct log_header_info {
	unsigned int log_len;
	unsigned int tv_sec;
	unsigned int tv_usec;
};


struct sec_log_info {
	
	unsigned int log_write_cnt;
	
	unsigned int log_read_cnt;
	
	unsigned int log_return_cnt;
	
	unsigned long start_log_addr;
	
	unsigned long initial_log_addr;
};
#endif	

#endif	
