

/***********************************************************************/

struct taskMsg
{
    struct Message         link;

    struct SignalSemaphore *sem;
    struct Process	       *proc;

    struct MsgPort	       *resPort;

    UBYTE                  *email;
    UBYTE                  *realName;
    UBYTE                  *to;
    UBYTE                  *cc;
    UBYTE                  *subject;
    UBYTE                  *text;
    UBYTE                  *host;
    long                   hostPort;

    struct MinList	       *attaches;

    UBYTE                  *errBuf;
    int                    errBufLen;

    UBYTE                  *codesetName;

    struct MsgPort	       *procPort;
    ULONG                  err;

    ULONG                  flags;
};

/***********************************************************************/

struct infoMsg
{
    struct Message link;
    ULONG	       type;
};

enum
{
    MTYPE_Status,
    MTYPE_Total,
    MTYPE_Current,
};

struct statusMsg
{
    struct Message link;
    ULONG	       type;
    ULONG	       status;
    ULONG	       err;
};

struct totalMsg
{
    struct Message link;
    ULONG	       type;
    ULONG	       total;
};

struct currentMsg
{
    struct Message link;
    ULONG	       type;
    ULONG	       current;
};

/***********************************************************************/
