




#include <linux/sunrpc/gss_api.h>

#define SIZEOF_INT 4


#define G_BAD_SERVICE_NAME                       (-2045022976L)
#define G_BAD_STRING_UID                         (-2045022975L)
#define G_NOUSER                                 (-2045022974L)
#define G_VALIDATE_FAILED                        (-2045022973L)
#define G_BUFFER_ALLOC                           (-2045022972L)
#define G_BAD_MSG_CTX                            (-2045022971L)
#define G_WRONG_SIZE                             (-2045022970L)
#define G_BAD_USAGE                              (-2045022969L)
#define G_UNKNOWN_QOP                            (-2045022968L)
#define G_NO_HOSTNAME                            (-2045022967L)
#define G_BAD_HOSTNAME                           (-2045022966L)
#define G_WRONG_MECH                             (-2045022965L)
#define G_BAD_TOK_HEADER                         (-2045022964L)
#define G_BAD_DIRECTION                          (-2045022963L)
#define G_TOK_TRUNC                              (-2045022962L)
#define G_REFLECT                                (-2045022961L)
#define G_WRONG_TOKID                            (-2045022960L)

#define g_OID_equal(o1,o2) \
   (((o1)->len == (o2)->len) && \
    (memcmp((o1)->data,(o2)->data,(int) (o1)->len) == 0))

u32 g_verify_token_header(
     struct xdr_netobj *mech,
     int *body_size,
     unsigned char **buf_in,
     int toksize);

int g_token_size(
     struct xdr_netobj *mech,
     unsigned int body_size);

void g_make_token_header(
     struct xdr_netobj *mech,
     int body_size,
     unsigned char **buf);
