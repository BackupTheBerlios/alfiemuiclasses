
#ifndef _INSTANCE_H
#define _INSTANCE_H

/***********************************************************************/

#include <proto/socket.h>

#include <dos/dostags.h>

#include <sys/socket.h>
#include <netdb.h>

#include "lineread.h"

/***********************************************************************/

struct taskMsg
{
    struct Message link;

    struct data    *data;

    STRPTR         email;
    STRPTR         realName;
    STRPTR         to;
    STRPTR         cc;
    STRPTR         subject;
    STRPTR         text;
    STRPTR         host;
    long           hostPort;

    STRPTR         errBuf;
    int            errBufLen;

    STRPTR         codesetName;

    ULONG          flags;

    ULONG          err;
};

/***********************************************************************/

#endif /* _INSTANCE_H */
