



#ifndef _ZCONF_H
#define _ZCONF_H




#ifndef MAX_MEM_LEVEL
#  define MAX_MEM_LEVEL 8
#endif


#ifndef MAX_WBITS
#  define MAX_WBITS   15 
#endif


#ifndef DEF_WBITS
#  define DEF_WBITS MAX_WBITS
#endif


#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif

                        

typedef unsigned char  Byte;  
typedef unsigned int   uInt;  
typedef unsigned long  uLong; 
typedef void     *voidp;

#endif 
