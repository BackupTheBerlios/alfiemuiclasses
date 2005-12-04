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

#define SHORTBLANKS " "
#define BLANKS      "  "

#define VSP         2

/***********************************************************************/

struct box
{
    WORD width;
    WORD height;
    WORD left;
    WORD top;

    WORD delta;

    WORD cleft;
    WORD ctop;
    WORD cright;
    WORD cbottom;

    WORD bleft;
    WORD btop;
    WORD bright;
    WORD bbottom;

    LONG pen;
};

struct data
{
    Object                      *parent;

    ULONG                       d;
    ULONG                       m;
    ULONG                       y;
    ULONG                       greg;
    ULONG                       od;
    ULONG                       first;
    ULONG                       days;

    ULONG                       wsd;

    ULONG                       minY;
    ULONG                       width;
    ULONG                       height;
    ULONG                       blankWidth;
    ULONG                       wmax;
    ULONG                       hmax;
    ULONG                       hsp;
    ULONG                       vsp;
    ULONG                       nns;

    LONG                        daysNamesTextPen;
    APTR                        daysNamesBackground;
    LONG                        daysNamesDetailPen;
    APTR                        daysBackground;
    LONG                        dayTextPen;
    LONG                        dayShinePen;
    LONG                        dayFillPen;
    LONG                        holydaysPen;
    LONG                        anniversaryPen;

    struct TextFont             *font;
    struct RastPort             rp;

    struct MUI_EventHandlerNode eh;

    struct box                  dbox[31];
    WORD                        dnwidth[7];
    WORD                        dndelta[7];

    ULONG                       secs;
    ULONG                       micros;

    struct MUI_FrameSpec 	    daysFrameSpec;

    ULONG                       flags;
};

enum
{
    FLG_Setup       = 1<<0,
    FLG_Show        = 1<<1,
    FLG_FixedHoriz  = 1<<2,
    FLG_CloseFont   = 1<<3,

    FLG_Input       = 1<<4,
    FLG_Short       = 1<<5,
    FLG_ShowDefault = 1<<6,
    FLG_SkipDraw    = 1<<7,
    FLG_TableDone   = 1<<8,
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,MUIA_CycleChain,TRUE,TAG_MORE,(ULONG)msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        data->vsp = VSP;

        msg->MethodID = OM_SET;
        DoMethodA(obj,(Msg)msg);
        msg->MethodID = OM_NEW;

        if (GetTagData(MUIA_Calendar_FixedHoriz,FALSE,msg->ops_AttrList))
            data->flags |= FLG_FixedHoriz;

        if (GetTagData(MUIA_Calendar_SetDaysAsDefault,FALSE,msg->ops_AttrList))
            data->flags |= FLG_ShowDefault;

        if (GetTagData(MUIA_Calendar_Short,FALSE,msg->ops_AttrList))
            data->flags |= FLG_Short;

        if (GetTagData(MUIA_Calendar_Input,TRUE,msg->ops_AttrList))
            data->flags |= FLG_Input;
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;
    struct TagItem          *tstate;
    ULONG                   redraw = FALSE, mode = MADF_DRAWOBJECT;

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

                if (!redraw)
                {
                    redraw = TRUE;
                    mode = MADF_DRAWUPDATE;
                }
                break;

            case MUIA_Calendar_Month:
                if (tidata==data->m)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }

                data->m = tidata;

                redraw = TRUE;
                break;

            case MUIA_Calendar_InternalMonth:
                if (tidata+1==data->m)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }

                data->m = tidata+1;

                redraw = TRUE;
                break;

            case MUIA_Calendar_Year:
                if (tidata==data->y)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }

                data->y = tidata;

                redraw = TRUE;
                break;

            case MUIA_Calendar_Greg:
            {
                ULONG d, m, y;

                if (tidata==data->greg)
                {
                    tag->ti_Tag = TAG_IGNORE;
                    break;
                }

                CALGregToDate(tidata,&d,&m,&y);

                if ((!redraw || mode==MADF_DRAWUPDATE) && m==data->m && y==data->y)
                    mode = MADF_DRAWUPDATE;

                data->d = d;
                data->m = m;
                data->y = y;

                redraw = TRUE;
                break;
            }

            default:
                break;
        }
    }

    if (redraw)
    {
        CALValidateDate(&data->d,&data->m,&data->y);
        data->greg  = CALDateToGreg(data->d,data->m,data->y);
        data->first = CALDateWeekday(1,data->m,data->y);
        data->days  = CALGetMonthDays(data->m,data->y);

        if (data->flags & FLG_Show) MUI_Redraw(obj,mode);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    STRPTR               ptr;
    ULONG                *val, doBars, barsv = 0;

    if (lib_flags & BASEFLG_MUI4)
    {
        if (!DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysBackground,(ULONG)&ptr))
            ptr = MUIDEF_Calendar_DaysBackground_MUI4;
        SetSuperAttrs(cl,obj,MUIA_Background,(ULONG)ptr,MUIA_NoNotify,TRUE,TAG_DONE);
        data->daysBackground = ptr;

        if (!DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysFrame,(ULONG)&ptr))
            ptr = MUIDEF_Calendar_DaysFrame;
        CopyMem(ptr,&data->daysFrameSpec,sizeof(data->daysFrameSpec));
        SetSuperAttrs(cl,obj,MUIA_Group_Forward,FALSE,MUIA_Frame,(ULONG)&data->daysFrameSpec,TAG_DONE);

        doBars = TRUE;
        barsv  = ptr[0]=='0';
    }
    else
    {
        if ((DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_UseDaysBackground,(ULONG)&val) ? *val : MUIDEF_Calendar_UseDaysBackground) &&
            DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysBackground,(ULONG)&ptr))
        {
            SetSuperAttrs(cl,obj,MUIA_Background,(ULONG)ptr,MUIA_NoNotify,TRUE,TAG_DONE);
            data->daysBackground = ptr;
        }
        else
        {
            get(obj,MUIA_Parent,&ptr);
            SetSuperAttrs(cl,obj,MUIA_Background,ptr ? (ULONG)_backspec(ptr) : MUII_WindowBack,MUIA_NoNotify,TRUE,TAG_DONE);
            data->daysBackground = _backspec(ptr);
        }

        doBars = FALSE;
    }

    if (!(DoSuperMethodA(cl,obj,(Msg)msg))) return FALSE;

    if (doBars)
    {
        Object *parent;

        get(obj,MUIA_Parent,&parent);
        set(parent,MUIA_Calendar_Bars,barsv);
    }

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysNamesTextPen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_DaysNamesTextPen;
    data->daysNamesTextPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    if ((DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_UseDaysNamesBackground,(ULONG)&val) ? *val : MUIDEF_Calendar_UseDaysNamesBackground) &&
        DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysNamesBackground,(ULONG)&ptr))
    {
        data->daysNamesBackground = ptr;
    }
    else
    {
        data->daysNamesBackground = 0;
    }

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysNamesDetailPen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_DaysNamesDetailPen;
    data->daysNamesDetailPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysTextPen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_DaysTextPen;
    data->dayTextPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysShinePen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_DaysShinePen;
    data->dayShinePen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    if (DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_UseDaysFillPen,(ULONG)&val) ? *val : MUIDEF_Calendar_UseDaysFillPen)
    {
        if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_DaysFillPen,(ULONG)&ptr))
            ptr = MUIDEF_Calendar_DaysFillPen;
        data->dayFillPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);
    }
    else data->dayFillPen = -1;

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_SundaysPen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_SundaysPen;
    data->holydaysPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    if (!DoMethod(obj,MUIM_GetConfigItem,MUICFG_Calendar_AnniversaryPen,(ULONG)&ptr))
        ptr = MUIDEF_Calendar_AnniversaryPen;
    data->anniversaryPen = MUI_ObtainPen(muiRenderInfo(obj),(struct MUI_PenSpec *)ptr,0);

    data->wsd = DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_WeekStartDay,(ULONG)&val) ?
        *val : lib_locale->loc_CalendarType;

    if ((data->flags & FLG_Short) ||
        !DoSuperMethod(cl,obj,MUIM_GetConfigItem,MUICFG_Calendar_Font,(ULONG)&ptr) ||
        !(data->font = openFont((STRPTR)ptr))) data->font = _font(obj);
    else data->flags |= FLG_CloseFont;

    data->flags |= FLG_Setup;

    return TRUE;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->daysNamesTextPen!=-1)   MUI_ReleasePen(muiRenderInfo(obj),data->daysNamesTextPen);
    if (data->daysNamesDetailPen!=-1) MUI_ReleasePen(muiRenderInfo(obj),data->daysNamesDetailPen);
    if (data->dayTextPen!=-1)         MUI_ReleasePen(muiRenderInfo(obj),data->dayTextPen);
    if (data->dayShinePen!=-1)        MUI_ReleasePen(muiRenderInfo(obj),data->dayShinePen);
    if (data->dayFillPen!=-1)         MUI_ReleasePen(muiRenderInfo(obj),data->dayFillPen);
    if (data->holydaysPen!=-1)        MUI_ReleasePen(muiRenderInfo(obj),data->holydaysPen);
    if (data->anniversaryPen!=-1)     MUI_ReleasePen(muiRenderInfo(obj),data->anniversaryPen);

    if (data->flags & FLG_CloseFont) CloseFont(data->font);

    data->flags &= ~(FLG_Setup|FLG_CloseFont);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mShow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    if (data->flags & FLG_Input)
    {
        memset(&data->eh,0,sizeof(data->eh));
        data->eh.ehn_Class  = cl;
        data->eh.ehn_Object = obj;
        data->eh.ehn_Events = IDCMP_MOUSEBUTTONS|IDCMP_RAWKEY;
        DoMethod(_win(obj),MUIM_Window_AddEventHandler,(ULONG)&data->eh);
    }

    get(obj,MUIA_Parent,&data->parent);

    if (data->flags & FLG_ShowDefault)
        DoMethod(_app(obj),MUIM_Application_PushMethod,(ULONG)_win(obj),3,MUIM_Set,MUIA_Window_DefaultObject,(ULONG)obj);

    CopyMem(_rp(obj),&data->rp,sizeof(data->rp));

    data->flags |= FLG_Show;

    return TRUE;
}

/***********************************************************************/

static ULONG
mHide(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->flags & FLG_Input) DoMethod(_win(obj),MUIM_Window_RemEventHandler,(ULONG)&data->eh);

    data->flags &= ~(FLG_Show|FLG_TableDone);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mAskMinMax(struct IClass *cl,Object *obj,struct MUIP_AskMinMax *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct RastPort      rp;
    struct TextExtent    info;
    register STRPTR      blanks;
    register int         i;
    register UWORD       width, height, wmax, hmax, vsp = data->vsp;

    DoSuperMethodA(cl,obj,(Msg)msg);

    copymem(&rp,&_screen(obj)->RastPort,sizeof(rp));
    SetFont(&rp,data->font);

    blanks = (data->flags & FLG_Short) ? SHORTBLANKS : BLANKS;

    TextExtent(&rp,blanks,strlen(blanks),&info);
    data->blankWidth = info.te_Width;

    wmax = hmax = 0;

    for (i = 0; i<7; i++)
    {
        UBYTE dnb[2], *dn;

        if (data->flags & FLG_Short)
        {
            dnb[0] = lib_daysArray[i][0];
            dnb[1] = 0;
            dn = dnb;
        }
        else dn = lib_daysArray[i];

        TextExtent(&rp,dn,strlen(dn),&info);

        if (info.te_Width>wmax)  wmax = info.te_Width;
        if (info.te_Height>hmax) hmax = info.te_Height;

        data->dnwidth[i] = info.te_Width;
    }

    for (i = 0; i<31; i++)
    {
        TextExtent(&rp,lib_dayNumbers[i],i<9 ? 1 : 2,&info);

        if (info.te_Width>wmax)  wmax = info.te_Width;
        if (info.te_Height>hmax) hmax = info.te_Height;

        data->dbox[i].width  = info.te_Width;
        data->dbox[i].height = info.te_Height;
    }

    vsp = hmax/2;
    if (vsp<2) vsp = 2;
    data->vsp = vsp;

    width  = 7*wmax+8*data->blankWidth;
    height = 2+hmax+1+1+vsp+6*(hmax+vsp);

    data->width  = width;
    data->height = height;
    data->wmax   = wmax;
    data->hmax   = hmax;
    data->minY   = info.te_Extent.MinY;

    msg->MinMaxInfo->MinWidth  += width;
    msg->MinMaxInfo->DefWidth  += width;
    if (data->flags & FLG_FixedHoriz) msg->MinMaxInfo->MaxWidth += width;
    else msg->MinMaxInfo->MaxWidth = MBQ_MUI_MAXMAX;

    msg->MinMaxInfo->MinHeight += height;
    msg->MinMaxInfo->DefHeight += height;
    msg->MinMaxInfo->MaxHeight += height;

    if (width>height) msg->MinMaxInfo->MaxHeight += (width-height)/6;

    return 0;
}

/***********************************************************************/

static ULONG
mDraw(struct IClass *cl,Object *obj,struct MUIP_Draw *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct RastPort      *rp;
    int                  i, wsd;
    UWORD                wmax, hmax;

    if (data->flags & FLG_SkipDraw) return 0;

    wsd  = data->wsd;
    wmax = data->wmax;
    hmax = data->hmax;

    DoSuperMethodA(cl,obj,(APTR)msg);

    rp = &data->rp;
    SetFont(rp,data->font);

    if (msg->flags & MADF_DRAWOBJECT)
    {
        int   first, days;
        UWORD x, y, blankWidth, vsp;

        blankWidth = data->blankWidth+(_mwidth(obj)-data->width)/8;
        vsp        = data->vsp+(_mheight(obj)-data->height)/8;

        if (data->daysNamesBackground)
        {
            data->flags |= FLG_SkipDraw;

            SetSuperAttrs(cl,obj,MUIA_Background,(ULONG)data->daysNamesBackground,MUIA_NoNotify,TRUE,TAG_DONE);

            DoMethod(obj,MUIM_DrawBackground,
                _mleft(obj),
                _mtop(obj),
                _mwidth(obj),
                data->hmax+2,
                _mleft(obj),
                _mtop(obj),
                0);

            SetSuperAttrs(cl,obj,MUIA_Background,(ULONG)data->daysBackground,MUIA_NoNotify,TRUE,TAG_DONE);
            data->flags &= ~FLG_SkipDraw;
        }

        SetAPen(rp,MUIPEN(data->daysNamesTextPen));

        for (i = 0; i<7; i++)
        {
            UBYTE dnb[2], *dn, d;

            d = (i+wsd)%7;

            if (data->flags & FLG_Short)
            {
                dnb[0] = lib_daysArray[d][0];
                dnb[1] = 0;
                dn = dnb;
            }
            else dn = lib_daysArray[d];

            if (!(data->flags & FLG_TableDone))
            {
                UWORD k = wmax-data->dnwidth[d];

                if (k) k = (k%2) ? k/2+1 : k/2;
                data->dndelta[d] = k;
            }

            Move(rp,_mleft(obj)+blankWidth+i*(wmax+blankWidth)+data->dndelta[d],_mtop(obj)+2-data->minY);
            Text(rp,dn,strlen(dn));
        }

        SetAPen(rp,MUIPEN(data->daysNamesDetailPen));
        Move(rp,_mleft(obj),_mtop(obj)+data->hmax+2);
        Draw(rp,_mright(obj),_mtop(obj)+data->hmax+2);

        SetAPen(rp,MUIPEN(data->dayTextPen));

        first = (7-wsd+data->first)%7;
        days  = data->days;

        for (i = 0; i<days; i++)
        {
            ULONG cpen = FALSE;
            int   c, r, d;

            c = (i+first)%7; // col
            r = (i+first)/7; // row
            d = (c+wsd)%7;   // real row (day)

            if (!(data->flags & FLG_TableDone))
            {
                UWORD k = wmax-data->dbox[i].width;

                if (k) k = (k%2) ? k/2+1 : k/2;
                data->dbox[i].delta = k;
            }

            if (lib_holidays)
            {
                BYTE b = lib_holidays[((data->m-1)<<8)|i];

                if (b)
                {
                    if (b>0)
                    {
                        data->dbox[i].pen = data->holydaysPen;
                        SetAPen(rp,MUIPEN(data->holydaysPen));
                    }
                    else
                    {
                        data->dbox[i].pen = data->anniversaryPen;
                        SetAPen(rp,MUIPEN(data->anniversaryPen));
                    }

                    cpen = TRUE;
                }
            }

            if (!cpen && (i+data->first)%7==0)
            {
                data->dbox[i].pen = data->holydaysPen;
                SetAPen(rp,MUIPEN(data->holydaysPen));
                cpen = TRUE;
            }

            if (!cpen) data->dbox[i].pen = data->dayTextPen;

            x = _mleft(obj)+blankWidth+c*(wmax+blankWidth);
            y = 2+hmax+2+vsp+r*(hmax+vsp)+vsp;

            Move(rp,x+data->dbox[i].delta,_mtop(obj)+y-data->minY);
            Text(rp,lib_dayNumbers[i],i<9 ? 1 : 2);

            if (cpen) SetAPen(rp,MUIPEN(data->dayTextPen));

            data->dbox[i].cleft   = x-blankWidth/2;
            data->dbox[i].cright  = x+wmax+blankWidth/2;
            data->dbox[i].ctop    = _mtop(obj)+y-vsp/2-1;
            data->dbox[i].cbottom = _mtop(obj)+y-data->minY+vsp/2+2;

            data->dbox[i].bleft   = x-1;
            data->dbox[i].bright  = data->dbox[i].bleft+wmax;
            data->dbox[i].btop    = data->dbox[i].ctop;
            data->dbox[i].bbottom = data->dbox[i].cbottom;

            data->dbox[i].left    = x+data->dbox[i].delta;
            data->dbox[i].top     = _mtop(obj)+y-data->minY;
        }

        data->flags |= FLG_TableDone;

        data->od = 0;
    }

    if (msg->flags & (MADF_DRAWOBJECT|MADF_DRAWUPDATE) && (data->flags & FLG_Input))
    {
        if (data->od>0)
        {
            i = data->od-1;

            if (data->dayFillPen!=-1)
            {
                UWORD l, t;

                l = data->dbox[i].bleft;
                t = data->dbox[i].btop;

                DoMethod(obj,MUIM_DrawBackground,l,t,
                    data->dbox[i].bright-data->dbox[i].bleft+1,
                    data->dbox[i].bbottom-data->dbox[i].btop+1,
                    l,t,0);
            }

            SetAPen(rp,MUIPEN(data->dbox[i].pen));
            Move(rp,data->dbox[i].left,data->dbox[i].top);
            Text(rp,lib_dayNumbers[i],i<9 ? 1 : 2);
        }

        i = data->d-1;

        if (data->dayFillPen!=-1)
        {
            SetAPen(rp,MUIPEN(data->dayFillPen));
            RectFill(rp,data->dbox[i].bleft,data->dbox[i].btop,data->dbox[i].bright,data->dbox[i].bbottom);
        }

        SetAPen(rp,MUIPEN(data->dayShinePen));
        Move(rp,data->dbox[i].left,data->dbox[i].top);
        Text(rp,lib_dayNumbers[i],i<9 ? 1 : 2);

        data->od = data->d;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mHandleEvent(struct IClass *cl,Object *obj,struct MUIP_HandleEvent *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    ULONG                active;

    if (!msg->imsg || (msg->imsg->Class!=IDCMP_MOUSEBUTTONS && msg->imsg->Class!=IDCMP_RAWKEY))
        return DoSuperMethodA(cl,obj,(Msg)msg);;

    get(_win(obj),MUIA_Window_ActiveObject,&active);
    if (!active) get(_win(obj),MUIA_Window_DefaultObject,&active);
    active = active==(ULONG)obj;

    if (msg->imsg->Class==IDCMP_MOUSEBUTTONS)
    {
        register UWORD mx, my;

        mx = msg->imsg->MouseX;
        my = msg->imsg->MouseY;

        if (msg->imsg->Qualifier==0xc000 && _mleft(obj)<mx && _mright(obj)>mx && _mtop(obj)<my && _mbottom(obj)>my)
        {
            register int i, days, dc;

            dc = DoubleClick(data->secs,data->micros,msg->imsg->Seconds,msg->imsg->Micros);

            data->secs   = msg->imsg->Seconds;
            data->micros = msg->imsg->Micros;

            days = CALGetMonthDays(data->m,data->y);
            for (i = 0; i<days; i++)
            {
                if (data->dbox[i].cleft<mx && mx<data->dbox[i].cright &&
                    data->dbox[i].ctop<my  && my<data->dbox[i].cbottom)
                {
                    if (i+1==data->d)
                    {
                        if (dc)
                        {
                            set(data->parent,MUIA_Calendar_DoubleClick,i+1);
                            data->secs = data->micros = 0;
                        }
                    }
                    else
                    {
                        set(obj,MUIA_Calendar_Day,i+1);
                        dc = 0;
                    }

                    if (!dc) set(data->parent,MUIA_Calendar_Click,i+1);

                    break;
                }
            }

            if (!active) set(_win(obj),MUIA_Window_DefaultObject,(ULONG)obj);
        }

        return DoSuperMethodA(cl,obj,(Msg)msg);;
    }

    if (msg->imsg->Class==IDCMP_RAWKEY && active)
    {
        register int ddelta = 0, mdelta = 0, ydelta = 0;

        switch (msg->muikey)
        {
            case MUIKEY_LEFT:
                ddelta = -1;
                break;

            case MUIKEY_RIGHT:
                ddelta = 1;
                break;

            case MUIKEY_UP:
                ddelta = -7;
                break;

            case MUIKEY_DOWN:
                ddelta = 7;
                break;

            case MUIKEY_PAGEUP:
                mdelta = -1;
                break;

            case MUIKEY_PAGEDOWN:
                mdelta = 1;
                break;

            case MUIKEY_TOP:
                ydelta = -1;
                break;

            case MUIKEY_BOTTOM:
                ydelta = 1;
                break;

            case MUIKEY_PRESS:
                set(data->parent,MUIA_Calendar_DoubleClick,data->d);
                break;

            case MUIKEY_NONE:
                if (msg->imsg->Class==IDCMP_RAWKEY)
                    if (msg->imsg->Code==NM_WHEEL_UP) ddelta = -1;
                    else if (msg->imsg->Code==NM_WHEEL_DOWN) ddelta = 1;
                break;
        }

        if (ddelta) DoMethod(data->parent,MUIM_Calendar_AddDay,ddelta);
        if (mdelta) DoMethod(data->parent,MUIM_Calendar_AddMonth,mdelta);
        if (ydelta) DoMethod(data->parent,MUIM_Calendar_AddYear,ydelta);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:           return mNew(cl,obj,(APTR)msg);
        case OM_SET:           return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:       return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:     return mCleanup(cl,obj,(APTR)msg);
        case MUIM_Show:        return mShow(cl,obj,(APTR)msg);
        case MUIM_Hide:        return mHide(cl,obj,(APTR)msg);
        case MUIM_AskMinMax:   return mAskMinMax(cl,obj,(APTR)msg);
        case MUIM_Draw:        return mDraw(cl,obj,(APTR)msg);
        case MUIM_HandleEvent: return mHandleEvent(cl,obj,(APTR)msg);

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initDaysClass(void)
{
    if (lib_daysClass = MUI_CreateCustomClass(NULL,MUIC_Area,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (lib_flags & BASEFLG_MUI20)
            lib_daysClass->mcc_Class->cl_ID = "Calendar_Days";

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/
