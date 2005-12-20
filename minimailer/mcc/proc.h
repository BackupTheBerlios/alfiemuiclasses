#ifndef _PROC_H
#define _PROC_H

#include <proto/exec.h>
#include <proto/socket.h>

#include <dos/dostags.h>

#include <sys/socket.h>
#include <netdb.h>

#include "lineread.h"

/***********************************************************************/

struct FREEDBS_StartMsg
{
    struct Message        link;
    ULONG                 cmd;
    ULONG                 flags;
    LONG                  err;
    struct FREEDBS_Handle *handle;
    struct TagItem        *attrs;
};

struct FREEDBS_Handle
{
    struct SignalSemaphore  sem;

    struct FREEDBS_StartMsg msg;
    struct MsgPort          reply;
    struct Process          *notiProc;
    int                     sig;

    struct Process          *proc;
    struct MsgPort          *port;

    LONG                    err;
    ULONG                   flags;
};

enum
{
    HANDLEFLG_Running = 1<<0,
    HANDLEFLG_Done    = 1<<1,
};

/***********************************************************************/

enum
{
    FREEDBV_Proc_Status_Init = 0,
    FREEDBV_Proc_Status_SkipHead,
    FREEDBV_Proc_Status_Result,
    FREEDBV_Proc_Status_Multi,
    FREEDBV_Proc_Status_Ignore,
};

/***********************************************************************/

#endif /* _PROC_H */
