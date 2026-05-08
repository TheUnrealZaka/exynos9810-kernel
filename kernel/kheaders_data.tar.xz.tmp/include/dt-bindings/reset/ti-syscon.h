

#ifndef __DT_BINDINGS_RESET_TI_SYSCON_H__
#define __DT_BINDINGS_RESET_TI_SYSCON_H__


#define ASSERT_NONE	(1 << 0)
#define DEASSERT_NONE	(1 << 1)
#define STATUS_NONE	(1 << 2)


#define ASSERT_SET	(1 << 3)
#define DEASSERT_SET	(1 << 4)
#define STATUS_SET	(1 << 5)


#define ASSERT_CLEAR	(0 << 3)
#define DEASSERT_CLEAR	(0 << 4)
#define STATUS_CLEAR	(0 << 5)

#endif
