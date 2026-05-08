#ifndef __SOUND_TEA6330T_H
#define __SOUND_TEA6330T_H



#include <sound/i2c.h>		

int snd_tea6330t_detect(struct snd_i2c_bus *bus, int equalizer);
int snd_tea6330t_update_mixer(struct snd_card *card, struct snd_i2c_bus *bus,
			      int equalizer, int fader);

#endif 
