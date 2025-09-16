

#ifndef _LINUX_BRCM_MESSAGE_H_
#define _LINUX_BRCM_MESSAGE_H_

#include <linux/scatterlist.h>

enum brcm_message_type {
	BRCM_MESSAGE_UNKNOWN = 0,
	BRCM_MESSAGE_SPU,
	BRCM_MESSAGE_SBA,
	BRCM_MESSAGE_MAX,
};

struct brcm_sba_command {
	u64 cmd;
#define BRCM_SBA_CMD_TYPE_A		BIT(0)
#define BRCM_SBA_CMD_TYPE_B		BIT(1)
#define BRCM_SBA_CMD_TYPE_C		BIT(2)
#define BRCM_SBA_CMD_HAS_RESP		BIT(3)
#define BRCM_SBA_CMD_HAS_OUTPUT		BIT(4)
	u64 flags;
	dma_addr_t input;
	size_t input_len;
	dma_addr_t resp;
	size_t resp_len;
	dma_addr_t output;
	size_t output_len;
};

struct brcm_message {
	enum brcm_message_type type;
	union {
		struct {
			struct scatterlist *src;
			struct scatterlist *dst;
		} spu;
		struct {
			struct brcm_sba_command *cmds;
			unsigned int cmds_count;
		} sba;
	};
	void *ctx;
	int error;
};

#endif 
