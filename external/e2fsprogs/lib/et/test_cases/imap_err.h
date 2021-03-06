
#include <et/com_err.h>

#define IMAP_IOERROR                             (-1904809472L)
#define IMAP_PERMISSION_DENIED                   (-1904809471L)
#define IMAP_QUOTA_EXCEEDED                      (-1904809470L)
#define IMAP_USERFLAG_EXHAUSTED                  (-1904809469L)
#define IMAP_MAILBOX_BADFORMAT                   (-1904809468L)
#define IMAP_MAILBOX_NOTSUPPORTED                (-1904809467L)
#define IMAP_MAILBOX_NONEXISTENT                 (-1904809466L)
#define IMAP_MAILBOX_EXISTS                      (-1904809465L)
#define IMAP_MAILBOX_BADNAME                     (-1904809464L)
#define IMAP_MAILBOX_POPLOCKED                   (-1904809463L)
#define IMAP_PARTITION_UNKNOWN                   (-1904809462L)
#define IMAP_INVALID_IDENTIFIER                  (-1904809461L)
#define IMAP_MESSAGE_CONTAINSNULL                (-1904809460L)
#define IMAP_MESSAGE_CONTAINSNL                  (-1904809459L)
#define IMAP_MESSAGE_CONTAINS8BIT                (-1904809458L)
#define IMAP_MESSAGE_BADHEADER                   (-1904809457L)
#define IMAP_MESSAGE_NOBLANKLINE                 (-1904809456L)
#define IMAP_QUOTAROOT_NONEXISTENT               (-1904809455L)
#define IMAP_UNRECOGNIZED_CHARSET                (-1904809454L)
#define IMAP_INVALID_USER                        (-1904809453L)
#define IMAP_INVALID_LOGIN                       (-1904809452L)
#define IMAP_ANONYMOUS_NOT_PERMITTED             (-1904809451L)
#define IMAP_UNSUPPORTED_QUOTA                   (-1904809450L)
#define IMAP_NO_OVERQUOTA                        (-1904809449L)
#define IMAP_NO_CLOSEQUOTA                       (-1904809448L)
#define IMAP_NO_MSGGONE                          (-1904809447L)
#define IMAP_NO_CHECKSEEN                        (-1904809446L)
#define IMAP_NO_CHECKPRESERVE                    (-1904809445L)
#define IMAP_BYE_LOGOUT                          (-1904809444L)
#define IMAP_OK_COMPLETED                        (-1904809443L)
extern const struct error_table et_imap_error_table;
extern void initialize_imap_error_table(void);

/* For compatibility with Heimdal */
extern void initialize_imap_error_table_r(struct et_list **list);

#define ERROR_TABLE_BASE_imap (-1904809472L)

/* for compatibility with older versions... */
#define init_imap_err_tbl initialize_imap_error_table
#define imap_err_base ERROR_TABLE_BASE_imap
