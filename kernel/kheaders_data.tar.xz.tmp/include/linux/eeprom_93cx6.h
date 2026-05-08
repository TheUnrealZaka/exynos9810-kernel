




#define PCI_EEPROM_WIDTH_93C46	6
#define PCI_EEPROM_WIDTH_93C56	8
#define PCI_EEPROM_WIDTH_93C66	8
#define PCI_EEPROM_WIDTH_93C86	8
#define PCI_EEPROM_WIDTH_OPCODE	3
#define PCI_EEPROM_WRITE_OPCODE	0x05
#define PCI_EEPROM_ERASE_OPCODE 0x07
#define PCI_EEPROM_READ_OPCODE	0x06
#define PCI_EEPROM_EWDS_OPCODE	0x10
#define PCI_EEPROM_EWEN_OPCODE	0x13


struct eeprom_93cx6 {
	void *data;

	void (*register_read)(struct eeprom_93cx6 *eeprom);
	void (*register_write)(struct eeprom_93cx6 *eeprom);

	int width;

	char drive_data;
	char reg_data_in;
	char reg_data_out;
	char reg_data_clock;
	char reg_chip_select;
};

extern void eeprom_93cx6_read(struct eeprom_93cx6 *eeprom,
	const u8 word, u16 *data);
extern void eeprom_93cx6_multiread(struct eeprom_93cx6 *eeprom,
	const u8 word, __le16 *data, const u16 words);
extern void eeprom_93cx6_readb(struct eeprom_93cx6 *eeprom,
	const u8 byte, u8 *data);
extern void eeprom_93cx6_multireadb(struct eeprom_93cx6 *eeprom,
	const u8 byte, u8 *data, const u16 bytes);

extern void eeprom_93cx6_wren(struct eeprom_93cx6 *eeprom, bool enable);

extern void eeprom_93cx6_write(struct eeprom_93cx6 *eeprom,
			       u8 addr, u16 data);
