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
#include <devices/inputevent.h>

/***********************************************************************/

struct data
{
    struct MUI_EventHandlerNode eh;
    Object                      *parent;
    ULONG                       flags;
};

enum
{
    FLG_Handler = 1<<0,
    FLG_Active  = 1<<1,
};

/***********************************************************************/

static void
addHandler(struct IClass *cl,Object *obj,struct data *data)
{
    if (!(data->flags & FLG_Handler))
    {
        memset(&data->eh,0,sizeof(data->eh));
        data->eh.ehn_Class  = cl;
        data->eh.ehn_Object = obj;
        data->eh.ehn_Events = IDCMP_RAWKEY;
        DoMethod(_win(obj),MUIM_Window_AddEventHandler,(ULONG)&data->eh);

        data->flags |= FLG_Handler;
    }
}

/***********************************************************************/

static void
remHandler(Object *obj,struct data *data)
{
    if (data->flags & FLG_Handler)
    {
        DoMethod(_win(obj),MUIM_Window_RemEventHandler,(ULONG)&data->eh);

        data->flags &= ~FLG_Handler;
    }
}

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    return DoSuperNew(cl,obj,
            StringFrame,
            MUIA_CycleChain,            TRUE,
            MUIA_FixWidthTxt,           (ULONG)"XXXXXXX",
            MUIA_Textinput_MaxLen,      5,
            MUIA_Textinput_AcceptChars, (ULONG)"0123456789",
            MUIA_Textinput_Format,      MUIV_Textinput_Format_Right,
            MUIA_Textinput_IsNumeric,   TRUE,
            MUIA_Textinput_MinVal,      1752,
            MUIA_Textinput_MaxVal,      2400,
            TAG_MORE,(ULONG)msg->ops_AttrList);
}

/***********************************************************************/

static ULONG
mShow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    Object               *p;

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    for (p = obj; ;)
    {
        data->parent = p;
        get(p,MUIA_Parent,&p);
        if (!p) break;
    }

    return TRUE;
}

/***********************************************************************/

static ULONG
mActive(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    addHandler(cl,obj,data);
    data->flags |= FLG_Active;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mInactive(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    remHandler(obj,data);
    data->flags &= ~FLG_Active;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mHandleEvent(struct IClass *cl,Object *obj,struct MUIP_HandleEvent *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if ((data->flags & FLG_Active) && msg->imsg && (msg->imsg->Class==IDCMP_RAWKEY))
    {
        register LONG delta = 0;

        switch (msg->muikey)
        {
            case MUIKEY_UP:
                delta = -1;
                break;

            case MUIKEY_DOWN:
                delta = 1;
                break;

            case MUIKEY_NONE:
                if (msg->imsg->Class==IDCMP_RAWKEY)
                    if (msg->imsg->Code==NM_WHEEL_UP) delta = -1;
                    else if (msg->imsg->Code==NM_WHEEL_DOWN) delta = 1;
                break;
        }

        if (delta) DoMethod(data->parent,MUIM_Calendar_TimerYear,delta,0);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
     {
        case OM_NEW:           return mNew(cl,obj,(APTR)msg);

        case MUIM_Show:        return mShow(cl,obj,(APTR)msg);
        case MUIM_GoActive:    return mActive(cl,obj,(APTR)msg);
        case MUIM_GoInactive:  return mInactive(cl,obj,(APTR)msg);
        case MUIM_HandleEvent: return mHandleEvent(cl,obj,(APTR)msg);

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initYearsClass(void)
{
    if (lib_yearsClass = MUI_CreateCustomClass(NULL,MUIC_Textinput,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (lib_flags & BASEFLG_MUI20)
            lib_daysClass->mcc_Class->cl_ID = "Calendar_Years";

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

