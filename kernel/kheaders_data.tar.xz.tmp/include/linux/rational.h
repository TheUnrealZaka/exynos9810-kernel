

#ifndef _LINUX_RATIONAL_H
#define _LINUX_RATIONAL_H

void rational_best_approximation(
	unsigned long given_numerator, unsigned long given_denominator,
	unsigned long max_numerator, unsigned long max_denominator,
	unsigned long *best_numerator, unsigned long *best_denominator);

#endif 
