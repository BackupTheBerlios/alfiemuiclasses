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

/***********************************************************************/

struct data
{
    Object *days[7];
    ULONG  wsd;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Group_Horiz,        TRUE,
            MUIA_Group_HorizSpacing, 1,
            MUIA_Group_SameSize,     TRUE,
            TAG_MORE,(ULONG)msg->ops_AttrList))
    {
        struct data *data = INST_DATA(cl,obj);
        int         i;

        for (i= 0; i<7; i++)
        {
            Object *o;

            o = TextObject,
                    MUIA_CycleChain,    TRUE,
                    MUIA_Font,          MUIV_Font_Button,
                    MUIA_InputMode,     MUIV_InputMode_Immediate,
                    ButtonFrame,
                    MUIA_Background,    MUII_ButtonBack,
                    MUIA_Text_Contents, GetLocaleStr(lib_locale,ABDAY_1+i),
                    MUIA_Text_PreParse, MUIX_C,
                    MUIA_Text_SetMax,   TRUE,
            End;

            if (o)
            {
                DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)o);
                DoMethod(o,MUIM_Notify,MUIA_Selected,TRUE,(ULONG)obj,3,MUIM_Set,MUIA_Calendar_StartDay,i);
                data->days[i] = o;
            }
            else
            {
                CoerceMethod(cl->cl_Super,obj,OM_DISPOSE);
                return 0;
            }
        }
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct TagItem *tag;

    if (tag = FindTagItem(MUIA_Calendar_StartDay,msg->ops_AttrList))
    {
        struct data *data = INST_DATA(cl,obj);
        ULONG       wsd = tag->ti_Data;
        int         i;

        if (wsd>6) wsd = 6;

        for (i = 0; i<7; i++) if (i!=wsd) nnset(data->days[i],MUIA_Selected,FALSE);
        nnset(data->days[wsd],MUIA_Selected,TRUE);

        data->wsd = wsd;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_Calendar_StartDay: *msg->opg_Storage = data->wsd; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:  return mNew(cl,obj,(APTR)msg);
        case OM_SET:  return mSets(cl,obj,(APTR)msg);
        case OM_GET:  return mGet(cl,obj,(APTR)msg);

        default:      return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initWSD(void)
{
    if (lib_wsd = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (MUIMasterBase->lib_Version>=20)
            lib_wsd->mcc_Class->cl_ID = "Calendar_WSD";

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/
