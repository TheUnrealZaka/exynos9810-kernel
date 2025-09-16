
#ifndef __ASM_COMPILER_H
#define __ASM_COMPILER_H


#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#endif	
