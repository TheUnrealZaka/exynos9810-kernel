


#include <linux/if_ether.h>


enum tveeprom_audio_processor {
	TVEEPROM_AUDPROC_NONE,
	TVEEPROM_AUDPROC_INTERNAL,
	TVEEPROM_AUDPROC_MSP,
	TVEEPROM_AUDPROC_OTHER,
};


struct tveeprom {
	u32 has_radio;
	u32 has_ir;
	u32 has_MAC_address;

	u32 tuner_type;
	u32 tuner_formats;
	u32 tuner_hauppauge_model;

	u32 tuner2_type;
	u32 tuner2_formats;
	u32 tuner2_hauppauge_model;

	u32 audio_processor;
	u32 decoder_processor;

	u32 model;
	u32 revision;
	u32 serial_number;
	char rev_str[5];
	u8 MAC_address[ETH_ALEN];
};


void tveeprom_hauppauge_analog(struct i2c_client *c, struct tveeprom *tvee,
			       unsigned char *eeprom_data);


int tveeprom_read(struct i2c_client *c, unsigned char *eedata, int len);
