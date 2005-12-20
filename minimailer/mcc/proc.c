
#include "class.h"
#include "http.h"
#include "MiniMailer.mcc_rev.h"

#include <proto/socket.h>
#ifdef __MORPHOS__
#include <net/socketbasetags.h>
#else
#include <bsdsocket/socketbasetags.h>
#include <sys/socket.h>
#endif
#include <netdb.h>

#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

static void
notifyStatus(struct MsgPort *port,ULONG status,ULONG err)
{
    struct statusMsg *sm;

    if (sm = allocArbitrateVecPooled(sizeof(struct statusMsg)))
    {
        sm->type   = MTYPE_Status;
        sm->status = status;
        sm->err	   = err;

        PutMsg(port,(struct Message *)sm);
    }
}

/***********************************************************************/

static void
notifyTotal(struct MsgPort *port,ULONG total)
{
    struct totalMsg *tm;

    if (tm = allocArbitrateVecPooled(sizeof(struct totalMsg)))
    {
        tm->type  = MTYPE_Total;
        tm->total = total;

        PutMsg(port,(struct Message *)tm);
    }
}

/***********************************************************************/

static void
notifyCurrent(struct MsgPort *port,ULONG current)
{
    struct currentMsg *cm;

    if (cm = allocArbitrateVecPooled(sizeof(struct currentMsg)))
    {
        cm->type    = MTYPE_Current;
        cm->current = current;

        PutMsg(port,(struct Message *)cm);
    }
}

/***********************************************************************/

static void
getTime(struct DateStamp *date,UBYTE *buf)
{
    ULONG secs;

    secs = date->ds_Days*24*60*60 + date->ds_Minute*60 + date->ds_Tick/50;
    sprintf(buf,"%08lx",secs);
}

/***********************************************************************/

static UBYTE *months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static UBYTE *days[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

static void
getGMTTime(struct DateStamp *date,UBYTE *buf)
{
    struct Locale    *loc;
    struct ClockData cd;
    UBYTE            gmtBuf[16];
    ULONG            secs;
    LONG             gmt;

    if (loc = OpenLocale(NULL))
    {
        gmt = loc->loc_GMTOffset;
        CloseLocale(loc);
    }
    else gmt = 60;

    if (gmt>=0) sprintf(gmtBuf,"-%02ld%02ld",gmt/60,gmt%60);
    else sprintf(gmtBuf,"+%02ld%02ld",-gmt/60,-gmt%60);

    secs = date->ds_Days*24*60*60 + date->ds_Minute*60 + date->ds_Tick/50;
    Amiga2Date(secs,&cd);

    sprintf(buf,"%s, %ld %s %ld %02ld:%02ld:%02ld %s",days[cd.wday],(ULONG)cd.mday,months[cd.month-1],(ULONG)cd.year,(ULONG)cd.hour,(ULONG)cd.min,(ULONG)cd.sec,gmtBuf);
}

/***********************************************************************/

static ULONG
sen(struct Library *SocketBase,int sock,UBYTE *buf,int bufSize)
{
    if (send(sock,buf,bufSize,0)<0)
        return Msg_ErrSend;

    return 0;
}

/***********************************************************************/

struct args
{
    ULONG  *code;
    UBYTE  *rest;
};

static ULONG
rec(struct Library *SocketBase,int sock,UBYTE *buf,int bufSize,UBYTE *errBuf,int errBufLen)
{
    struct RDArgs  ra;
    struct args    args;
    UBYTE          rabuf[256];

    if (recv(sock,buf,bufSize,0)<=0)
        return Msg_ErrRecv;

    ra.RDA_Source.CS_Buffer  = buf;
    ra.RDA_Source.CS_Length  = strlen(buf);
    ra.RDA_Source.CS_CurChr  = 0;
    ra.RDA_DAList            = NULL;
    ra.RDA_Buffer            = rabuf;
    ra.RDA_BufSiz            = sizeof(rabuf);
    ra.RDA_Flags             = RDAF_NOALLOC|RDAF_NOPROMPT;

    if (!ReadArgs("CODE/A/N,REST/A/F",(LONG *)&args,&ra))
        return Msg_ProtoError;

    if (*args.code>=400)
    {
        int i;

        i = snprintf(errBuf,errBufLen,"%ld %s",*args.code,args.rest,errBufLen);
        if (i) errBuf[i-1] = 0;

        return Msg_SeverError;
    }

    return 0;
}

/***********************************************************************/

struct mime
{
    UBYTE *name;
    UBYTE *mime;
    UBYTE  flow;
};

static struct mime mimes[] =
{
    "8vsx",                "audio/x-8vsx",                  0,
    "ai",                  "application/postscript",	    0,
    "aif",                 "audio/aiff",                    0,
    "aifc",                "audio/aiff",                    0,
    "aiff",                "audio/aiff",                    0,
    "au",                  "audio/basic",                   0,
    "avi",                 "video/x-msvideo",               0,
    "bin",                 "application/octet-stream",      0,
    "class",               "application/octet-stream",      0,
    "dms",                 "application/octet-stream",      0,
    "doc",                 "text/plain",		            1,
    "dvi",	               "application/x-dvi",             0,
    "eps",                 "application/postscript",        0,
    "exe",                 "application/octet-stream",      0,
    "gif",                 "image/gif",                     0,
    "gsd",                 "audio/x-gsm",                   0,
    "gsm",                 "audio/x-gsm",                   0,
    "gtar",                "application/octet-stream",      0,
    "guide",               "text/x-aguide",                 0,
    "gz",                  "application/octet-stream",      0,
    "ham",                 "image/x-iff",                   0,
    "ham8",                "image/x-iff",                   0,
    "hdf",                 "application/x-hdf",             0,
    "hml",                 "text/html",                     1,
    "htm",                 "text/html",                     1,
    "html",                "text/html",                     1,
    "ief",                 "image/ief",                     0,
    "iff",                 "image/x-iff",                   0,
    "ilbm",                "image/x-iff",                   0,
    "jfif",                "image/jpeg",                    0,
    "jpe",                 "image/jpeg",                    0,
    "jpeg",                "image/jpeg",                    0,
    "jpg",                 "image/jpeg",                    0,
    "latex",               "application/x-latex",           0,
    "lha",                 "application/octet-stream",      0,
    "lzh",                 "application/octet-stream",      0,
    "mid",                 "audio/midi",                    0,
    "midi",                "audio/midi",                    0,
    "mod",                 "audio/x-mod",                   0,
    "mov",                 "video/quicktime",               0,
    "movie",               "video/x-sgi-movie",             0,
    "mpe",                 "video/mpeg",                    0,
    "mpeg",                "video/mpeg",                    0,
    "mpg",                 "video/mpeg",                    0,
    "muad",                "audio/maud",                    0,
    "pbm",                 "image/x-portable-bitmap",       0,
    "pdf",  	           "application/pdf",               0,
    "pgm",                 "image/x-portable-graymap",      0,
    "pnm",                 "image/x-portable-anymap",       0,
    "ppm",                 "image/x-portable-pixmap",       0,
    "ps",                  "application/postscript",        0,
    "qt",                  "video/quicktime",               0,
    "ra",                  "audio/x-ra",                    0,
    "rgb",                 "image/x-rgb",                   0,
    "shtml",               "text/html",                     1,
    "snd",                 "audio/basic",                   0,
    "svx",                 "audio/x-8vsx",                  0,
    "tar",                 "application/octet-stream",      0,
    "tif",                 "image/tiff",                    0,
    "tiff",                "image/tiff",                    0,
    "txt",                 "text/plain",                    1,
    "voc",                 "audio/voc",                     0,
    "wav",                 "audio/wav",                     0,
    "xbm",                 "image/x-xbitmap",               0,
    "xpm",                 "image/x-xpixmap",               0,
    "xwd",                 "image/x-xwindowdump",           0,
    "zip",                 "application/octet-stream",      0
};

#define MIMENUM (sizeof(mimes)/sizeof(struct mime))

static struct mime *
getMime(UBYTE *name)
{
    register UBYTE *s, *d;

    for (d = NULL, s = name; *s; s++)
	if (*s=='.') d = s+1;

    if (d && *d)
    {
        register int cond, mid, low, high;
    	
        low  = 0;
        high = MIMENUM-1;

        while (low<=high)
        {
            mid = (low+high)>>1;
            if (!(cond = stricmp(name,mimes[mid].name))) return mimes+mid;
            else
                if (cond<0) high = mid-1;
                else low = mid+1;
        }
    }

    return mimes+13;
}

/***********************************************************************/

static ULONG
attachesSize(struct MinList *attaches)
{
    struct FileInfoBlock *fib;
    ULONG		         size = 0;

    if (fib = AllocDosObject(DOS_FIB,NULL))
    {
        register struct MUIS_MiniMailer_Attach *mstate, *succ;

    	for (mstate = (struct MUIS_MiniMailer_Attach *)attaches->mlh_Head; succ = (struct MUIS_MiniMailer_Attach *)mstate->link.mln_Succ; mstate = succ)
        {
    	    BPTR lock;

    	    if (lock = Lock(mstate->file,SHARED_LOCK))
            {
            	if (Examine(lock,fib) && (fib->fib_DirEntryType<0) && (fib->fib_Size>0))
	    	    size += fib->fib_Size;

                UnLock(lock);
	        }
    	}

    	FreeDosObject(DOS_FIB,fib);
    }

    return (size+2)/3*4;
}

/***********************************************************************/

struct outData
{
    struct Library *SocketBase;
    int		       sock;
    struct MsgPort *port;
    ULONG	       *current;
};

/***********************************************************************/

static ULONG
outFunc(APTR data,UBYTE *buf,int bufLen)
{
    struct outData *odata = data;

    *odata->current += bufLen;

    notifyCurrent(odata->port,*odata->current);

    return sen(odata->SocketBase,odata->sock,buf,bufLen);
}

/***********************************************************************/

static ULONG
sendAttaches(struct Library *SocketBase,int sock,struct MinList *attaches,UBYTE *boundary,struct MsgPort *port,ULONG *current)
{
    register struct MUIS_MiniMailer_Attach *mstate, *succ;
    struct outData                         odata;
    ULONG		                           err = 0, r;

    odata.SocketBase = SocketBase;
    odata.sock	     = sock;
    odata.port 	     = port;
    odata.current    = current;

    for (mstate = (struct MUIS_MiniMailer_Attach *)attaches->mlh_Head; succ = (struct MUIS_MiniMailer_Attach *)mstate->link.mln_Succ; mstate = succ)
    {
        UBYTE buf[256];

        sprintf(buf,"\
--%s\r\n\
Content-Type: %s; name=\"%s\"\r\n\
Content-Disposition: attachment; filename=\"%s\"\r\n\
Content-Transfer-Encoding: base64\r\n\r\n",
boundary,
getMime(mstate->file)->mime,
FilePart(mstate->file),
FilePart(mstate->file));

    	if (err = sen(SocketBase,sock,buf,strlen(buf))) goto done;

    	r = encodeB64(&odata,outFunc,mstate->file);
        if (r!=B64_OK)
        {
    	    err = (r==B64_IoErr) ? Msg_NoMem : Msg_IoErr;
            goto done;
	    }

    	if (err = sen(SocketBase,sock,"\r\n",2)) goto done;
    }

done:

    return err;
}

/***********************************************************************/

static ULONG
sendText(struct Library *SocketBase,int sock,UBYTE *text,ULONG len,struct MsgPort *port,ULONG *current)
{
    register int pass, rest, i;
    ULONG    	 err;

    pass = len/256;
    rest = len%256;

    for (i = 0; i<pass; i++)
    {
    	if (err = sen(SocketBase,sock,text,256)) return err;
        *current += 256;
        notifyCurrent(port,*current);
        text += 256;
    }

    if (rest)
    {
    	if (err = sen(SocketBase,sock,text,rest)) return err;
        *current += rest;
        notifyCurrent(port,*current);
    }

    return 0;
}

/***********************************************************************/

#ifdef __MORPHOS__
void task(void)
#else
void SAVEDS task(void)
#endif
{
    struct MsgPort         port;
    struct Process         *task = (struct Process *)FindTask(NULL);
    struct SignalSemaphore *sem;
    struct taskMsg         *tmsg;
    register ULONG         err;
    int                    sig;

    WaitPort(&task->pr_MsgPort);
    tmsg = (struct taskMsg *)GetMsg(&task->pr_MsgPort);

    sem = tmsg->sem;

    if ((sig = AllocSignal(-1))>=0)
    {
        struct Library *SocketBase;

        if (SocketBase = OpenLibrary("bsdsocket.library",4))
        {
            register int sock;

            SocketBaseTags(SBTM_SETVAL(SBTC_BREAKMASK),SIGBREAKF_CTRL_C | (1<<sig),TAG_DONE);

            if ((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))>=0)
            {
                struct sockaddr_in sin, me;
                struct MsgPort     *resPort;
                UBYTE              buf[1024], email[EMAILLEN], realName[REALNAMELEN],
                                   to[TOLEN], subject[SUBJECTLEN], codesetName[CODESETNAMELEN],
                                   charset[CODESETNAMELEN+32], hostName[HOSTLEN], timeBuf[32],
                                   GMTTimeBuf[64], boundary[64], *text, *thisHostName, *errBuf;
                struct DateStamp   date;
                struct MinList 	   *attaches;
                ULONG          	   sendQuit = FALSE, doAttaches, total, current;
                long               len;
                int            	   hostPort, errBufLen, textLen;

        		resPort = tmsg->resPort;
                stccpy(email,tmsg->email,sizeof(email));
                stccpy(realName,tmsg->realName,sizeof(realName));
                stccpy(to,tmsg->to,sizeof(to));
                stccpy(subject,tmsg->subject,sizeof(subject));
                stccpy(hostName,tmsg->host,sizeof(hostName));
                if (tmsg->codesetName) stccpy(codesetName,tmsg->codesetName,sizeof(codesetName));
                else *codesetName = 0;
                hostPort   = tmsg->hostPort;
                text       = tmsg->text;
                errBuf     = tmsg->errBuf;
                errBufLen  = tmsg->errBufLen;
        		attaches   = tmsg->attaches;

		        doAttaches = !IsListEmpty((struct List *)attaches);

                INITPORT(&port,sig);
                tmsg->procPort = &port;
                tmsg->err = 0;
                ReplyMsg((struct Message *)tmsg);

	            WaitPort(&port);
	            tmsg = (struct taskMsg *)GetMsg(&port);

                sin.sin_port = hostPort;

                notifyStatus(resPort,MUIV_MiniMailer_Resolving,0);

                if ((long)(sin.sin_addr.s_addr = inet_addr(hostName))==INADDR_NONE)
                {
                    struct hostent *host;

                    if (host = gethostbyname(hostName))
                    {
                        copymem(&sin.sin_addr.s_addr,host->h_addr,4);
                    }
                    else
                    {
                        err = Msg_CantResolve;
                        goto fail;
                    }
                }

                sin.sin_family = AF_INET;
                sin.sin_len    = sizeof(struct sockaddr_in);

                notifyStatus(resPort,MUIV_MiniMailer_Connecting,0);

                if (connect(sock,(struct sockaddr *)&sin,sizeof(sin))<0)
                {
                    err = Msg_CantConnect;
                    goto fail;
                }

                sendQuit = TRUE;

                if (err = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen)) goto fail;

                len = sizeof(me);
                if (getsockname(sock,(struct sockaddr *)&me,&len)<0)
                {
                    err = Msg_NoMem;
                    goto fail;
                }
                else
                {
                    thisHostName = Inet_NtoA(me.sin_addr.s_addr);
                }

                notifyStatus(resPort,MUIV_MiniMailer_Sending,0);

                snprintf(buf,sizeof(buf),"HELO %s\r\n",thisHostName);
                if (err = sen(SocketBase,sock,buf,strlen(buf))) goto fail;

                DateStamp(&date);
                getTime(&date,timeBuf);
                getGMTTime(&date,GMTTimeBuf);

                snprintf(buf,sizeof(buf),"MAIL FROM: <%s>\r\n",email);
                if (err = sen(SocketBase,sock,buf,strlen(buf))) goto fail;

                if (err = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen)) goto fail;

                snprintf(buf,sizeof(buf),"RCPT TO: <%s>\r\n",to);
                if (err = sen(SocketBase,sock,buf,strlen(buf))) goto fail;

                if (err = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen)) goto fail;

                if (err = sen(SocketBase,sock,"DATA\r\n",6)) goto fail;

                if (err = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen)) goto fail;

                if (*codesetName) sprintf(charset,"Content-Type: text/plain; charset=%s",codesetName);
                else strcpy(charset,"Content-Type: text/plain");

        		if (doAttaches)
                {
		            sprintf(boundary,"----=_NextPart_%08lx",(ULONG)FindTask(NULL));

        		    total = attachesSize(attaches);

                    total += snprintf(buf,sizeof(buf),"\
Message-ID: <MiniMailer%s@%s>\r\n\
From: %s <%s>\r\n\
Subject: %s\r\n\
Date: %s\r\n\
To: <%s>\r\n\
X-Mailer: " VERS " by Alfonso Ranieri http://alfie.altervista.org\r\n\
MIME-Version: 1.0\r\n\
Content-Type: multipart/mixed;\r\n\
\t\tboundary=\"%s\"\r\n\r\n\
This is a multi-part message in MIME format.\r\n\r\n\
--%s\r\n\
%s;\r\n\
\t\tformat=flowed\r\n\
Content-Transfer-Encoding: 8bit\r\n\r\n",
timeBuf, thisHostName,
realName, email,
subject,
GMTTimeBuf,
to,
boundary,
boundary,
charset
);
                }
            	else
                {
                    total = snprintf(buf,sizeof(buf),"\
Message-ID: <MiniMailer%s@%s>\r\n\
From: %s <%s>\r\n\
Subject: %s\r\n\
Date: %s\r\n\
To: <%s>\r\n\
X-Mailer: " VERS " by Alfonso Ranieri http://alfie.altervista.org\r\n\
Content-Transfer-Encoding: 8bit\r\n\
%s\r\n\r\n",
timeBuf, thisHostName,
realName, email,
subject,
GMTTimeBuf,
to,
charset
);
		        }
		
		        textLen = strlen(text);
                total += textLen;
                notifyTotal(resPort,total);

                if (err = sen(SocketBase,sock,buf,current = strlen(buf))) goto fail;
    		    notifyCurrent(resPort,current);

		        if (err = sendText(SocketBase,sock,text,textLen,resPort,&current)) goto fail;

                if (doAttaches)
                {
                    if (err = sen(SocketBase,sock,"\r\n\r\n",2)) goto fail;
                    if (err = sendAttaches(SocketBase,sock,attaches,boundary,resPort,&current)) goto fail;

		            sprintf(buf,"\r\n\r\n--%s--",boundary);
                    if (err = sen(SocketBase,sock,buf,strlen(buf))) goto fail;
                }

                if (err = sen(SocketBase,sock,"\r\n.\r\n",5)) goto fail;

                if (err = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen)) goto fail;

           fail:
                if (err && (Errno()==4)) err = Msg_Broken;

                if (sendQuit)
                {
                    ULONG e;

                    e = sen(SocketBase,sock,"QUIT\r\n",6);
                    if (err==0) err = e;
                    else
                    {
                        e = rec(SocketBase,sock,buf,sizeof(buf),errBuf,errBufLen);
                        if (err==0) err = e;
                    }
                }

	            notifyStatus(resPort,MUIV_MiniMailer_Done,err);

                CloseSocket(sock);
            }
            else err = Msg_NoSocket;

            CloseLibrary(SocketBase);
        }
        else err = Msg_NoSocketBase;
    }
    else err = Msg_NoMem;

    tmsg->err = err;

    if (sig>=0) FreeSignal(sig);

    ObtainSemaphore(sem);
    tmsg->proc = NULL;
    ReleaseSemaphore(sem);

    Forbid();
    ReplyMsg((struct Message *)tmsg);
}

/***********************************************************************/
