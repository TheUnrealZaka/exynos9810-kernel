

#ifndef __XEN_PUBLIC_NMI_H__
#define __XEN_PUBLIC_NMI_H__

#include <xen/interface/xen.h>


 
#define _XEN_NMIREASON_io_error     0
#define XEN_NMIREASON_io_error      (1UL << _XEN_NMIREASON_io_error)
 
#define _XEN_NMIREASON_pci_serr     1
#define XEN_NMIREASON_pci_serr      (1UL << _XEN_NMIREASON_pci_serr)
 
#define _XEN_NMIREASON_unknown      2
#define XEN_NMIREASON_unknown       (1UL << _XEN_NMIREASON_unknown)




#define XENNMI_register_callback   0
struct xennmi_callback {
    unsigned long handler_address;
    unsigned long pad;
};
DEFINE_GUEST_HANDLE_STRUCT(xennmi_callback);


#define XENNMI_unregister_callback 1

#endif 
