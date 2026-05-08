

#ifndef __XEN_PUBLIC_IO_TPMIF_H__
#define __XEN_PUBLIC_IO_TPMIF_H__



enum vtpm_shared_page_state {
	VTPM_STATE_IDLE,         
	VTPM_STATE_SUBMIT,       
	VTPM_STATE_FINISH,       
	VTPM_STATE_CANCEL,       
};



struct vtpm_shared_page {
	uint32_t length;         

	uint8_t state;           
	uint8_t locality;        
	uint8_t pad;

	uint8_t nr_extra_pages;  
	uint32_t extra_pages[0]; 
};

#endif
