

struct m48t86_ops
{
	void (*writebyte)(unsigned char value, unsigned long addr);
	unsigned char (*readbyte)(unsigned long addr);
};
