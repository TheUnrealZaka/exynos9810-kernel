

#ifndef _UDA134X_H
#define _UDA134X_H

#include <sound/l3.h>

struct uda134x_platform_data {
	struct l3_pins l3;
	void (*power) (int);
	int model;
#define UDA134X_UDA1340 1
#define UDA134X_UDA1341 2
#define UDA134X_UDA1344 3
#define UDA134X_UDA1345 4
};

#endif 
