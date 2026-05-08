

#ifndef _LINUX_ASN1_DECODER_H
#define _LINUX_ASN1_DECODER_H

#include <linux/asn1.h>

struct asn1_decoder;

extern int asn1_ber_decoder(const struct asn1_decoder *decoder,
			    void *context,
			    const unsigned char *data,
			    size_t datalen);

#endif 
