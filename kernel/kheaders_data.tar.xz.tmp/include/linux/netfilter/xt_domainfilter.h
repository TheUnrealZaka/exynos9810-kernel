

#ifndef _XT_DOMAINFILTER_MATCH_H
#define _XT_DOMAINFILTER_MATCH_H

enum {
	XT_DOMAINFILTER_WHITE    = 1 << 0,
	XT_DOMAINFILTER_BLACK    = 1 << 1,

	XT_DOMAINFILTER_NAME_LEN  = 256, // lenght of a domain name
};

struct xt_domainfilter_match_info {
	char domain_name[XT_DOMAINFILTER_NAME_LEN];
	__u8 flags;
};

#endif //_XT_DOMAINFILTER_MATCH_H
