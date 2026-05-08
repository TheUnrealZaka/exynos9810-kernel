

#ifndef _S5P_MFC_HWFC_H
#define _S5P_MFC_HWFC_H

#include <linux/types.h>

#define HWFC_ERR_NONE			0
#define HWFC_ERR_TSMUX			1
#define HWFC_ERR_MFC			2
#define HWFC_ERR_MFC_NOT_PREPARED	3
#define HWFC_ERR_MFC_TIMEOUT		4
#define HWFC_ERR_MFC_NOT_ENABLED	5


struct encoding_param {
	u64 time_stamp;
};


#ifdef CONFIG_VIDEO_EXYNOS_MFC
int s5p_mfc_hwfc_encode(int buf_index, int job_id, struct encoding_param *param);
#else
static inline int s5p_mfc_hwfc_encode(int buf_index, int job_id, struct encoding_param *param)
{
	return -HWFC_ERR_MFC_NOT_ENABLED;
}
#endif

#endif 
