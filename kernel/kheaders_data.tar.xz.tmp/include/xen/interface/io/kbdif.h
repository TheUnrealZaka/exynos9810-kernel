

#ifndef __XEN_PUBLIC_IO_KBDIF_H__
#define __XEN_PUBLIC_IO_KBDIF_H__






#define XENKBD_TYPE_MOTION  1


#define XENKBD_TYPE_KEY     3

#define XENKBD_TYPE_POS     4

struct xenkbd_motion {
	uint8_t type;		
	int32_t rel_x;		
	int32_t rel_y;		
	int32_t rel_z;		
};

struct xenkbd_key {
	uint8_t type;		
	uint8_t pressed;	
	uint32_t keycode;	
};

struct xenkbd_position {
	uint8_t type;		
	int32_t abs_x;		
	int32_t abs_y;		
	int32_t rel_z;		
};

#define XENKBD_IN_EVENT_SIZE 40

union xenkbd_in_event {
	uint8_t type;
	struct xenkbd_motion motion;
	struct xenkbd_key key;
	struct xenkbd_position pos;
	char pad[XENKBD_IN_EVENT_SIZE];
};





#define XENKBD_OUT_EVENT_SIZE 40

union xenkbd_out_event {
	uint8_t type;
	char pad[XENKBD_OUT_EVENT_SIZE];
};



#define XENKBD_IN_RING_SIZE 2048
#define XENKBD_IN_RING_LEN (XENKBD_IN_RING_SIZE / XENKBD_IN_EVENT_SIZE)
#define XENKBD_IN_RING_OFFS 1024
#define XENKBD_IN_RING(page) \
	((union xenkbd_in_event *)((char *)(page) + XENKBD_IN_RING_OFFS))
#define XENKBD_IN_RING_REF(page, idx) \
	(XENKBD_IN_RING((page))[(idx) % XENKBD_IN_RING_LEN])

#define XENKBD_OUT_RING_SIZE 1024
#define XENKBD_OUT_RING_LEN (XENKBD_OUT_RING_SIZE / XENKBD_OUT_EVENT_SIZE)
#define XENKBD_OUT_RING_OFFS (XENKBD_IN_RING_OFFS + XENKBD_IN_RING_SIZE)
#define XENKBD_OUT_RING(page) \
	((union xenkbd_out_event *)((char *)(page) + XENKBD_OUT_RING_OFFS))
#define XENKBD_OUT_RING_REF(page, idx) \
	(XENKBD_OUT_RING((page))[(idx) % XENKBD_OUT_RING_LEN])

struct xenkbd_page {
	uint32_t in_cons, in_prod;
	uint32_t out_cons, out_prod;
};

#endif
