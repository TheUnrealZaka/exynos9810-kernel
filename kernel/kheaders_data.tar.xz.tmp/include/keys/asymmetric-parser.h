

#ifndef _KEYS_ASYMMETRIC_PARSER_H
#define _KEYS_ASYMMETRIC_PARSER_H


struct asymmetric_key_parser {
	struct list_head	link;
	struct module		*owner;
	const char		*name;

	
	int (*parse)(struct key_preparsed_payload *prep);
};

extern int register_asymmetric_key_parser(struct asymmetric_key_parser *);
extern void unregister_asymmetric_key_parser(struct asymmetric_key_parser *);

#endif 
