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

#ifdef __MORPHOS__

void
sprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}

#else

ULONG STDARGS
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}

UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void STDARGS
sprintf(STRPTR to,STRPTR fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,to);
}

#endif

/***********************************************************************/

LONG
xget(Object *obj,ULONG attribute)
{
    LONG x;

    return get(obj,attribute,&x) ? x : 0;
}

/***********************************************************************/

Object *
oibutton(STRPTR spec)
{
    return TextObject,
        MUIA_CycleChain,    TRUE,
        MUIA_Font,          MUIV_Font_Button,
        MUIA_InputMode,     MUIV_InputMode_RelVerify,
        ButtonFrame,
        MUIA_Background,    MUII_ButtonBack,
        MUIA_Text_Contents, spec,
        MUIA_Text_PreParse, MUIX_C,
        MUIA_Text_SetMax,   TRUE,
    End;
}

/***********************************************************************/

struct TextFont *
openFont(STRPTR name)
{
    REGARRAY UBYTE  buf[256];
    register STRPTR t, s;
    struct TextAttr ta;
    long            ys;

    strcpy(buf,name);

    if (t = strchr(buf,'/'))
    {
        *t++ = 0;
        if (!stcd_l(t,&ys) || ys<=0) ys = 8;
    }
    else ys = 8;

    for (s = NULL, t = buf; *t; t++) if (*t=='.') s = t;
    if (!s || stricmp(++s,"font")) strcat(buf,".font");

    ta.ta_Name  = buf;
    ta.ta_YSize = ys;
    ta.ta_Style = 0;
    ta.ta_Flags = 0;

    return OpenDiskFont(&ta);
}

/***********************************************************************/

struct strLen
{
    STRPTR name;
    int    len;
};

STRPTR *
makeMonthesArray(ULONG ab)
{
    REGARRAY struct strLen monthes[12];
    register STRPTR        *array;
    register ULONG         base;
    register int           i, size;

    size = sizeof(STRPTR)*13;
    base = ab ? ABMON_1 : MON_1;

    for (i = 0; i<12; i++, base++)
    {
        monthes[i].name = GetLocaleStr(lib_locale,base);
        monthes[i].len = strlen(monthes[i].name)+1;
        size += monthes[i].len;
    }

    if (array = allocVecPooled(lib_pool,size))
    {
        STRPTR ptr;

        ptr = (STRPTR)array+sizeof(STRPTR)*13;

        for (i = 0; i<12; i++)
        {
            strcpy(array[i] = ptr,monthes[i].name);
            ptr += monthes[i].len;
        }

        array[i] = NULL;
    }

    return array;
}

/***********************************************************************/

STRPTR *
makeDaysArray(ULONG ab)
{
    REGARRAY struct strLen days[12];
    register STRPTR        *array;
    register ULONG         base;
    register int           i, size;

    size = sizeof(STRPTR)*13;

    base = ab ? ABDAY_1 : DAY_1;

    for (i = 0; i<7; i++, base++)
    {
        days[i].name = GetLocaleStr(lib_locale,base);
        days[i].len = strlen(days[i].name)+1;
        size += days[i].len;
    }

    if (array = allocVecPooled(lib_pool,size))
    {
        STRPTR ptr;

        ptr = (STRPTR)array+sizeof(STRPTR)*13;

        for (i = 0; i<7; i++)
        {
            strcpy(array[i] = ptr,days[i].name);
            ptr += days[i].len;
        }

        array[i] = NULL;

    }

    return array;
}

/***********************************************************************/

struct item
{
    ULONG *d;
    ULONG *m;
    ULONG a;
};

void
readHolidays(void)
{
    register BPTR lock;

    if (lock = Open("ENV:CaledarHolidays",MODE_OLDFILE))
    {
        if (lib_holidays = allocVecPooled(lib_pool,4096))
        {
            REGARRAY UBYTE buf[256];

            while (FGets(lock,buf,sizeof(buf)-1))
            {
                struct RDArgs  ra;
                struct item    item = {0};
                REGARRAY UBYTE wbuf[256];

                if (*buf=='#' || *buf=='\n') continue;

                ra.RDA_Source.CS_Buffer  = buf;
                ra.RDA_Source.CS_Length  = strlen(buf);
                ra.RDA_Source.CS_CurChr  = 0;
                ra.RDA_DAList            = NULL;
                ra.RDA_Buffer            = wbuf;
                ra.RDA_BufSiz            = sizeof(wbuf);
                ra.RDA_Flags             = RDAF_NOALLOC|RDAF_NOPROMPT;

                if (ReadArgs("DAY/A/N,MONTH/A/N,A=ANNIVERSARY/S",(LONG *)&item,&ra))
                {
                    ULONG y = 2004;

                    CALValidateDate(item.d,item.m,&y);
                    lib_holidays[((*item.m-1)<<8)|(*item.d-1)] = item.a ? -1 : 1;
                }
            }
        }

        Close(lock);
    }
}

/***********************************************************************/

APTR
allocVecPooled(APTR pool,ULONG size)
{
    register ULONG *mem;

    if (mem = AllocPooled(pool,size = size+sizeof(ULONG)))
        *mem++ = size;

    return mem;
}

/****************************************************************************/

void
freeVecPooled(APTR pool,APTR mem)
{
    FreePooled(pool,(LONG *)mem-1,*((LONG *)mem-1));
}

/****************************************************************************/
