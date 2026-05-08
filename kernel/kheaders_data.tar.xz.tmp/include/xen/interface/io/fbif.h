

#ifndef __XEN_PUBLIC_IO_FBIF_H__
#define __XEN_PUBLIC_IO_FBIF_H__







#define XENFB_TYPE_UPDATE 2

struct xenfb_update {
	uint8_t type;		
	int32_t x;		
	int32_t y;		
	int32_t width;		
	int32_t height;		
};


#define XENFB_TYPE_RESIZE 3

struct xenfb_resize {
	uint8_t type;		
	int32_t width;		
	int32_t height;		
	int32_t stride;		
	int32_t depth;		
	int32_t offset;		
};

#define XENFB_OUT_EVENT_SIZE 40

union xenfb_out_event {
	uint8_t type;
	struct xenfb_update update;
	struct xenfb_resize resize;
	char pad[XENFB_OUT_EVENT_SIZE];
};





#define XENFB_IN_EVENT_SIZE 40

union xenfb_in_event {
	uint8_t type;
	char pad[XENFB_IN_EVENT_SIZE];
};



#define XENFB_IN_RING_SIZE 1024
#define XENFB_IN_RING_LEN (XENFB_IN_RING_SIZE / XENFB_IN_EVENT_SIZE)
#define XENFB_IN_RING_OFFS 1024
#define XENFB_IN_RING(page) \
	((union xenfb_in_event *)((char *)(page) + XENFB_IN_RING_OFFS))
#define XENFB_IN_RING_REF(page, idx) \
	(XENFB_IN_RING((page))[(idx) % XENFB_IN_RING_LEN])

#define XENFB_OUT_RING_SIZE 2048
#define XENFB_OUT_RING_LEN (XENFB_OUT_RING_SIZE / XENFB_OUT_EVENT_SIZE)
#define XENFB_OUT_RING_OFFS (XENFB_IN_RING_OFFS + XENFB_IN_RING_SIZE)
#define XENFB_OUT_RING(page) \
	((union xenfb_out_event *)((char *)(page) + XENFB_OUT_RING_OFFS))
#define XENFB_OUT_RING_REF(page, idx) \
	(XENFB_OUT_RING((page))[(idx) % XENFB_OUT_RING_LEN])

struct xenfb_page {
	uint32_t in_cons, in_prod;
	uint32_t out_cons, out_prod;

	int32_t width;          
	int32_t height;         
	uint32_t line_length;   
	uint32_t mem_length;    
	uint8_t depth;          

	
	unsigned long pd[256];
};


#ifdef __KERNEL__
#define XENFB_WIDTH 800
#define XENFB_HEIGHT 600
#define XENFB_DEPTH 32
#endif

#endif
