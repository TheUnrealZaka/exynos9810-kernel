

#ifndef _CFAG12864B_H_
#define _CFAG12864B_H_

#define CFAG12864B_WIDTH	(128)
#define CFAG12864B_HEIGHT	(64)
#define CFAG12864B_CONTROLLERS	(2)
#define CFAG12864B_PAGES	(8)
#define CFAG12864B_ADDRESSES	(64)
#define CFAG12864B_SIZE		((CFAG12864B_CONTROLLERS) * \
				(CFAG12864B_PAGES) * \
				(CFAG12864B_ADDRESSES))


extern unsigned char * cfag12864b_buffer;


extern unsigned int cfag12864b_getrate(void);


extern unsigned char cfag12864b_enable(void);


extern void cfag12864b_disable(void);


extern unsigned char cfag12864b_isenabled(void);


extern unsigned char cfag12864b_isinited(void);

#endif 

