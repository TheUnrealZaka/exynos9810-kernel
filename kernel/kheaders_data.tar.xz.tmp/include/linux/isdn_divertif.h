
#ifndef _LINUX_ISDN_DIVERTIF_H
#define _LINUX_ISDN_DIVERTIF_H

#include <linux/isdnif.h>
#include <linux/types.h>
#include <uapi/linux/isdn_divertif.h>



 
typedef struct
  { ulong if_magic; 
    int cmd; 
    int (*stat_callback)(isdn_ctrl *); 
    int (*ll_cmd)(isdn_ctrl *); 
    char * (*drv_to_name)(int); 
    int (*name_to_drv)(char *); 
  } isdn_divert_if;




extern int DIVERT_REG_NAME(isdn_divert_if *);
#endif 
