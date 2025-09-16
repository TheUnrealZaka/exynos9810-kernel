

#ifndef MXM_WMI_H
#define MXM_WMI_H


#define MXM_MXDS_ADAPTER_0 0x0
#define MXM_MXDS_ADAPTER_1 0x0

#define MXM_MXDS_ADAPTER_IGD 0x10
int mxm_wmi_call_mxds(int adapter);
int mxm_wmi_call_mxmx(int adapter);
bool mxm_wmi_supported(void);

#endif
