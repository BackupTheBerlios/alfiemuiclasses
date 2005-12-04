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
#include "Calendar.mcc_rev.h"

/***********************************************************************/

struct data
{
    Object *ctrlg;
    Object *month;
    Object *year;
    Object *up;
    Object *down;
    Object *hbar1;
    Object *days;
    Object *hbar2;
    Object *today;

    ULONG  d;
    ULONG  m;
    ULONG  y;
    ULONG  greg;
    ULONG  tclick;

    ULONG  flags;
};

enum
{
    FLG_Setup = 1<<0,
    FLG_Bars  = 1<<1,
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *month = NULL, *year = NULL, *up = NULL, *down = NULL, *days, *ctrlg, *hbar1 = NULL, *hbar2 = NULL, *today, *btoday = NULL;
    register ULONG  small, hfixed, ddefault, tclick, input;

    if (GetTagData(MUIA_Calendar_MinVer,0,msg->ops_AttrList)>lib_version) return 0;

    small    = GetTagData(MUIA_Calendar_Short,FALSE,msg->ops_AttrList);
    hfixed   = GetTagData(MUIA_Calendar_FixedHoriz,FALSE,msg->ops_AttrList);
    ddefault = GetTagData(MUIA_Calendar_SetDaysAsDefault,FALSE,msg->ops_AttrList);
    tclick   = GetTagData(MUIA_Calendar_TodayClick,FALSE,msg->ops_AttrList);
    input    = GetTagData(MUIA_Calendar_Input,TRUE,msg->ops_AttrList);

    if (!GetTagData(MUIA_Calendar_OnlyDays,FALSE,msg->ops_AttrList))
    {
        ctrlg = HGroup,
            Child, (ULONG)(month = MUI_MakeObject(MUIO_Cycle,0,small ? (ULONG)lib_abmonthesArray : (ULONG)lib_monthesArray)),
            Child, (ULONG)HGroup,
                MUIA_Group_HorizSpacing, 0,
                Child, (ULONG)(year = yearsObject, End),
                Child, (ULONG)(up = oibutton("\33I[6:31]")),
                Child, (ULONG)(down = oibutton("\33I[6:30]")),
            End,
        End;

        if (!ctrlg || !(hbar1 = RectangleObject, MUIA_Rectangle_HBar, TRUE, MUIA_Weight, 0, End))
        {
            if (ctrlg) MUI_DisposeObject(ctrlg);

            return 0;
        }
    }
    else ctrlg = NULL;

    if (GetTagData(MUIA_Calendar_ShowToday,FALSE,msg->ops_AttrList))
    {
        REGARRAY UBYTE buf[128];

        CALFormatDate(lib_locale,buf,sizeof(buf),lib_locale->loc_ShortDateFormat,CALGetToday());

        today = HGroup,
            MUIA_Group_Spacing,4,
            Child, HSpace(0),
            Child, btoday = oibutton(GetLocaleStr(lib_locale,TODAYSTR)),
            Child, TextObject, MUIA_Text_PreParse, "\33c", MUIA_Text_Contents, buf, MUIA_Text_SetMax, TRUE, End,
            Child, HSpace(0),
        End;

        if (!today || !(hbar2 = RectangleObject, MUIA_Rectangle_HBar, TRUE, MUIA_Weight, 0, End))
        {
            if (today)
            {
                MUI_DisposeObject(today);
                today = NULL;
            }
        }
    }
    else today = NULL;

    if (obj = (Object *)DoSuperNew(cl,obj,
            small ? MUIA_Font : TAG_IGNORE, MUIV_Font_Tiny,

            Child, (ULONG)(days = daysObject,
                MUIA_Calendar_FixedHoriz,       hfixed,
                MUIA_Calendar_Short,            small,
                MUIA_Calendar_SetDaysAsDefault, ddefault,
                MUIA_Calendar_Input,            input,
            End),

            TAG_MORE,(ULONG)msg->ops_AttrList))
    {
        register struct data    *data = INST_DATA(cl,obj);
        register struct TagItem *tag;

        data->ctrlg  = ctrlg;
        data->hbar1  = hbar1;
        data->days   = days;
        data->hbar2  = hbar2;
        data->today  = today;

        data->tclick = tclick;
        data->flags |= FLG_Bars;

        if (ctrlg)
        {
            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)ctrlg);
            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)hbar1);
            DoSuperMethod(cl,obj,MUIM_Group_Sort,(ULONG)ctrlg,(ULONG)hbar1,(ULONG)days,NULL);

            data->month = month;
            data->year  = year;
            data->up    = up;
            data->down  = down;

            set(month,MUIA_CycleChain,TRUE);

            DoMethod(month,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,(ULONG)obj,3,MUIM_Set,MUIA_Calendar_InternalMonth,MUIV_TriggerValue);
            DoMethod(year,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,(ULONG)obj,1,MUIM_Calendar_SetStringYear);
            DoMethod(up,MUIM_Notify,MUIA_Timer,MUIV_EveryTime,(ULONG)obj,4,MUIM_Calendar_TimerYear,-1,MUIV_TriggerValue);
            DoMethod(down,MUIM_Notify,MUIA_Timer,MUIV_EveryTime,(ULONG)obj,4,MUIM_Calendar_TimerYear,1,MUIV_TriggerValue);
        }

        if (today)
        {
            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)hbar2);
            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)today);

            DoMethod(btoday,MUIM_Notify,MUIA_Pressed,FALSE,(ULONG)obj,1,MUIM_Calendar_Today);
        }

        if (tag = FindTagItem(MUIA_Calendar_Greg,msg->ops_AttrList))
        {
            set(obj,MUIA_Calendar_Greg,tag->ti_Data);
        }
        else
        {
            register struct TagItem *dtag, *mtag, *ytag;
            register ULONG          today;

            today = CALGetToday();

            dtag = FindTagItem(MUIA_Calendar_Day,msg->ops_AttrList);
            mtag = FindTagItem(MUIA_Calendar_Month,msg->ops_AttrList);
            ytag = FindTagItem(MUIA_Calendar_Year,msg->ops_AttrList);

            if (dtag || mtag || ytag)
            {
                ULONG d, m, y;

                CALGregToDate(today,&d,&m,&y);

                if (dtag) d = dtag->ti_Data;
                if (mtag) m = mtag->ti_Data;
                if (ytag) y = ytag->ti_Data;

                SetAttrs(obj,MUIA_Calendar_Day,   d,
                             MUIA_Calendar_Month, m,
                             MUIA_Calendar_Year,  y,
                             TAG_DONE);
            }
            else
            {
                set(obj,MUIA_Calendar_Greg,today);
            }
        }
     }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!(data->flags & FLG_Bars))
    {
        if (data->hbar1) MUI_DisposeObject(data->hbar1);
        if (data->hbar2) MUI_DisposeObject(data->hbar2);
    }

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);

    switch(msg->opg_AttrID)
    {
        case MUIA_Calendar_InternalMonth: *msg->opg_Storage = data->m-1;        return TRUE;

        case MUIA_Calendar_Day:           *msg->opg_Storage = data->d;          return TRUE;
        case MUIA_Calendar_Month:         *msg->opg_Storage = data->m;          return TRUE;
        case MUIA_Calendar_Year:          *msg->opg_Storage = data->y;          return TRUE;
        case MUIA_Calendar_Greg:          *msg->opg_Storage = data->greg;       return TRUE;

        case MUIA_Calendar_Click:         *msg->opg_Storage = data->d;          return TRUE;
        case MUIA_Calendar_DoubleClick:   *msg->opg_Storage = data->d;          return TRUE;

        case MUIA_Calendar_Base:          *msg->opg_Storage = (ULONG)lib_base;  return TRUE;

        case MUIA_Version:                *msg->opg_Storage = lib_version;      return TRUE;
        case MUIA_Revision:               *msg->opg_Storage = lib_revision;     return TRUE;

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;
    struct TagItem          *tstate;
    register ULONG          changeDate = FALSE;

    for (tstate = msg->ops_AttrList; tag = NextTagItem(&tstate); )
    {
        register ULONG tidata = tag->ti_Data;

        switch(tag->ti_Tag)
        {
            case MUIA_Calendar_Day:
                if (tidata==data->d)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }
                data->d = tidata;
                changeDate = TRUE;
                break;

            case MUIA_Calendar_Month:
                if (tidata==data->m)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }
                data->m = tidata;
                changeDate = TRUE;
                break;

            case MUIA_Calendar_InternalMonth:
                if (tidata+1==data->m)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }
                data->m = tidata+1;
                changeDate = TRUE;
                break;

            case MUIA_Calendar_Year:
                if (tidata==data->y)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }
                data->y = tidata;
                changeDate = TRUE;
                break;

            case MUIA_Calendar_Greg:
                if (tidata==data->greg)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }

                if (tidata<MINGREG) tidata = MINGREG;
                else if (tidata>MAXGREG) tidata = MAXGREG;

                data->greg = tidata;
                CALGregToDate(data->greg,&data->d,&data->m,&data->y);
                changeDate = TRUE;
                break;

            case MUIA_Calendar_Click:
                SetAttrs(obj,MUIA_Calendar_Day,tidata,MUIA_Group_Forward,TRUE,TAG_DONE);
                break;

            case MUIA_Calendar_DoubleClick:
                data->d--;
                SetAttrs(obj,MUIA_Calendar_Day,tidata,MUIA_Group_Forward,TRUE,TAG_DONE);
                break;

            case MUIA_Calendar_Bars:
                if ((!data->ctrlg && !data->today) || BOOLSAME(data->flags & FLG_Bars,tidata)) tag->ti_Data = TAG_IGNORE;
                else
                {
                    if (data->flags & FLG_Setup) DoSuperMethod(cl,obj,MUIM_Group_InitChange);

                    if (tidata)
                    {
                        if (data->today) DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)data->hbar2);

                        if (data->ctrlg)
                        {
                            DoSuperMethod(cl,obj,OM_ADDMEMBER,(ULONG)data->hbar1);
                            DoSuperMethod(cl,obj,MUIM_Group_Sort,(ULONG)data->ctrlg,(ULONG)data->hbar1,(ULONG)data->days,(ULONG)data->hbar2,(ULONG)data->today,NULL);
                        }
                        else DoSuperMethod(cl,obj,MUIM_Group_Sort,(ULONG)data->days,(ULONG)data->hbar2,(ULONG)data->today,NULL);

                        data->flags |= FLG_Bars;
                    }
                    else
                    {
                        if (data->ctrlg) DoSuperMethod(cl,obj,OM_REMMEMBER,(ULONG)data->hbar1);
                        if (data->today) DoSuperMethod(cl,obj,OM_REMMEMBER,(ULONG)data->hbar2);

                        data->flags &= ~FLG_Bars;
                    }

                    if (data->flags & FLG_Setup) DoSuperMethod(cl,obj,MUIM_Group_ExitChange);
                }
                break;

            default:
                break;
        }
    }

    if (changeDate)
    {
        CALValidateDate(&data->d,&data->m,&data->y);
        data->greg = CALDateToGreg(data->d,data->m,data->y);

        nnset(data->days,MUIA_Calendar_Day,data->d);
        set(data->month,MUIA_Cycle_Active,data->m-1);
        set(data->year,MUIA_Textinput_Integer,data->y);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    data->flags |= FLG_Setup;

    return TRUE;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    data->flags &= ~FLG_Setup;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mSetStringYear(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    ULONG                v;

    get(data->year,MUIA_Textinput_Integer,&v);
    set(obj,MUIA_Calendar_Year,v);

    return data->greg;
}

/***********************************************************************/

static ULONG
mAddDay(struct IClass *cl,Object *obj,struct MUIP_Calendar_AddDay *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    set(obj,MUIA_Calendar_Greg,data->greg+msg->delta);

    return data->greg;
}

/***********************************************************************/

static ULONG
mAddMonth(struct IClass *cl,Object *obj,struct MUIP_Calendar_AddMonth *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    ULONG                d, y;
    LONG                 m;

    d = data->d;
    m = data->m-1;
    y = data->y;

    m  = m+msg->delta;
    if (m>=0)
    {
        y += m/12;
        m  = m%12;
    }
    else
    {
        y -= (-m)/12;
        if ((m/12*12)!=m) y--;
        m = m%12+12;
    }

    m++;

    CALValidateDate(&d,(ULONG *)&m,&y);
    set(obj,MUIA_Calendar_Greg,CALDateToGreg(d,(ULONG)m,y));

    return data->greg;
}

/***********************************************************************/

static ULONG
mAddYear(struct IClass *cl,Object *obj,struct MUIP_Calendar_AddYear *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    ULONG                d, m, y;

    d = data->d;
    m = data->m;
    y = data->y+msg->delta;

    CALValidateDate(&d,&m,&y);

    set(obj,MUIA_Calendar_Greg,CALDateToGreg(d,m,y));

    return data->greg;
}

/***********************************************************************/

static ULONG
mTimerYear(struct IClass *cl,Object *obj,struct MUIP_Calendar_TimerYear *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       delta;

    delta = msg->delta;

    if (msg->timer>7)
        if (msg->timer>15)
            if (msg->timer>30) delta *= 30;
            else delta *= 10;
        else delta *= 2;

    set(obj,MUIA_Calendar_Year,data->y+delta);

    return data->greg;
}

/***********************************************************************/

static ULONG
mFormatDate(struct IClass *cl,Object *obj,struct MUIP_Calendar_FormatDate *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register STRPTR      fmt;

    if (msg->fmt==MUIV_Calendar_FormatDate_Long) fmt = FORMTDATEFMT_SYSTEM;
    else if (msg->fmt==MUIV_Calendar_FormatDate_Short) fmt = FORMTDATEFMT_SYSTEMSHORT;
         else fmt = msg->fmt;

    return (ULONG)CALFormatDate(lib_locale,msg->buf,msg->bufLen,fmt,data->greg);
}

/***********************************************************************/

static ULONG
mToday(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    set(obj,MUIA_Calendar_Greg,CALGetToday());

    if (data->tclick==MUIV_Calendar_TodayClick_Single) set(obj,MUIA_Calendar_Click,data->d);
    else if (data->tclick==MUIV_Calendar_TodayClick_Double) set(obj,MUIA_Calendar_DoubleClick,data->d);

    return data->greg;
}

/***********************************************************************/

static ULONG
mGetInfo(struct IClass *cl,Object *obj,struct MUIP_Calendar_GetInfo *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       greg, res = TRUE;
    ULONG                d, m, y;

    if (msg->greg==MUIV_Calendar_GetInfo_Greg_Default)
    {
        greg = data->greg;
        d = data->d;
        m = data->m;
        y = data->y;
    }
    else
        if (msg->greg==MUIV_Calendar_GetInfo_Greg_Today)
        {
            greg = CALGetToday();
            CALGregToDate(greg,&d,&m,&y);
        }
        else
        {
            greg = msg->greg;
            CALGregToDate(greg,&d,&m,&y);
        }

    switch (msg->info)
    {
        case MUIV_Calendar_GetInfo_Greg:
            *msg->storage[0] = data->greg;
            break;

        case MUIV_Calendar_GetInfo_WeekDay:
            *msg->storage[0] = CALGregWeekday(greg);
            break;

        case MUIV_Calendar_GetInfo_JulianDay:
            *msg->storage[0] = CALGetJulian(greg);
            break;

        case MUIV_Calendar_GetInfo_Today:
            *msg->storage[0] = CALGetToday();
            break;

        case MUIV_Calendar_GetInfo_Easter:
            CALGetEaster(y,msg->storage[0],msg->storage[1]);
            break;

        default:
            res = 0;
    }

    return res;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART
    switch(msg->MethodID)
    {
        case OM_NEW:                      return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:                  return mDispose(cl,obj,(APTR)msg);
        case OM_GET:                      return mGet(cl,obj,(APTR)msg);
        case OM_SET:                      return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:                  return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                return mCleanup(cl,obj,(APTR)msg);

        case MUIM_Calendar_SetStringYear: return mSetStringYear(cl,obj,(APTR)msg);
        case MUIM_Calendar_TimerYear:     return mTimerYear(cl,obj,(APTR)msg);

        case MUIM_Calendar_FormatDate:    return mFormatDate(cl,obj,(APTR)msg);
        case MUIM_Calendar_Today:         return mToday(cl,obj,(APTR)msg);
        case MUIM_Calendar_AddDay:        return mAddDay(cl,obj,(APTR)msg);
        case MUIM_Calendar_AddMonth:      return mAddMonth(cl,obj,(APTR)msg);
        case MUIM_Calendar_AddYear:       return mAddYear(cl,obj,(APTR)msg);
        case MUIM_Calendar_GetInfo:       return mGetInfo(cl,obj,(APTR)msg);

        default:                          return DoSuperMethodA(cl,obj,msg);
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

        lib_class->mcc_UserData = lib_base;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

