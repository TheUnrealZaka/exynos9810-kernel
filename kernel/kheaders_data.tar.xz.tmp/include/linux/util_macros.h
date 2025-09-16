#ifndef _LINUX_HELPER_MACROS_H_
#define _LINUX_HELPER_MACROS_H_

#define __find_closest(x, a, as, op)					\
({									\
	typeof(as) __fc_i, __fc_as = (as) - 1;				\
	typeof(x) __fc_x = (x);						\
	typeof(*a) const *__fc_a = (a);					\
	for (__fc_i = 0; __fc_i < __fc_as; __fc_i++) {			\
		if (__fc_x op DIV_ROUND_CLOSEST(__fc_a[__fc_i] +	\
						__fc_a[__fc_i + 1], 2))	\
			break;						\
	}								\
	(__fc_i);							\
})


#define find_closest(x, a, as) __find_closest(x, a, as, <=)


#define find_closest_descending(x, a, as) __find_closest(x, a, as, >=)

#endif
