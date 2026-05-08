

#ifndef __ISDNHDLC_H__
#define __ISDNHDLC_H__

struct isdnhdlc_vars {
	int bit_shift;
	int hdlc_bits1;
	int data_bits;
	int ffbit_shift;	
	int state;
	int dstpos;

	u16 crc;

	u8 cbin;
	u8 shift_reg;
	u8 ffvalue;

	
	u32 data_received:1;
	
	u32 dchannel:1;
	
	u32 do_adapt56:1;
	
	u32 do_closing:1;
	
	u32 do_bitreverse:1;
};


#define HDLC_56KBIT	0x01
#define HDLC_DCHANNEL	0x02
#define HDLC_BITREVERSE	0x04


#define HDLC_FRAMING_ERROR     1
#define HDLC_CRC_ERROR         2
#define HDLC_LENGTH_ERROR      3

extern void	isdnhdlc_rcv_init(struct isdnhdlc_vars *hdlc, u32 features);

extern int	isdnhdlc_decode(struct isdnhdlc_vars *hdlc, const u8 *src,
			int slen, int *count, u8 *dst, int dsize);

extern void	isdnhdlc_out_init(struct isdnhdlc_vars *hdlc, u32 features);

extern int	isdnhdlc_encode(struct isdnhdlc_vars *hdlc, const u8 *src,
			u16 slen, int *count, u8 *dst, int dsize);

#endif 
