

#ifndef __MTD_NAND_ECC_H__
#define __MTD_NAND_ECC_H__

struct mtd_info;


void __nand_calculate_ecc(const u_char *dat, unsigned int eccsize,
				u_char *ecc_code);


int nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code);


int __nand_correct_data(u_char *dat, u_char *read_ecc, u_char *calc_ecc,
			unsigned int eccsize);


int nand_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc);

#endif 
