

#ifndef __MATH_EMU_OP_8_H__
#define __MATH_EMU_OP_8_H__


#define _FP_FRAC_DECL_8(X)	_FP_W_TYPE X##_f[8]
#define _FP_FRAC_HIGH_8(X)	(X##_f[7])
#define _FP_FRAC_LOW_8(X)	(X##_f[0])
#define _FP_FRAC_WORD_8(X,w)	(X##_f[w])

#define _FP_FRAC_SLL_8(X,N)						\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _up = (N) % _FP_W_TYPE_SIZE;					\
    _down = _FP_W_TYPE_SIZE - _up;					\
    if (!_up)								\
      for (_i = 7; _i >= _skip; --_i)					\
	X##_f[_i] = X##_f[_i-_skip];					\
    else								\
      {									\
	for (_i = 7; _i > _skip; --_i)					\
	  X##_f[_i] = X##_f[_i-_skip] << _up				\
		      | X##_f[_i-_skip-1] >> _down;			\
	X##_f[_i--] = X##_f[0] << _up; 					\
      }									\
    for (; _i >= 0; --_i)						\
      X##_f[_i] = 0;							\
  } while (0)

#define _FP_FRAC_SRL_8(X,N)						\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _down = (N) % _FP_W_TYPE_SIZE;					\
    _up = _FP_W_TYPE_SIZE - _down;					\
    if (!_down)								\
      for (_i = 0; _i <= 7-_skip; ++_i)					\
	X##_f[_i] = X##_f[_i+_skip];					\
    else								\
      {									\
	for (_i = 0; _i < 7-_skip; ++_i)				\
	  X##_f[_i] = X##_f[_i+_skip] >> _down				\
		      | X##_f[_i+_skip+1] << _up;			\
	X##_f[_i++] = X##_f[7] >> _down;				\
      }									\
    for (; _i < 8; ++_i)						\
      X##_f[_i] = 0;							\
  } while (0)



#define _FP_FRAC_SRS_8(X,N,size)					\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _FP_W_TYPE _s;							\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _down = (N) % _FP_W_TYPE_SIZE;					\
    _up = _FP_W_TYPE_SIZE - _down;					\
    for (_s = _i = 0; _i < _skip; ++_i)					\
      _s |= X##_f[_i];							\
    _s |= X##_f[_i] << _up;						\
				\
    if (!_down)								\
      for (_i = 0; _i <= 7-_skip; ++_i)					\
	X##_f[_i] = X##_f[_i+_skip];					\
    else								\
      {									\
	for (_i = 0; _i < 7-_skip; ++_i)				\
	  X##_f[_i] = X##_f[_i+_skip] >> _down				\
		      | X##_f[_i+_skip+1] << _up;			\
	X##_f[_i++] = X##_f[7] >> _down;				\
      }									\
    for (; _i < 8; ++_i)						\
      X##_f[_i] = 0;							\
    	\
    X##_f[0] |= (_s != 0);						\
  } while (0)

#endif
