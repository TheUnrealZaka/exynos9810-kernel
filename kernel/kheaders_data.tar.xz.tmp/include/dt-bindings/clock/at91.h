

#ifndef _DT_BINDINGS_CLK_AT91_H
#define _DT_BINDINGS_CLK_AT91_H

#define AT91_PMC_MOSCS		0		
#define AT91_PMC_LOCKA		1		
#define AT91_PMC_LOCKB		2		
#define AT91_PMC_MCKRDY		3		
#define AT91_PMC_LOCKU		6		
#define AT91_PMC_PCKRDY(id)	(8 + (id))	
#define AT91_PMC_MOSCSELS	16		
#define AT91_PMC_MOSCRCS	17		
#define AT91_PMC_CFDEV		18		
#define AT91_PMC_GCKRDY		24		

#endif
