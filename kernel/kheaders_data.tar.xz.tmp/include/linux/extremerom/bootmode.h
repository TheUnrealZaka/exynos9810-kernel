

#ifndef EXTREMEROM_BOOTMODE_H
#define EXTREMEROM_BOOTMODE_H

enum extremerom_bootmode {
	BOOTMODE_NORMAL,
	BOOTMODE_CHARGER,
	BOOTMODE_RECOVERY,
	BOOTMODE_UNKNOWN
};

enum extremerom_bootmode get_extremerom_bootmode(void);

#endif

