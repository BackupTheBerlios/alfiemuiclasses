/*
**
** Calendar - Calendar MUI custom class
**
** Copyright 2004-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**/

#include "class.h"
#include <libraries/asl.h>
#include "Calendar.mcp_rev.h"
#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

struct data
{
    Object  *ewin;

    Object  *wsd;

    Object  *daysNamesTextPen;
    Object  *daysNamesBackground;
    Object  *daysNamesDetailPen;
    Object  *daysBackground;
    Object  *daysTextPen;
    Object  *daysShinePen;
    Object  *daysFillPen;
    Object  *sundaysPen;
    Object  *anniversaryPen;

    Object  *font;

    Object  *useDaysNamesBackground;
    Object  *useDaysBackground;
    Object  *useDaysFillPen;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperMethodA(cl,obj,(APTR)msg))
    {
        register struct data    *data = INST_DATA(cl,obj);
        REGARRAY UBYTE          buf[128];
        register STRPTR		    t;
        register Object         *prefs, *trans, *sample, *groups[8];

        snprintf(buf,sizeof(buf),getString(Msg_Info_First),"\33bCalendar " VRSTRING "\33n (" DATE ")\33n");

        if ((t = getString(Msg_Info_Translator)) && *t)
            trans = VGroup,
                MUIA_InnerLeft,0,
                MUIA_InnerBottom,0,
                MUIA_InnerTop,0,
                MUIA_InnerRight,0,
                Child, TextObject,
                    MUIA_Text_PreParse, "\033c",
                    MUIA_Text_Contents, t,
                End,
                Child, ofhspace("a"),
            End;
        else trans = NULL;

        if (prefs = VGroup,

                Child, HGroup,
                    Child, HGroup,
                        GroupFrame,
                        MUIA_Background, MUII_GroupBack,
                        MUIA_ShortHelp, getString(Msg_WeekStartDay_Help),
                        Child, HSpace(0),
                        Child, oflabel(Msg_WeekStartDay),
                        Child, data->wsd = wsdObject, End,
                        Child, HSpace(0),
                    End,
                    Child, VGroup,
                        VirtualFrame,
                        MUIA_Background, MUII_GroupBack,
                        Child, sample = TextObject,
                            MUIA_CycleChain,    TRUE,
                            MUIA_Font,          MUIV_Font_Button,
                            MUIA_InputMode,     MUIV_InputMode_RelVerify,
                            ButtonFrame,
                            MUIA_Background,    MUII_ButtonBack,
                            MUIA_Text_Contents, getString(Msg_Sample_Sample),
                            MUIA_Text_PreParse, MUIX_C,
                            MUIA_Text_SetMax,   TRUE,
                        End,
                        Child, VSpace(0),
                    End,
                End,

                Child, HGroup,

                    Child, VGroup,
                        MUIA_Background, MUII_GroupBack,
                        GroupFrame,
                        MUIA_FrameTitle, getString(Msg_DaysNames_Title),

                        Child, wspace(5),

                        Child, HGroup,
                            Child, wspace(5),

                            Child, ColGroup(2),
                                Child, oflabel(Msg_DaysNamesTextPen),
                                Child, data->daysNamesTextPen = opoppen(Msg_DaysNamesTextPen,Msg_DaysNamesTextPen_Title,Msg_DaysNamesTextPen_Help),

                                Child, VGroup,
                                    Child, wspace(1),
                                    Child, groups[0] = HGroup,
                                        MUIA_Group_HorizSpacing, 2,
		                                Child, oflabel(Msg_DaysNamesBackground),
    	                                Child, data->useDaysNamesBackground = ocheck(0,Msg_UseDaysNamesBackground_Help),
                                    End,
                                    Child, wspace(1),
                                End,
                                Child, data->daysNamesBackground = opopimage(MUIV_Imageadjust_Type_Background,Msg_DaysNamesBackground,Msg_DaysNamesBackground_Title,Msg_DaysNamesBackground_Help),

                                Child, oflabel(Msg_DaysNamesDetailPen),
                                Child, data->daysNamesDetailPen = opoppen(Msg_DaysNamesDetailPen,Msg_DaysNamesDetailPen_Title,Msg_DaysNamesDetailPen_Help),
                            End,

                            Child, wspace(5),
                        End,
                        Child, wspace(5),
            		End,

                    Child, VGroup,
                        MUIA_Background, MUII_GroupBack,
                        GroupFrame,
                        MUIA_FrameTitle, getString(Msg_Days_Title),

                        Child, wspace(5),

                        Child, HGroup,
                            Child, wspace(5),

                            Child, RowGroup(3),

                                Child, oflabel(Msg_DaysTextPen),
                                Child, data->daysTextPen  = opoppen(Msg_DaysTextPen,Msg_DaysTextPen_Title,Msg_DaysTextPen_Help),
                                Child, wspace(5),

                                Child, VGroup,
                                    Child, wspace(1),
                                    Child, groups[1] = HGroup,
                                        MUIA_Group_HorizSpacing, 2,
		                                Child, oflabel(Msg_DaysBackground),
	                                    Child, data->useDaysBackground = ocheck(0,Msg_UseDaysBackground_Help),
                                    End,
                                    Child, wspace(1),
                                End,
                                Child, data->daysBackground = (lib_flags & BASEFLG_MUI4) ? opopfri(Msg_DaysBackground,Msg_DaysBackground_Title,Msg_DaysBackground_Help) : opopimage(MUIV_Imageadjust_Type_Background,Msg_DaysBackground,Msg_DaysBackground_Title,Msg_DaysBackground_Help),

                                Child, oflabel(Msg_DaysShinePen),
                                Child, data->daysShinePen = opoppen(Msg_DaysShinePen,Msg_DaysShinePen_Title,Msg_DaysShinePen_Help),
                                Child, wspace(5),
                                Child, oflabel(Msg_SundaysPen),
                                Child, data->sundaysPen = opoppen(Msg_SundaysPen,Msg_SundaysPen_Title,Msg_SundaysPen_Help),

                                Child, VGroup,
                                    Child, wspace(1),
                                    Child, groups[2] = HGroup,
                                        MUIA_Group_HorizSpacing, 2,
                                        Child, oflabel(Msg_DaysFillPen),
                                        Child, data->useDaysFillPen = ocheck(0,Msg_UseDaysFillPen_Help),
                                    End,
                                    Child, wspace(1),
                                End,
                                Child, data->daysFillPen  = opoppen(Msg_DaysFillPen,Msg_DaysFillPen_Title,Msg_DaysFillPen_Help),
                                Child, wspace(5),
                                Child, oflabel(Msg_AnniversaryPen),
                                Child, data->anniversaryPen = opoppen(Msg_AnniversaryPen,Msg_AnniversaryPen_Title,Msg_AnniversaryPen_Help),
                            End,

                            Child, wspace(5),
                        End,
                        Child, wspace(5),
                    End,
                End,

                Child, HGroup,
                    MUIA_Background, MUII_GroupBack,
                    GroupFrame,

                    Child, oflabel(Msg_Font),
                    Child, PopaslObject,
                        MUIA_ShortHelp, getString(Msg_Font_Help),
                        MUIA_Popasl_Type, ASL_FontRequest,
                        MUIA_Popstring_String, data->font = StringObject,
                            StringFrame,
                            MUIA_CycleChain,  TRUE,
                            MUIA_ControlChar, getKeyChar(getString(Msg_Font)),
                        End,
                        MUIA_Popstring_Button, opop(MUII_PopFont,Msg_Font),
                    End,
                End,

                Child, CrawlingObject, // >Crawling

                    MUIA_FixHeightTxt, "\n",
                    TextFrame,
                    MUIA_Background, "m1",

                    Child, TextObject,
                        MUIA_Text_PreParse, "\033c",
                        MUIA_Text_Contents, buf,
                    End,

                    Child, TextObject,
                        MUIA_Text_PreParse, "\033c",
                        MUIA_Text_Contents, getString(Msg_Info_Reserved),
                    End,

                    Child, ofhspace("a"),

                    Child, TextObject,
                        MUIA_Text_PreParse, "\033c",
                        MUIA_Text_Contents, getString(Msg_Info_Rest),
                    End,

                    Child, ofhspace("a"),

                    trans ? Child : TAG_IGNORE, trans,

                    Child, TextObject,
                        MUIA_Text_PreParse, "\033c",
                        MUIA_Text_Contents, buf,
                    End,

                End, // <Crawling

            End)
        {
            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)prefs);

            DoMethod(sample,MUIM_Notify,MUIA_Pressed,FALSE,(ULONG)obj,1,MUIM_Calendar_MCPSample);

            if (lib_flags & BASEFLG_MUI4)
            {
                DoMethod(groups[0],OM_REMMEMBER,(ULONG)data->useDaysNamesBackground);
                DoMethod(groups[1],OM_REMMEMBER,(ULONG)data->useDaysBackground);
				DoMethod(groups[2],OM_REMMEMBER,(ULONG)data->useDaysFillPen);


                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->wsd,                     MUICFG_Calendar_WeekStartDay,               1, (ULONG)getString(Msg_WeekStartDay           ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysNamesTextPen,        MUICFG_Calendar_DaysNamesTextPen,           1, (ULONG)getString(Msg_DaysNamesTextPen       ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysNamesBackground,     MUICFG_Calendar_DaysNamesBackground,        1, (ULONG)getString(Msg_DaysNamesBackground    ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysNamesDetailPen,      MUICFG_Calendar_DaysNamesDetailPen,         1, (ULONG)getString(Msg_DaysNamesDetailPen     ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysBackground,          MUICFG_Calendar_DaysBackground,             1, (ULONG)getString(Msg_DaysBackground         ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysTextPen,             MUICFG_Calendar_DaysTextPen,                1, (ULONG)getString(Msg_DaysTextPen            ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysShinePen,            MUICFG_Calendar_DaysShinePen,               1, (ULONG)getString(Msg_DaysShinePen           ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->daysFillPen,             MUICFG_Calendar_DaysFillPen,                1, (ULONG)getString(Msg_DaysFillPen            ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->sundaysPen,             MUICFG_Calendar_SundaysPen,                1, (ULONG)getString(Msg_SundaysPen            ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->anniversaryPen,          MUICFG_Calendar_AnniversaryPen,             1, (ULONG)getString(Msg_AnniversaryPen         ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->font,                    MUICFG_Calendar_Font,                       1, (ULONG)getString(Msg_Font                   ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->useDaysNamesBackground,  MUICFG_Calendar_UseDaysNamesBackground,     1, (ULONG)getString(Msg_UseDaysNamesBackground ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->useDaysBackground,       MUICFG_Calendar_UseDaysBackground,          1, (ULONG)getString(Msg_UseDaysBackground      ));
                DoSuperMethod(cl,obj,MUIM_Mccprefs_RegisterGadget,(ULONG)data->useDaysFillPen,          MUICFG_Calendar_UseDaysFillPen,             1, (ULONG)getString(Msg_UseDaysFillPen         ));
            }
        }
        else
        {
            CoerceMethod(cl,obj,OM_DISPOSE);
            obj = 0;
        }
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    switch (msg->opg_AttrID)
    {
        case MUIA_Version:  *msg->opg_Storage = lib_version; return TRUE;
        case MUIA_Revision: *msg->opg_Storage = lib_revision; return TRUE;
        default:            return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mConfigToGadgets(struct IClass *cl,Object *obj,struct MUIP_Settingsgroup_ConfigToGadgets *msg)
{
    register struct data    *data = INST_DATA(cl, obj );
    register Object         *cfg = msg->configdata;
    APTR                    ptr;
    register ULONG          v;
    ULONG                   *val;

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysNamesTextPen)))
        ptr = MUIDEF_Calendar_DaysNamesTextPen;
    set(data->daysNamesTextPen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysNamesBackground)))
        ptr = MUIDEF_Calendar_DaysNamesBackground;
    set(data->daysNamesBackground,MUIA_Imagedisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysNamesDetailPen)))
        ptr = MUIDEF_Calendar_DaysNamesDetailPen;
    set(data->daysNamesDetailPen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysBackground)))
        ptr = (lib_flags & BASEFLG_MUI4) ? MUIDEF_Calendar_DaysBackground_MUI4 : MUIDEF_Calendar_DaysBackground;
    set(data->daysBackground,MUIA_Imagedisplay_Spec,(ULONG)ptr);
    if (lib_flags & BASEFLG_MUI4)
    {
        if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysFrame)))
            ptr = MUIDEF_Calendar_DaysFrame;
        set(data->daysBackground,MUIA_Framedisplay_Spec,(ULONG)ptr);
    }

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysTextPen)))
        ptr = MUIDEF_Calendar_DaysTextPen;
    set(data->daysTextPen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysShinePen)))
        ptr = MUIDEF_Calendar_DaysShinePen;
    set(data->daysShinePen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_DaysFillPen)))
        ptr = MUIDEF_Calendar_DaysFillPen;
    set(data->daysFillPen,MUIA_Pendisplay_Spec,(ULONG)(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_Font)))
        ptr = MUIDEF_Calendar_Font;
    set(data->font,MUIA_String_Contents,(ULONG)ptr);

    v = (val = (ULONG *)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_UseDaysNamesBackground)) ?
        *val : MUIDEF_Calendar_UseDaysNamesBackground;
    set(data->useDaysNamesBackground,MUIA_Selected,v);

    v = (val = (ULONG *)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_UseDaysBackground)) ?
        *val : MUIDEF_Calendar_UseDaysBackground;
    set(data->useDaysBackground,MUIA_Selected,v);

    v = (val = (ULONG *)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_UseDaysFillPen)) ?
        *val : MUIDEF_Calendar_UseDaysFillPen;
    set(data->useDaysFillPen,MUIA_Selected,v);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_SundaysPen)))
        ptr = MUIDEF_Calendar_SundaysPen;
    set(data->sundaysPen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    if (!(ptr = (APTR)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_AnniversaryPen)))
        ptr = MUIDEF_Calendar_AnniversaryPen;
    set(data->anniversaryPen,MUIA_Pendisplay_Spec,(ULONG)ptr);

    v = (val = (ULONG *)DoMethod(cfg,MUIM_Dataspace_Find,MUICFG_Calendar_WeekStartDay)) ?
        *val : lib_locale->loc_CalendarType;
    set(data->wsd,MUIA_Calendar_StartDay,*val);

    return 0;
}

/***********************************************************************/

static ULONG
mGadgetsToConfig(struct IClass *cl,Object *obj,struct MUIP_Settingsgroup_GadgetsToConfig *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register Object         *cfg = msg->configdata;
    APTR                    ptr;
    ULONG                   val;

    get(data->daysNamesTextPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysNamesTextPen);

    get(data->daysNamesBackground,MUIA_Imagedisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysNamesBackground);

    get(data->daysNamesDetailPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysNamesDetailPen);

    get(data->daysBackground,MUIA_Imagedisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysBackground);

    get(data->daysTextPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysTextPen);

    get(data->daysShinePen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysShinePen);

    get(data->daysFillPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysFillPen);

    get(data->useDaysNamesBackground,MUIA_Selected,&val);
    DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)&val,sizeof(val),MUICFG_Calendar_UseDaysNamesBackground);

    get(data->useDaysBackground,MUIA_Selected,&val);
    DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)&val,sizeof(val),MUICFG_Calendar_UseDaysBackground);
    if (lib_flags & BASEFLG_MUI4)
    {
        get(data->daysBackground,MUIA_Framedisplay_Spec,&ptr);
        if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_DaysFrame);
    }

    get(data->useDaysFillPen,MUIA_Selected,&val);
    DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)&val,sizeof(val),MUICFG_Calendar_UseDaysFillPen);

    get(data->sundaysPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_SundaysPen);

    get(data->anniversaryPen,MUIA_Pendisplay_Spec,&ptr);
    if (ptr) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen(ptr)+1,MUICFG_Calendar_AnniversaryPen);

    get(data->font,MUIA_String_Contents,&ptr);
    if (*((STRPTR)ptr)) DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)ptr,strlen((STRPTR)ptr)+1,MUICFG_Calendar_Font);
    else DoMethod(cfg,MUIM_Dataspace_Remove,MUICFG_Calendar_Font);

    get(data->wsd,MUIA_Calendar_StartDay,&val);
    DoMethod(cfg,MUIM_Dataspace_Add,(ULONG)&val,sizeof(val),MUICFG_Calendar_WeekStartDay);

    return 0;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->ewin)
    {
        DoMethod(_app(obj),OM_REMMEMBER,(ULONG)data->ewin);
        MUI_DisposeObject(data->ewin);
        data->ewin = NULL;
    }

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

#ifndef MUIA_Window_ShowPopup
#define MUIA_Window_ShowPopup 0x8042324e
#endif

#ifndef MUIA_Window_ShowSnapshot
#define MUIA_Window_ShowSnapshot 0x80423c55
#endif

#ifndef MUIA_Window_ShowPrefs
#define MUIA_Window_ShowPrefs 0x8042e262
#endif

#ifndef MUIA_Window_ShowIconify
#define MUIA_Window_ShowIconify 0x8042bc26
#endif

#ifndef MUIA_Window_ShowAbout
#define MUIA_Window_ShowAbout 0x80429c1e
#endif

#ifndef MUIA_Window_ShowJump
#define MUIA_Window_ShowJump 0x80422f40
#endif

#ifndef MUIA_Window_AllowTopMenus
#define MUIA_Window_AllowTopMenus 0x8042fe69
#endif

static ULONG
mSample(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->ewin)
    {
        data->ewin = WindowObject,
            MUIA_Window_Title,          getString(Msg_Sample_WinTitle),
	        MUIA_Window_ShowIconify,    FALSE,
    	    MUIA_Window_ShowPopup,      FALSE,
    	    MUIA_Window_ShowSnapshot,   FALSE,
    	    MUIA_Window_ShowPrefs,      FALSE,
            MUIA_Window_AllowTopMenus,  FALSE,
            MUIA_Window_ShowJump,       FALSE,

            WindowContents, VGroup,
                Child, HGroup,
                    Child, wspace(30),
                    Child, CalendarObject,
                        MUIA_Calendar_ShowToday, TRUE,
                    End,
                    Child, wspace(30),
                End,
                Child, VSpace(0),
            End,
        End;

        if (data->ewin)
        {
            DoMethod(_app(obj),OM_ADDMEMBER,(ULONG)data->ewin);
            DoMethod(data->ewin,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,MUIV_Notify_Self,3,MUIM_Set,MUIA_Window_Open,FALSE);
        }
    }

    if (data->ewin) set(data->ewin,MUIA_Window_Open,TRUE);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                                return mNew(cl,obj,(APTR)msg);
        case OM_GET:                                return mGet(cl,obj,(APTR)msg);

        case MUIM_Settingsgroup_ConfigToGadgets:    return mConfigToGadgets(cl,obj,(APTR)msg);
        case MUIM_Settingsgroup_GadgetsToConfig:    return mGadgetsToConfig(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                          return mCleanup(cl,obj,(APTR)msg);

        case MUIM_Calendar_MCPSample:               return mSample(cl,obj,(APTR)msg);

        default:                                    return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initMCP(void)
{
    if (lib_class = MUI_CreateCustomClass(lib_base,MUIC_Mccprefs,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (MUIMasterBase->lib_Version>=20)
            lib_class->mcc_Class->cl_ID = lib_name;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/
