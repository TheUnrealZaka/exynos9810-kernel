

#ifndef __SIMPLE_CARD_H
#define __SIMPLE_CARD_H

#include <sound/soc.h>
#include <sound/simple_card_utils.h>

struct asoc_simple_card_info {
	const char *name;
	const char *card;
	const char *codec;
	const char *platform;

	unsigned int daifmt;
	struct asoc_simple_dai cpu_dai;
	struct asoc_simple_dai codec_dai;
};

#endif 
