

#ifndef AK881X_H
#define AK881X_H

#define AK881X_IF_MODE_MASK	(3 << 0)
#define AK881X_IF_MODE_BT656	(0 << 0)
#define AK881X_IF_MODE_MASTER	(1 << 0)
#define AK881X_IF_MODE_SLAVE	(2 << 0)
#define AK881X_FIELD		(1 << 2)
#define AK881X_COMPONENT	(1 << 3)

struct ak881x_pdata {
	unsigned long flags;
};

#endif
