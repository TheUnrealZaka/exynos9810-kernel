


#ifndef _LINUX_I2C_PCA954X_H
#define _LINUX_I2C_PCA954X_H




struct pca954x_platform_mode {
	int		adap_id;
	unsigned int	deselect_on_exit:1;
	unsigned int	class;
};


struct pca954x_platform_data {
	struct pca954x_platform_mode *modes;
	int num_modes;
};

#endif 
