
#include "class.h"
#include <libraries/asl.h>
#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

struct testStruct
{
    Object *stringobject;
    LONG   success;
};

/***********************************************************************/

struct data
{
    struct MinList      attaches;
    Object	            *text;
    Object	            *menu;

    #ifdef __MORPHOS__
    struct EmulLibEntry	startTrap;
    struct EmulLibEntry closeTrap;
    #endif
    struct Hook		    startHook;
    struct Hook         closeHook;

    ULONG		        flags;
};

enum
{
    FLG_Disabled = 1<<0,
};

/***********************************************************************/

#ifdef __MORPHOS__
ULONG
startFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
    Object *obj = (Object *)REG_A2;
    struct TagItem *attrs = (struct TagItem *)REG_A1;
#else
ULONG SAVEDS ASM
startFun(REG(a0,struct Hook *hook),REG(a2,Object *obj),REG(a1,struct TagItem *attrs))
{
#endif
    struct data     *data = hook->h_Data;
    char            *storage;

    set(_win(obj),MUIA_Window_Sleep,TRUE);

    get(data->text,MUIA_Textinput_Contents,&storage);
    if (storage)
    {
        attrs[0].ti_Tag  = ASLFR_InitialFile;
        attrs[0].ti_Data = (ULONG)"";
    }

    return TRUE;
}

/***************************************************************************/

static ULONG
findAttach(struct MinList *ml,UBYTE *file)
{
    register struct MUIS_MiniMailer_Attach *mstate, *succ;

    for (mstate = (struct MUIS_MiniMailer_Attach *)ml->mlh_Head; succ = (struct MUIS_MiniMailer_Attach *)mstate->link.mln_Succ; mstate = succ)
	if (!stricmp(mstate->file,file)) return TRUE;

    return FALSE;
}

/***************************************************************************/

#ifdef __MORPHOS__
ULONG
closeFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
    Object *obj = (Object *)REG_A2;
    struct testStruct *ts = (struct testStruct *)REG_A1;
#else
ULONG SAVEDS ASM
closeFun(REG(a0,struct Hook *hook),REG(a2,Object *obj),REG(a1,struct testStruct *ts))
{
#endif
    struct data          *data = hook->h_Data;
    ULONG                *sdata = INST_DATA(lib_attach->mcc_Super,obj);
    struct FileRequester *req = (struct FileRequester *)sdata[1];

    if (ts->success)
    {
        UBYTE  buf[256];
        Object *o = ts->stringobject;

        if (req->fr_NumArgs)
        {
	        struct FileInfoBlock *fib;

    	    if (fib = AllocDosObject(DOS_FIB,NULL))
            {
            	struct WBArg *args;
                int          i, na, dir;
                UBYTE        *s;

                dir = (int)req->fr_Drawer;

                for (i = 0, na = req->fr_NumArgs, args = req->fr_ArgList; i<na; i++)
                {
                    BPTR lock;

                    if (dir)
                    {
                        strcpy(buf,req->fr_Drawer);
                        AddPart(buf,args[i].wa_Name,sizeof(buf));
                        s = buf;
                    }
                    else s = args[i].wa_Name;

	        	    if (lock = Lock(s,SHARED_LOCK))
	                {
        	        	if (Examine(lock,fib) && (fib->fib_DirEntryType<0) && (fib->fib_Size>0) && !findAttach(&data->attaches,s))
                        {
                    	    struct MUIS_MiniMailer_Attach *a;
		    	            ULONG                         fl = strlen(s);

            			    if (a = allocArbitrateVecPooled(sizeof(struct MUIS_MiniMailer_Attach)+fl+1))
            		        {
            			        a->file =(UBYTE *)a+sizeof(struct MUIS_MiniMailer_Attach);
		                        CopyMem(s,a->file,fl+1);

            			        AddTail((struct List *)&data->attaches,(struct Node *)a);

			                    if (strchr(s,' '))
		                        {
        		                    DoMethod(o,MUIM_Textinput_AppendText,(ULONG)"\"",1);
		                            DoMethod(o,MUIM_Textinput_AppendText,(ULONG)s,fl);
		                            DoMethod(o,MUIM_Textinput_AppendText,(ULONG)"\"",1);
		                        }
        		                else DoMethod(o,MUIM_Textinput_AppendText,(ULONG)s,fl);

		                        DoMethod(o,MUIM_Textinput_AppendText,(ULONG)" ",1);
		                    }
                        }

	                	UnLock(lock);
    	            }
        		}

                FreeDosObject(DOS_FIB,fib);
            }
        }
    }

    set(_win(obj),MUIA_Window_Sleep,FALSE);

    return 0;
}

/***************************************************************************/

static ULONG tnMenuIDs[] =
{
    Msg_CItem_Edit,
	Msg_CItem_Cut,
    Msg_CItem_Copy,
};

static struct NewMenu tnMenu[] =
{
    MTITLE(Msg_CItem_Edit),
        MITEM(Msg_CItem_Cut,Msg_CItem_Cut),
        MITEM(Msg_CItem_Copy,Msg_CItem_Copy),
    MEND
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    Object *text, *bt;

    if (obj = (Object *)DoSuperNew(cl,obj,
	        MUIA_ShortHelp, 	 (ULONG)getString(Msg_Attach_Help),
            ASLFR_TitleText, 	 (ULONG)getString(Msg_Attach_Title),
            ASLFR_DoMultiSelect, TRUE,
	        ASLFR_DoPatterns,	 TRUE,
	        ASLFR_RejectIcons,	 FALSE,

            MUIA_Popstring_String, (ULONG)(text = TextinputObject,
            	TextFrame,
                MUIA_Background, 	   MUII_TextBack,
                MUIA_Textinput_Editable,   FALSE,
                MUIA_Textinput_AutoExpand, TRUE,
            End),

            MUIA_Popstring_Button, (ULONG)(bt = PopButton(MUII_PopFile)),

            TAG_MORE, (ULONG)msg->ops_AttrList))

    {
        struct data *data = INST_DATA(cl,obj);

    	data->text = text;

	    #ifdef __MORPHOS__
        data->startTrap.Trap      = TRAP_LIB;
        data->startTrap.Extension = 0;
        data->startTrap.Func      = (APTR)startFun;
        data->startHook.h_Entry   = (HOOKFUNC)&data->startTrap;

        data->closeTrap.Trap      = TRAP_LIB;
        data->closeTrap.Extension = 0;
        data->closeTrap.Func      = (APTR)closeFun;
        data->closeHook.h_Entry   = (HOOKFUNC)&data->closeTrap;
    	#else
        data->startHook.h_Entry   = (HOOKFUNC)startFun;
        data->closeHook.h_Entry   = (HOOKFUNC)closeFun;
	    #endif
        data->startHook.h_Data    = data;
        data->closeHook.h_Data    = data;


        QUICKNEWLIST(&data->attaches);

        data->menu = MUI_MakeObject(MUIO_MenustripNM,(ULONG)tnMenu,0);
        if (data->menu) superset(cl,obj,MUIA_ContextMenu,TRUE);

    	SetSuperAttrs(cl,obj,MUIA_Popasl_StartHook,    (ULONG)&data->startHook,
                		     MUIA_Popstring_CloseHook, (ULONG)&data->closeHook,
                             TAG_DONE);

        set(bt,MUIA_CycleChain,TRUE);

        if (GetTagData(MUIA_Disabled,FALSE,msg->ops_AttrList))
            data->flags |= FLG_Disabled;
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    struct data                            *data = INST_DATA(cl,obj);
    register struct MUIS_MiniMailer_Attach *a;

    ObtainSemaphore(&lib_poolSem);

    while (a = (struct MUIS_MiniMailer_Attach *)RemHead((struct List *)&data->attaches))
    	freeVecPooled(lib_pool,a);

    ReleaseSemaphore(&lib_poolSem);

    if (data->menu) MUI_DisposeObject(data->menu);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_MiniMailer_Attaches: *msg->opg_Storage = (ULONG)&data->attaches; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data    	        *data = INST_DATA(cl,obj);
    register struct TagItem *tag;

    if (tag = FindTagItem(MUIA_Disabled,msg->ops_AttrList))
        if (tag->ti_Data) data->flags |= FLG_Disabled;
        else data->flags &= ~FLG_Disabled;

    /* ?!?!?!?!?!? Textinput bug */
    if (tag = FindTagItem(MUIA_ContextMenu,msg->ops_AttrList))
        tag->ti_Tag = TAG_IGNORE;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    struct data *data = INST_DATA(cl,obj);

    switch (muiUserData(msg->item))
    {
        case Msg_CItem_Cut:
    	{
	        register struct MUIS_MiniMailer_Attach *a;

	        ObtainSemaphore(&lib_poolSem);

    	    while (a = (struct MUIS_MiniMailer_Attach *)RemHead((struct List *)&data->attaches))
        		freeVecPooled(lib_pool,a);

	        ReleaseSemaphore(&lib_poolSem);

            set(data->text,MUIA_Textinput_Contents,NULL);

            break;
        }

        case Msg_CItem_Copy:
            DoMethod(data->text,MUIM_Textinput_DoCopy);
            break;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_ContextMenuBuild *msg)
{
    struct data *data = INST_DATA(cl,obj);

    if (data->flags & FLG_Disabled) return NULL;

    return (ULONG)data->menu;
}

/***********************************************************************/

static ULONG
mClearAttaches(struct IClass *cl,Object *obj,Msg msg)
{
    struct data                            *data = INST_DATA(cl,obj);
    register struct MUIS_MiniMailer_Attach *a;

    ObtainSemaphore(&lib_poolSem);

    while (a = (struct MUIS_MiniMailer_Attach *)RemHead((struct List *)&data->attaches))
    	freeVecPooled(lib_pool,a);

    ReleaseSemaphore(&lib_poolSem);

    set(data->text,MUIA_String_Contents,NULL);

    return 0;
}

/***********************************************************************/

static ULONG
mAddAttach(struct IClass *cl,Object *obj,struct MUIP_MiniMailer_AddAttach *msg)
{
    struct data *data = INST_DATA(cl,obj);
    UBYTE       *f = msg->file;
    BPTR        lock;
    ULONG       res = FALSE;

    if (lock = Lock(f,SHARED_LOCK))
    {
        struct FileInfoBlock *fib;

        if (fib = AllocDosObject(DOS_FIB,NULL))
        {
        	if (Examine(lock,fib) && (fib->fib_DirEntryType<0) && (fib->fib_Size>0) && !findAttach(&data->attaches,f))
            {
        	    struct MUIS_MiniMailer_Attach *a;
		        ULONG                         fl = strlen(f);

    		    if (a = allocArbitrateVecPooled(sizeof(struct MUIS_MiniMailer_Attach)+fl+1))
    	        {
    		        a->file =(UBYTE *)a+sizeof(struct MUIS_MiniMailer_Attach);
	                CopyMem(f,a->file,fl+1);

    		        AddTail((struct List *)&data->attaches,(struct Node *)a);

	                if (strchr(f,' '))
	                {
    	                DoMethod(data->text,MUIM_Textinput_AppendText,(ULONG)"\"",1);
	                    DoMethod(data->text,MUIM_Textinput_AppendText,(ULONG)f,fl);
	                    DoMethod(data->text,MUIM_Textinput_AppendText,(ULONG)"\"",1);
	                }
    	            else DoMethod(data->text,MUIM_Textinput_AppendText,(ULONG)f,fl);

	                DoMethod(data->text,MUIM_Textinput_AppendText,(ULONG)" ",1);

                    res = TRUE;
	            }
            }

            FreeDosObject(DOS_FIB,fib);
        }
	
        UnLock(lock);
    }

    return res;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:  		                return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:  	                return mDispose(cl,obj,(APTR)msg);
        case OM_GET:  		                return mGet(cl,obj,(APTR)msg);
        case OM_SET:                 	    return mSets(cl,obj,(APTR)msg);

        case MUIM_ContextMenuBuild:  	    return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice: 	    return mContextMenuChoice(cl,obj,(APTR)msg);

        case MUIM_MiniMailer_ClearAttaches: return mClearAttaches(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_AddAttach:     return mAddAttach(cl,obj,(APTR)msg);

        default:                            return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initAttach(void)
{
    if (lib_attach = MUI_CreateCustomClass(NULL,MUIC_Popasl,NULL,sizeof(struct data),DISP(dispatcher)))
    {
    	localizeNewMenu(tnMenu,tnMenuIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
freeAttach(void)
{
    MUI_DeleteCustomClass(lib_attach);
    lib_attach = NULL;
}

/***********************************************************************/

