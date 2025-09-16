

struct gpio_desc;

struct eeprom_93xx46_platform_data {
	unsigned char	flags;
#define EE_ADDR8	0x01		
#define EE_ADDR16	0x02		
#define EE_READONLY	0x08		

	unsigned int	quirks;

#define EEPROM_93XX46_QUIRK_SINGLE_WORD_READ		(1 << 0)

#define EEPROM_93XX46_QUIRK_INSTRUCTION_LENGTH		(1 << 1)

#define EEPROM_93XX46_QUIRK_EXTRA_READ_CYCLE		BIT(2)

	
	void (*prepare)(void *);
	void (*finish)(void *);
	struct gpio_desc *select;
};
