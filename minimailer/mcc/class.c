
#include "class.h"
#include <dos/dostags.h>
#include "http.h"
#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

#define SETINFO(o,str) set(o,MUIA_Gauge_InfoText,(ULONG)(str))

/***********************************************************************/

struct data
{
    Object                      *app;
    Object                      *this;

    APTR                        menu;

    Object                      *to;
    Object                      *cc;
    Object                      *subject;
    Object                      *text;

    Object		                *attach;

    Object                      *prefsGroup;
    Object                      *realName;
    Object                      *email;
    Object                      *host;
    Object                      *hostPort;

    Object                      *pager1;
    Object                      *info;

    UBYTE                       *textPtr;
    struct Hook                 *translateHook;
    struct Hook                 *freeTranslatedHook;
    UBYTE                       *translated;
    UBYTE                       codesetName[CODESETNAMELEN];
    Object                      *translateObj;

    struct taskMsg              tmsg;
    struct SignalSemaphore      sem;
    struct MsgPort              port;
    struct MsgPort              resPort;
    int                         sig;
    struct Process              *proc;
    struct MsgPort              *procPort;
    UBYTE                       errBuf[ERRBUFLEN];
    struct MUI_InputHandlerNode ih;

    ULONG                       flags;
};

enum
{
    FLG_Setup         = 1<<0,
    FLG_Disposing     = 1<<1,
    FLG_Task          = 1<<2,
    FLG_ShowPrefs     = 1<<3,
};

/***********************************************************************/

static ULONG tnMenuIDs[] =
{
    Msg_CItem_MiniMailer,
        Msg_CItem_ShowPrefs,
        0,
    	Msg_CItem_Clear,
    	Msg_CItem_Stop,
};

static struct NewMenu tnMenu[] =
{
    MTITLE(Msg_CItem_MiniMailer),
        MTITEM(Msg_CItem_ShowPrefs,Msg_CItem_ShowPrefs),
        MBAR,
        MITEM(Msg_CItem_Clear,Msg_CItem_Clear),
        MITEM(Msg_CItem_Stop,Msg_CItem_Stop),
    MEND
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data             temp;
    Object         	        *start, *stop;
    register struct TagItem *attrs = msg->ops_AttrList;
    UBYTE                   *w;
    int            	        sig;

    if (GetTagData(MUIA_MiniMailer_MinVer,0,attrs)>lib_version)
        return NULL;

    if ((sig = AllocSignal(-1))<0)
        return NULL;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,

            Child, (ULONG)VGroup,
                //GroupFrame,
                //MUIA_Background, MUII_GroupBack,

                Child, ColGroup(2),
                    Child, olabel2(Msg_To),
                    Child, temp.to = ostring(Msg_To,Msg_To_Help,100,TOLEN),
                    Child, olabel2(Msg_Subject),
                    Child, temp.subject = ostring(Msg_Subject,Msg_Subject_Help,100,SUBJECTLEN),
                    Child, olabel2(Msg_Attach),
                    Child, temp.attach = attachObject, End,
                End,

                Child, temp.text = otext(Msg_Text_Help),

                Child, temp.prefsGroup = VGroup,
                    Child, temp.prefsGroup = ColGroup(2),
                        Child, olabel2(Msg_RealName),
                        Child, temp.realName = ostring(Msg_RealName,Msg_RealName_Help,100,REALNAMELEN),
                        Child, olabel2(Msg_EMail),
                        Child, temp.email = ostring(Msg_EMail,Msg_EMail_Help,100,EMAILLEN),
                        Child, olabel2(Msg_Host),
                        Child, HGroup,
                            Child, temp.host = ostring(Msg_Host,Msg_Host_Help,100,HOSTLEN),
                            Child, olabel2(Msg_Port),
                            Child, temp.hostPort = TextinputObject,
                                StringFrame,
                                MUIA_FixWidthTxt,           "XXXXXXXX",
                                MUIA_ControlChar,           getKeyChar(getString(Msg_Port)),
                                MUIA_ShortHelp,             getString(Msg_Port_Help),
                                MUIA_CycleChain,            TRUE,
                                MUIA_Textinput_MaxLen,      7,
                                MUIA_Textinput_AcceptChars, "0123456789",
                                MUIA_Textinput_MinVal,      1,
                                MUIA_Textinput_MaxVal,      65535,
                                MUIA_String_Integer,        DEFHOSTPORT,
                                MUIA_Textinput_Format,      MUIV_Textinput_Format_Right,
                                MUIA_Textinput_IsNumeric,   1,
                                MUIA_Textinput_AdvanceOnCR, TRUE,
                            End,
                        End,
                    End,
                    Child, RectangleObject, MUIA_Weight, 0, MUIA_Rectangle_HBar, TRUE, End,
                End,
            End,

            Child, (ULONG)HGroup,
                MUIA_Group_Spacing, 2,
                Child, temp.info = GaugeObject,
                    MUIA_Frame, MUIV_Frame_Gauge,
                    MUIA_Text_PreParse, MUIX_C,
                    MUIA_Gauge_Horiz,    TRUE,
                    MUIA_Gauge_InfoText, (w = (UBYTE *)GetTagData(MUIA_MiniMailer_Welcome,NULL,attrs)) ? w : getString(Msg_Welcome),
                End,

                Child, temp.pager1 = HGroup,
                    MUIA_Weight,         40,
                    MUIA_Group_PageMode, TRUE,

                    Child, start = obutton(Msg_Send,Msg_Send_Help),
                    Child, stop = obutton(Msg_Stop,Msg_Stop_Help),
                End,
            End,

            TAG_MORE,(ULONG)attrs))
    {
        register struct data *data = INST_DATA(cl,obj);

        CopyMem(&temp,data,sizeof(*data));

        InitSemaphore(&data->sem);
        data->sig = sig;
        INITPORT(&data->port,sig);
        INITPORT(&data->resPort,sig);

        data->this = obj;
        data->flags |= FLG_ShowPrefs;
        data->translateObj = obj;

    	set(data->pager1,MUIA_Weight,10);

        msg->MethodID = OM_SET;
        DoMethodA(obj,(Msg)msg);
        msg->MethodID = OM_NEW;

        DoMethod(start,MUIM_Notify,MUIA_Pressed,FALSE,(ULONG)obj,1,MUIM_MiniMailer_Send);
        DoMethod(stop,MUIM_Notify,MUIA_Pressed,FALSE,(ULONG)obj,1,MUIM_MiniMailer_Stop);

        if (data->menu = MUI_MakeObject(MUIO_MenustripNM,(ULONG)tnMenu,0))
            superset(cl,obj,MUIA_ContextMenu,TRUE);
    }
    else
    {
        FreeSignal(sig);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);
    ULONG       res;
    int         sig = data->sig;

    data->flags |= FLG_Disposing;

    res = DoSuperMethodA(cl,obj,msg);

    FreeSignal(sig);

    return res;
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    data->app = _app(obj);

    memset(&data->ih,0,sizeof(data->ih));
    data->ih.ihn_Object  = obj;
    data->ih.ihn_Signals = 1<<data->sig;
    data->ih.ihn_Method  = MUIM_MiniMailer_HandleEvent;
    DoMethod(data->app,MUIM_Application_AddInputHandler,(ULONG)&data->ih);

    data->flags |= FLG_Setup;

    return TRUE;
}

/***********************************************************************/

static void
clearTaskStuff(struct IClass *cl,Object *obj,struct data *data)
{
    if (data->textPtr)
    {
        FreeVec(data->textPtr);
        data->textPtr = NULL;
    }

    if (data->translated)
    {
        if (data->freeTranslatedHook) CallHookPkt(data->freeTranslatedHook,data->translated,NULL);
        else DoMethod(data->translateObj,MUIM_MiniMailer_FreeTranslated,(ULONG)data->translated);

        data->translated = NULL;
    }

    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,FALSE,
        (ULONG)data->to,
        (ULONG)data->subject,
        (ULONG)data->attach,
        (ULONG)data->text,
        (ULONG)data->realName,
        (ULONG)data->email,
        (ULONG)data->host,
        (ULONG)data->hostPort,
        NULL);

    set(data->pager1,MUIA_Group_ActivePage,0);
}

/***************************************************************************/

static void
parseMsg(struct IClass *cl,struct data *data,struct infoMsg *im)
{
    switch (im->type)
    {
    	case MTYPE_Status:
	{
            ULONG status = ((struct statusMsg *)im)->status, err = ((struct statusMsg *)im)->err;

	    switch (status)
	    {
	        case MUIV_MiniMailer_Resolving:
	            SETINFO(data->info,getString(Msg_Resolving));
	            break;

	        case MUIV_MiniMailer_Connecting:
	            SETINFO(data->info,getString(Msg_Connecting));
	            break;

	        case MUIV_MiniMailer_Sending:
	            SETINFO(data->info,getString(Msg_Sending));
	            break;

	        case MUIV_MiniMailer_Done:
            	    SetAttrs(data->info,MUIA_Gauge_Max,0,MUIA_Gauge_Current,0,TAG_DONE);
	            if (err)
	            {
	                if (err==Msg_SeverError)
	                {
	                    REGARRAY UBYTE buf[256];

	                    snprintf(buf,sizeof(buf),getString(Msg_SeverError),data->errBuf);
	                    SETINFO(data->info,buf);
	                }
	                else SETINFO(data->info,getString(err));
	            }
	            else SETINFO(data->info,getString(Msg_Done));

	            break;
	    }
	}

    	case MTYPE_Total:
	{
            ULONG total = ((struct totalMsg *)im)->total;

            set(data->info,MUIA_Gauge_Max,total);
            break;
        }

    	case MTYPE_Current:
	{
            ULONG current = ((struct currentMsg *)im)->current;

            set(data->info,MUIA_Gauge_Current,current);
            break;
        }

        default:
            break;
    }
}

/***************************************************************************/

static void
breakProc(struct IClass *cl,struct data *data,ULONG wait)
{
    if (data->flags & FLG_Task)
    {
	struct infoMsg *im;

	if (wait)
        {
            ObtainSemaphore(&data->sem);
            if (data->tmsg.proc) Signal((struct Task *)data->tmsg.proc,SIGBREAKF_CTRL_C);
            ReleaseSemaphore(&data->sem);

            WaitPort(&data->port);
            GetMsg(&data->port);
	}

	while (im = (struct infoMsg *)GetMsg(&data->resPort))
	{
            parseMsg(cl,data,im);
            freeArbitrateVecPooled(im);
	}

	data->flags &= ~FLG_Task;
    }

    clearTaskStuff(cl,data->this,data);
}

/***********************************************************************/

static ULONG
mHandleEvent(struct IClass *cl,Object *obj,Msg msg)
{
    struct data    	    *data = INST_DATA(cl,obj);
    register struct infoMsg *im;
    register ULONG	    res = 0;

    while (im = (struct infoMsg *)GetMsg(&data->resPort))
    {
        res++;

	parseMsg(cl,data,im);
        freeArbitrateVecPooled(im);
    }

    if (GetMsg(&data->port))
    {
        res++;
	breakProc(cl,data,FALSE);
    }

    return res;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    DoMethod(data->app,MUIM_Application_RemInputHandler,(ULONG)&data->ih);
    breakProc(cl,data,TRUE);

    data->app = NULL;
    data->flags &= ~FLG_Setup;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_MiniMailer_Host:          *msg->opg_Storage = xget(data->host,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_HostPort:      *msg->opg_Storage = xget(data->hostPort,MUIA_String_Integer); return TRUE;
        case MUIA_MiniMailer_EMail:         *msg->opg_Storage = xget(data->email,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_RealName:      *msg->opg_Storage = xget(data->realName,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_To:            *msg->opg_Storage = xget(data->to,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_Subject:       *msg->opg_Storage = xget(data->subject,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_Text:          *msg->opg_Storage = xget(data->text,MUIA_String_Contents); return TRUE;
        case MUIA_MiniMailer_ShowPrefs:     *msg->opg_Storage = (data->flags & FLG_ShowPrefs) ? TRUE : FALSE;
        case MUIA_Version:                  *msg->opg_Storage = lib_version; return TRUE;
        case MUIA_Revision:                 *msg->opg_Storage = lib_revision; return TRUE;

        case MUIA_MiniMailer_Attaches:      return DoMethodA(data->attach,(Msg)msg);

        default:                            return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data    	    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;
    struct TagItem          *tstate;

    for (tstate = msg->ops_AttrList; tag = NextTagItem(&tstate); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_MiniMailer_Host:
                set(data->host,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_HostPort:
                if ((long)tidata<0 || (long)tidata>65535) tidata = DEFHOSTPORT;
                set(data->hostPort,MUIA_String_Integer,tidata);
                break;

            case MUIA_MiniMailer_EMail:
                set(data->email,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_RealName:
                set(data->realName,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_To:
                set(data->to,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_Subject:
                set(data->subject,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_Text:
                set(data->text,MUIA_String_Contents,tidata);
                break;

            case MUIA_MiniMailer_ShowPrefs:
                if (BOOLSAME(tidata,data->flags & FLG_ShowPrefs))
                {
                    tag->ti_Tag = TAG_IGNORE;
                }
                else
                {
                    if (tidata) data->flags |= FLG_ShowPrefs;
                    else data->flags &= ~FLG_ShowPrefs;

                    set(data->prefsGroup,MUIA_ShowMe,tidata);
                }
                break;

            case MUIA_MiniMailer_TranslateHook:
                data->translateHook = (struct Hook *)tidata;
                break;

            case MUIA_MiniMailer_FreeTranslatedHook:
                data->freeTranslatedHook = (struct Hook *)tidata;
                break;

            case MUIA_MiniMailer_TranslateObj:
                data->translateObj = tidata ? (Object *)tidata : obj;
                break;
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
menuChoice(struct data *data,ULONG item)
{
    Object *obj = data->this;

    switch (item)
    {
        case Msg_CItem_Clear:
            DoMethod(obj,MUIM_MiniMailer_Clear);
            break;

        case Msg_CItem_ShowPrefs:
            set(obj,MUIA_MiniMailer_ShowPrefs,!(data->flags & FLG_ShowPrefs));
            break;

        case Msg_CItem_Stop:
            DoMethod(obj,MUIM_MiniMailer_Stop);
            break;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    return menuChoice(INST_DATA(cl,obj),muiUserData(msg->item));
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_ContextMenuBuild *msg)
{
    struct data *data = INST_DATA(cl,obj);

    if (data->menu)
    {
        ULONG prefs, taskon;

        prefs = data->flags & FLG_ShowPrefs;

        ObtainSemaphoreShared(&data->sem);
        taskon = data->flags & FLG_Task;
        ReleaseSemaphore(&data->sem);

        set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_CItem_ShowPrefs),MUIA_Menuitem_Checked,prefs);
        set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_CItem_Stop),MUIA_Menuitem_Enabled,taskon);

        return (ULONG)data->menu;
    }

    return (ULONG)data->menu;
}

/***********************************************************************/

static ULONG
mSend(struct IClass *cl,Object *obj,Msg msg)
{
    struct data    *data = INST_DATA(cl,obj);
    struct Process *proc;
    struct taskMsg *tm = &data->tmsg;
    Object         *miss;
    ULONG          res = FALSE, err;

    if (!(data->flags & FLG_Setup)) return FALSE;
    if (data->flags & FLG_Task) return FALSE;

    set(data->app,MUIA_Application_Sleep,TRUE);

    memset(tm,0,sizeof(struct taskMsg));
    INITMESSAGE(tm,&data->port,sizeof(struct taskMsg));

    tm->resPort = &data->resPort;
    tm->sem     = &data->sem;

    tm->email     = (UBYTE *)xget(data->email,MUIA_String_Contents);
    tm->realName  = (UBYTE *)xget(data->realName,MUIA_String_Contents);
    tm->to        = (UBYTE *)xget(data->to,MUIA_String_Contents);
    tm->subject   = (UBYTE *)xget(data->subject,MUIA_String_Contents);
    tm->host      = (UBYTE *)xget(data->host,MUIA_String_Contents);
    tm->hostPort  = xget(data->hostPort,MUIA_String_Integer);
    //tm->hostPort  = 25;
    tm->errBuf    = data->errBuf;
    tm->errBufLen = sizeof(data->errBuf);

    tm->attaches  = (struct MinList *)xget(data->attach,MUIA_MiniMailer_Attaches);

    tm->text      = (UBYTE *)xget(data->text,MUIA_String_Contents);

    miss = NULL;
    err  = Msg_NoArg;

    if (!*tm->realName) miss = data->realName;
    else if (!*tm->email) miss = data->email;
         else if (!*tm->host) miss = data->host;

    if (miss)
    {
        set(obj,MUIA_MiniMailer_ShowPrefs,TRUE);
    }
    else
    {
        if (!*tm->to) miss = data->to;
        else if (!*tm->subject) miss = data->subject;
             else if (!tm->text || !*tm->text) miss = data->text;

        if (!miss)
        {
            err = Msg_InvalidArg;

            if (!strchr(tm->email,'@'))
            {
                miss = data->email;
                set(obj,MUIA_MiniMailer_ShowPrefs,TRUE);
            }
            else if (!strchr(tm->to,'@')) miss = data->to;
        }
    }

    if (miss)
    {
        set(_win(obj),MUIA_Window_ActiveObject,(ULONG)miss);
        SETINFO(data->info,getString(err));
        set(data->app,MUIA_Application_Sleep,FALSE);

        return 0;
    }

    if (tm->hostPort<0 || tm->hostPort>65535)
    {
        tm->hostPort = DEFHOSTPORT;
        set(data->hostPort,MUIA_String_Integer,DEFHOSTPORT);
    }

    if (data->translateHook)
    {
        struct MUIS_MiniMailer_Translate tra;

        tra.text           = tm->text;
        tra.codesetName    = data->codesetName;
        tra.codesetNameLen = sizeof(data->codesetName);
        tra.flags          = 0;

        data->translated = (UBYTE *)CallHookPkt(data->translateHook,&tra,NULL);
    }
    else
    {
        data->translated = (UBYTE *)DoMethod(data->translateObj,MUIM_MiniMailer_Translate,(ULONG)tm->text,(ULONG)data->codesetName,sizeof(data->codesetName),0);
    }

    if (data->translated)
    {
        tm->text        = data->translated;
        tm->codesetName = data->codesetName;
    }
    else tm->codesetName = NULL;

    if (proc = CreateNewProcTags(NP_Entry,        task,
                                 NP_Name,         "MiniMailerTask",
    	                         #ifdef __MORPHOS__
                                 NP_CodeType,     CODETYPE_PPC,
                                 NP_PPCStackSize, 48000,
                                 #else
                                 NP_StackSize,    12000,
                                 #endif
                                 NP_CopyVars,     FALSE,
                                 NP_Input,        NULL,
                                 NP_CloseInput,   FALSE,
                                 NP_Output,       NULL,
                                 NP_CloseOutput,  FALSE,
                                 NP_Error,        NULL,
                                 NP_CloseError,   FALSE,
                                 TAG_DONE))
    {
        PutMsg(&proc->pr_MsgPort,(struct Message *)&data->tmsg);
        WaitPort(&data->port);
        GetMsg(&data->port);

        if (tm->err==0)
        {
	    struct MsgPort *procPort;

            DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,TRUE,
                (ULONG)data->to,
                (ULONG)data->subject,
    	        (ULONG)data->attach,
                (ULONG)data->text,
                (ULONG)data->realName,
                (ULONG)data->email,
                (ULONG)data->host,
                (ULONG)data->hostPort,
                NULL);

            set(data->pager1,MUIA_Group_ActivePage,1);

	    procPort = tm->procPort;
	    memset(tm,0,sizeof(struct taskMsg));
    	    INITMESSAGE(tm,&data->port,sizeof(struct taskMsg));
            tm->proc = proc;
            PutMsg(procPort,(struct Message *)tm);

            data->flags |= FLG_Task;
            res = TRUE;
        }
    }

    set(data->app,MUIA_Application_Sleep,FALSE);

    return res;
}

/***********************************************************************/

static ULONG
mStop(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    breakProc(cl,data,TRUE);

    return 0;
}

/***********************************************************************/

static ULONG
mLoadPrefs(struct IClass *cl,Object *obj,struct MUIP_MiniMailer_LoadPrefs *msg)
{
    struct data  *data = INST_DATA(cl,obj);
    struct prefs *prefs;
    ULONG        res = FALSE;

    if (prefs = allocArbitratePooled(sizeof(*prefs)))
    {
        prefs->hostPort = DEFHOSTPORT;

        if (loadPrefs(msg->file,prefs))
        {
            set(data->email,MUIA_String_Contents,(ULONG)prefs->email);
            set(data->realName,MUIA_String_Contents,(ULONG)prefs->realName);
            set(data->host,MUIA_String_Contents,(ULONG)prefs->host);
            set(data->hostPort,MUIA_String_Integer,(ULONG)prefs->hostPort);

            res = TRUE;
        }

        freeArbitratePooled(prefs,sizeof(*prefs));
    }

    return res;
}

/***********************************************************************/

static ULONG
mSavePrefs(struct IClass *cl,Object *obj,struct MUIP_MiniMailer_SavePrefs *msg)
{
    struct data  *data = INST_DATA(cl,obj);
    struct prefs *prefs;
    ULONG        res = FALSE;

    if (prefs = allocArbitratePooled(sizeof(*prefs)))
    {
        UBYTE *v;

        prefs->version  = PREFSVERSION;

        get(data->email,MUIA_String_Contents,&v);
        strcpy(prefs->email,v);

        get(data->realName,MUIA_String_Contents,&v);
        strcpy(prefs->realName,v);

        get(data->host,MUIA_String_Contents,&v);
        strcpy(prefs->host,v);

        prefs->hostPort = xget(data->hostPort,MUIA_String_Integer);

        res = savePrefs(msg->file,prefs);

        freeArbitratePooled(prefs,sizeof(*prefs));
    }

    return res;
}

/***********************************************************************/

static ULONG
mClear(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    set(data->to,MUIA_String_Contents,NULL);
    set(data->subject,MUIA_String_Contents,NULL);
    set(data->text,MUIA_String_Contents,NULL);
    DoMethod(obj,MUIM_MiniMailer_ClearAttaches);

    return 0;
}

/***********************************************************************/

static ULONG
mClearAttaches(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    DoMethod(data->attach,MUIM_MiniMailer_ClearAttaches);

    return 0;
}

/***********************************************************************/

static ULONG
mAddAttach(struct IClass *cl,Object *obj,Msg msg)
{
    struct data *data = INST_DATA(cl,obj);

    return DoMethodA(data->attach,msg);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                        return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:                    return mDispose(cl,obj,(APTR)msg);
        case OM_GET:                        return mGet(cl,obj,(APTR)msg);
        case OM_SET:                        return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:                    return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                  return mCleanup(cl,obj,(APTR)msg);
        case MUIM_ContextMenuBuild:         return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice:        return mContextMenuChoice(cl,obj,(APTR)msg);

        case MUIM_MiniMailer_Send:          return mSend(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_Stop:          return mStop(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_LoadPrefs:     return mLoadPrefs(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_SavePrefs:     return mSavePrefs(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_Clear:         return mClear(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_HandleEvent:   return mHandleEvent(cl,obj,(APTR)msg);

        case MUIM_MiniMailer_ClearAttaches: return mClearAttaches(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_AddAttach:     return mAddAttach(cl,obj,(APTR)msg);

        default:                            return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initMCC(void)
{
    if (lib_class = MUI_CreateCustomClass(lib_base,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (lib_flags & BASEFLG_MUI20)
            lib_class->mcc_Class->cl_ID = lib_name;

    	localizeNewMenu(tnMenu,tnMenuIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
freeMCC(void)
{
    if (lib_class)
    {
        MUI_DeleteCustomClass(lib_class);
        lib_class = NULL;
    }
}

/***********************************************************************/
