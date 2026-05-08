

#ifndef __CPP_H__
#define __CPP_H__

#include <linux/scatterlist.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <crypto/aes.h>
#include <crypto/sha.h>


struct ccp_device;
struct ccp_cmd;

#if defined(CONFIG_CRYPTO_DEV_CCP_DD) || \
	defined(CONFIG_CRYPTO_DEV_CCP_DD_MODULE)


int ccp_present(void);

#define	CCP_VSIZE 16
#define	CCP_VMASK		((unsigned int)((1 << CCP_VSIZE) - 1))
#define	CCP_VERSION(v, r)	((unsigned int)((v << CCP_VSIZE) \
					       | (r & CCP_VMASK)))


unsigned int ccp_version(void);


int ccp_enqueue_cmd(struct ccp_cmd *cmd);

#else 

static inline int ccp_present(void)
{
	return -ENODEV;
}

static inline unsigned int ccp_version(void)
{
	return 0;
}

static inline int ccp_enqueue_cmd(struct ccp_cmd *cmd)
{
	return -ENODEV;
}

#endif 




enum ccp_aes_type {
	CCP_AES_TYPE_128 = 0,
	CCP_AES_TYPE_192,
	CCP_AES_TYPE_256,
	CCP_AES_TYPE__LAST,
};


enum ccp_aes_mode {
	CCP_AES_MODE_ECB = 0,
	CCP_AES_MODE_CBC,
	CCP_AES_MODE_OFB,
	CCP_AES_MODE_CFB,
	CCP_AES_MODE_CTR,
	CCP_AES_MODE_CMAC,
	CCP_AES_MODE__LAST,
};


enum ccp_aes_action {
	CCP_AES_ACTION_DECRYPT = 0,
	CCP_AES_ACTION_ENCRYPT,
	CCP_AES_ACTION__LAST,
};


struct ccp_aes_engine {
	enum ccp_aes_type type;
	enum ccp_aes_mode mode;
	enum ccp_aes_action action;

	struct scatterlist *key;
	u32 key_len;		

	struct scatterlist *iv;
	u32 iv_len;		

	struct scatterlist *src, *dst;
	u64 src_len;		

	u32 cmac_final;		
	struct scatterlist *cmac_key;	
	u32 cmac_key_len;	
};



enum ccp_xts_aes_unit_size {
	CCP_XTS_AES_UNIT_SIZE_16 = 0,
	CCP_XTS_AES_UNIT_SIZE_512,
	CCP_XTS_AES_UNIT_SIZE_1024,
	CCP_XTS_AES_UNIT_SIZE_2048,
	CCP_XTS_AES_UNIT_SIZE_4096,
	CCP_XTS_AES_UNIT_SIZE__LAST,
};


struct ccp_xts_aes_engine {
	enum ccp_aes_type type;
	enum ccp_aes_action action;
	enum ccp_xts_aes_unit_size unit_size;

	struct scatterlist *key;
	u32 key_len;		

	struct scatterlist *iv;
	u32 iv_len;		

	struct scatterlist *src, *dst;
	u64 src_len;		

	u32 final;
};



enum ccp_sha_type {
	CCP_SHA_TYPE_1 = 1,
	CCP_SHA_TYPE_224,
	CCP_SHA_TYPE_256,
	CCP_SHA_TYPE__LAST,
};


struct ccp_sha_engine {
	enum ccp_sha_type type;

	struct scatterlist *ctx;
	u32 ctx_len;		

	struct scatterlist *src;
	u64 src_len;		

	struct scatterlist *opad;
	u32 opad_len;		

	u32 first;		
	u32 final;		
	u64 msg_bits;		
};



struct ccp_rsa_engine {
	u32 key_size;		

	struct scatterlist *exp;
	u32 exp_len;		

	struct scatterlist *mod;
	u32 mod_len;		

	struct scatterlist *src, *dst;
	u32 src_len;		
};



enum ccp_passthru_bitwise {
	CCP_PASSTHRU_BITWISE_NOOP = 0,
	CCP_PASSTHRU_BITWISE_AND,
	CCP_PASSTHRU_BITWISE_OR,
	CCP_PASSTHRU_BITWISE_XOR,
	CCP_PASSTHRU_BITWISE_MASK,
	CCP_PASSTHRU_BITWISE__LAST,
};


enum ccp_passthru_byteswap {
	CCP_PASSTHRU_BYTESWAP_NOOP = 0,
	CCP_PASSTHRU_BYTESWAP_32BIT,
	CCP_PASSTHRU_BYTESWAP_256BIT,
	CCP_PASSTHRU_BYTESWAP__LAST,
};


struct ccp_passthru_engine {
	enum ccp_passthru_bitwise bit_mod;
	enum ccp_passthru_byteswap byte_swap;

	struct scatterlist *mask;
	u32 mask_len;		

	struct scatterlist *src, *dst;
	u64 src_len;		

	u32 final;
};


struct ccp_passthru_nomap_engine {
	enum ccp_passthru_bitwise bit_mod;
	enum ccp_passthru_byteswap byte_swap;

	dma_addr_t mask;
	u32 mask_len;		

	dma_addr_t src_dma, dst_dma;
	u64 src_len;		

	u32 final;
};


#define CCP_ECC_MODULUS_BYTES	48	
#define CCP_ECC_MAX_OPERANDS	6
#define CCP_ECC_MAX_OUTPUTS	3


enum ccp_ecc_function {
	CCP_ECC_FUNCTION_MMUL_384BIT = 0,
	CCP_ECC_FUNCTION_MADD_384BIT,
	CCP_ECC_FUNCTION_MINV_384BIT,
	CCP_ECC_FUNCTION_PADD_384BIT,
	CCP_ECC_FUNCTION_PMUL_384BIT,
	CCP_ECC_FUNCTION_PDBL_384BIT,
};


struct ccp_ecc_modular_math {
	struct scatterlist *operand_1;
	unsigned int operand_1_len;	

	struct scatterlist *operand_2;
	unsigned int operand_2_len;	

	struct scatterlist *result;
	unsigned int result_len;	
};


struct ccp_ecc_point {
	struct scatterlist *x;
	unsigned int x_len;	

	struct scatterlist *y;
	unsigned int y_len;	
};


struct ccp_ecc_point_math {
	struct ccp_ecc_point point_1;
	struct ccp_ecc_point point_2;

	struct scatterlist *domain_a;
	unsigned int domain_a_len;	

	struct scatterlist *scalar;
	unsigned int scalar_len;	

	struct ccp_ecc_point result;
};


struct ccp_ecc_engine {
	enum ccp_ecc_function function;

	struct scatterlist *mod;
	u32 mod_len;		

	union {
		struct ccp_ecc_modular_math mm;
		struct ccp_ecc_point_math pm;
	} u;

	u16 ecc_result;
};



enum ccp_engine {
	CCP_ENGINE_AES = 0,
	CCP_ENGINE_XTS_AES_128,
	CCP_ENGINE_RSVD1,
	CCP_ENGINE_SHA,
	CCP_ENGINE_RSA,
	CCP_ENGINE_PASSTHRU,
	CCP_ENGINE_ZLIB_DECOMPRESS,
	CCP_ENGINE_ECC,
	CCP_ENGINE__LAST,
};


#define CCP_CMD_MAY_BACKLOG		0x00000001
#define CCP_CMD_PASSTHRU_NO_DMA_MAP	0x00000002


struct ccp_cmd {
	
	struct list_head entry;
	struct work_struct work;
	struct ccp_device *ccp;
	int ret;

	u32 flags;

	enum ccp_engine engine;
	u32 engine_error;

	union {
		struct ccp_aes_engine aes;
		struct ccp_xts_aes_engine xts;
		struct ccp_sha_engine sha;
		struct ccp_rsa_engine rsa;
		struct ccp_passthru_engine passthru;
		struct ccp_passthru_nomap_engine passthru_nomap;
		struct ccp_ecc_engine ecc;
	} u;

	
	void (*callback)(void *data, int err);
	void *data;
};

#endif
