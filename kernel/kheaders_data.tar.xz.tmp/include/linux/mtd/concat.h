

#ifndef MTD_CONCAT_H
#define MTD_CONCAT_H


struct mtd_info *mtd_concat_create(
    struct mtd_info *subdev[],  
    int num_devs,               
    const char *name);          

void mtd_concat_destroy(struct mtd_info *mtd);

#endif

