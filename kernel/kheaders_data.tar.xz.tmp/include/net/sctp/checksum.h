

#ifndef __sctp_checksum_h__
#define __sctp_checksum_h__

#include <linux/types.h>
#include <net/sctp/sctp.h>
#include <linux/crc32c.h>
#include <linux/crc32.h>

static inline __wsum sctp_csum_update(const void *buff, int len, __wsum sum)
{
	
	return crc32c(sum, buff, len);
}

static inline __wsum sctp_csum_combine(__wsum csum, __wsum csum2,
				       int offset, int len)
{
	return __crc32c_le_combine(csum, csum2, len);
}

static inline __le32 sctp_compute_cksum(const struct sk_buff *skb,
					unsigned int offset)
{
	struct sctphdr *sh = (struct sctphdr *)(skb->data + offset);
        __le32 ret, old = sh->checksum;
	const struct skb_checksum_ops ops = {
		.update  = sctp_csum_update,
		.combine = sctp_csum_combine,
	};

	sh->checksum = 0;
	ret = cpu_to_le32(~__skb_checksum(skb, offset, skb->len - offset,
					  ~(__u32)0, &ops));
	sh->checksum = old;

	return ret;
}

#endif 
