

#ifndef __SOUND_MAX98088_PDATA_H__
#define __SOUND_MAX98088_PDATA_H__


struct max98088_eq_cfg {
       const char *name;
       unsigned int rate;
       u16 band1[5];
       u16 band2[5];
       u16 band3[5];
       u16 band4[5];
       u16 band5[5];
};


struct max98088_pdata {

       
       struct max98088_eq_cfg *eq_cfg;
       unsigned int eq_cfgcnt;

       
       
       unsigned int receiver_mode:1;

       
       unsigned int digmic_left_mode:1;
       unsigned int digmic_right_mode:1;

};

#endif
