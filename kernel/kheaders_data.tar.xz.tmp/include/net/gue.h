#ifndef __NET_GUE_H
#define __NET_GUE_H



struct guehdr {
	union {
		struct {
#if defined(__LITTLE_ENDIAN_BITFIELD)
			__u8	hlen:5,
				control:1,
				version:2;
#elif defined (__BIG_ENDIAN_BITFIELD)
			__u8	version:2,
				control:1,
				hlen:5;
#else
#error  "Please fix <asm/byteorder.h>"
#endif
			__u8    proto_ctype;
			__u16   flags;
		};
		__u32 word;
	};
};



#define GUE_FLAG_PRIV	htons(1<<0)	
#define GUE_LEN_PRIV	4

#define GUE_FLAGS_ALL	(GUE_FLAG_PRIV)



#define GUE_PFLAG_REMCSUM	htonl(1 << 31)
#define GUE_PLEN_REMCSUM	4

#define GUE_PFLAGS_ALL	(GUE_PFLAG_REMCSUM)


static inline size_t guehdr_flags_len(__be16 flags)
{
	return ((flags & GUE_FLAG_PRIV) ? GUE_LEN_PRIV : 0);
}

static inline size_t guehdr_priv_flags_len(__be32 flags)
{
	return 0;
}


static inline int validate_gue_flags(struct guehdr *guehdr,
				     size_t optlen)
{
	size_t len;
	__be32 flags = guehdr->flags;

	if (flags & ~GUE_FLAGS_ALL)
		return 1;

	len = guehdr_flags_len(flags);
	if (len > optlen)
		return 1;

	if (flags & GUE_FLAG_PRIV) {
		
		flags = *(__be32 *)((void *)&guehdr[1] + len - GUE_LEN_PRIV);

		if (flags & ~GUE_PFLAGS_ALL)
			return 1;

		len += guehdr_priv_flags_len(flags);
		if (len > optlen)
			return 1;
	}

	return 0;
}

#endif
