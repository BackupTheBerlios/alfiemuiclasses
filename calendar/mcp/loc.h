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

#ifndef LOC_H
#define LOC_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define Msg_DaysNames_Title 0
#define Msg_Days_Title 1
#define Msg_DaysNamesTextPen 2
#define Msg_DaysNamesTextPen_Title 3
#define Msg_DaysNamesTextPen_Help 4
#define Msg_DaysNamesBackground 5
#define Msg_DaysNamesBackground_Title 6
#define Msg_DaysNamesBackground_Help 7
#define Msg_DaysNamesDetailPen 8
#define Msg_DaysNamesDetailPen_Title 9
#define Msg_DaysNamesDetailPen_Help 10
#define Msg_DaysBackground 11
#define Msg_DaysBackground_Title 12
#define Msg_DaysBackground_Help 13
#define Msg_DaysTextPen 14
#define Msg_DaysTextPen_Title 15
#define Msg_DaysTextPen_Help 16
#define Msg_DaysShinePen 17
#define Msg_DaysShinePen_Title 18
#define Msg_DaysShinePen_Help 19
#define Msg_DaysFillPen 20
#define Msg_DaysFillPen_Title 21
#define Msg_DaysFillPen_Help 22
#define Msg_SundaysPen 23
#define Msg_SundaysPen_Title 24
#define Msg_SundaysPen_Help 25
#define Msg_AnniversaryPen 26
#define Msg_AnniversaryPen_Title 27
#define Msg_AnniversaryPen_Help 28
#define Msg_UseDaysNamesBackground_Help 29
#define Msg_UseDaysBackground_Help 30
#define Msg_UseDaysFillPen_Help 31
#define Msg_UseDaysNamesBackground 32
#define Msg_UseDaysBackground 33
#define Msg_UseDaysFillPen 34
#define Msg_Font 50
#define Msg_Font_Help 51
#define Msg_WeekStartDay 100
#define Msg_WeekStartDay_Help 101
#define Msg_Info_First 600
#define Msg_Info_Reserved 601
#define Msg_Info_Rest 602
#define Msg_Info_Translator 603
#define Msg_Sample_Sample 700
#define Msg_Sample_WinTitle 701

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define Msg_DaysNames_Title_STR "Days names"
#define Msg_Days_Title_STR "Days"
#define Msg_DaysNamesTextPen_STR "Text"
#define Msg_DaysNamesTextPen_Title_STR "Days names pen"
#define Msg_DaysNamesTextPen_Help_STR "The color of the pen used\nto render the days names."
#define Msg_DaysNamesBackground_STR "Background"
#define Msg_DaysNamesBackground_Title_STR "Days names background"
#define Msg_DaysNamesBackground_Help_STR "The background for the days names."
#define Msg_DaysNamesDetailPen_STR "Detail"
#define Msg_DaysNamesDetailPen_Title_STR "Days names fill pen"
#define Msg_DaysNamesDetailPen_Help_STR "The color of the pen used to\nrender the days names details."
#define Msg_DaysBackground_STR "Background"
#define Msg_DaysBackground_Title_STR "Days background"
#define Msg_DaysBackground_Help_STR "The background for the days."
#define Msg_DaysTextPen_STR "Text"
#define Msg_DaysTextPen_Title_STR "Days detail pen"
#define Msg_DaysTextPen_Help_STR "The color of the pen used\nto render the days."
#define Msg_DaysShinePen_STR "Shine"
#define Msg_DaysShinePen_Title_STR "Days shine pen"
#define Msg_DaysShinePen_Help_STR "The color of the pen used\nto render the selected day."
#define Msg_DaysFillPen_STR "Fill"
#define Msg_DaysFillPen_Title_STR "Days fill pen"
#define Msg_DaysFillPen_Help_STR "The color of the pen used to render\nthe background of the selected day."
#define Msg_SundaysPen_STR "Sundays"
#define Msg_SundaysPen_Title_STR "Sundays pen"
#define Msg_SundaysPen_Help_STR "The color of the pen used\nto render sundays."
#define Msg_AnniversaryPen_STR "Anniversaries"
#define Msg_AnniversaryPen_Title_STR "Anniversaries pen"
#define Msg_AnniversaryPen_Help_STR "The color of the pen used\nto render anniversaries."
#define Msg_UseDaysNamesBackground_Help_STR "If selected the days names background\nis used."
#define Msg_UseDaysBackground_Help_STR "If selected the days background\nis used."
#define Msg_UseDaysFillPen_Help_STR "If selected the days\nfill pen is used."
#define Msg_UseDaysNamesBackground_STR "Use"
#define Msg_UseDaysBackground_STR "Use"
#define Msg_UseDaysFillPen_STR "Use"
#define Msg_Font_STR "Font"
#define Msg_Font_Help_STR "The font used to render days."
#define Msg_WeekStartDay_STR "Your week starts with:"
#define Msg_WeekStartDay_Help_STR "The day your week starts with.\nIt is taken from the system by default,\nbut it may sometimes be wrong."
#define Msg_Info_First_STR "%s\nCopyright 2004-2005 by Alfonso Ranieri."
#define Msg_Info_Reserved_STR "All Rights are Reserved."
#define Msg_Info_Rest_STR "Please write to <alforan@tin.it> for\ndevelopers docs and licensing terms."
#define Msg_Info_Translator_STR ""
#define Msg_Sample_Sample_STR "Sample"
#define Msg_Sample_WinTitle_STR "Calendar example"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {Msg_DaysNames_Title,(STRPTR)Msg_DaysNames_Title_STR},
    {Msg_Days_Title,(STRPTR)Msg_Days_Title_STR},
    {Msg_DaysNamesTextPen,(STRPTR)Msg_DaysNamesTextPen_STR},
    {Msg_DaysNamesTextPen_Title,(STRPTR)Msg_DaysNamesTextPen_Title_STR},
    {Msg_DaysNamesTextPen_Help,(STRPTR)Msg_DaysNamesTextPen_Help_STR},
    {Msg_DaysNamesBackground,(STRPTR)Msg_DaysNamesBackground_STR},
    {Msg_DaysNamesBackground_Title,(STRPTR)Msg_DaysNamesBackground_Title_STR},
    {Msg_DaysNamesBackground_Help,(STRPTR)Msg_DaysNamesBackground_Help_STR},
    {Msg_DaysNamesDetailPen,(STRPTR)Msg_DaysNamesDetailPen_STR},
    {Msg_DaysNamesDetailPen_Title,(STRPTR)Msg_DaysNamesDetailPen_Title_STR},
    {Msg_DaysNamesDetailPen_Help,(STRPTR)Msg_DaysNamesDetailPen_Help_STR},
    {Msg_DaysBackground,(STRPTR)Msg_DaysBackground_STR},
    {Msg_DaysBackground_Title,(STRPTR)Msg_DaysBackground_Title_STR},
    {Msg_DaysBackground_Help,(STRPTR)Msg_DaysBackground_Help_STR},
    {Msg_DaysTextPen,(STRPTR)Msg_DaysTextPen_STR},
    {Msg_DaysTextPen_Title,(STRPTR)Msg_DaysTextPen_Title_STR},
    {Msg_DaysTextPen_Help,(STRPTR)Msg_DaysTextPen_Help_STR},
    {Msg_DaysShinePen,(STRPTR)Msg_DaysShinePen_STR},
    {Msg_DaysShinePen_Title,(STRPTR)Msg_DaysShinePen_Title_STR},
    {Msg_DaysShinePen_Help,(STRPTR)Msg_DaysShinePen_Help_STR},
    {Msg_DaysFillPen,(STRPTR)Msg_DaysFillPen_STR},
    {Msg_DaysFillPen_Title,(STRPTR)Msg_DaysFillPen_Title_STR},
    {Msg_DaysFillPen_Help,(STRPTR)Msg_DaysFillPen_Help_STR},
    {Msg_SundaysPen,(STRPTR)Msg_SundaysPen_STR},
    {Msg_SundaysPen_Title,(STRPTR)Msg_SundaysPen_Title_STR},
    {Msg_SundaysPen_Help,(STRPTR)Msg_SundaysPen_Help_STR},
    {Msg_AnniversaryPen,(STRPTR)Msg_AnniversaryPen_STR},
    {Msg_AnniversaryPen_Title,(STRPTR)Msg_AnniversaryPen_Title_STR},
    {Msg_AnniversaryPen_Help,(STRPTR)Msg_AnniversaryPen_Help_STR},
    {Msg_UseDaysNamesBackground_Help,(STRPTR)Msg_UseDaysNamesBackground_Help_STR},
    {Msg_UseDaysBackground_Help,(STRPTR)Msg_UseDaysBackground_Help_STR},
    {Msg_UseDaysFillPen_Help,(STRPTR)Msg_UseDaysFillPen_Help_STR},
    {Msg_UseDaysNamesBackground,(STRPTR)Msg_UseDaysNamesBackground_STR},
    {Msg_UseDaysBackground,(STRPTR)Msg_UseDaysBackground_STR},
    {Msg_UseDaysFillPen,(STRPTR)Msg_UseDaysFillPen_STR},
    {Msg_Font,(STRPTR)Msg_Font_STR},
    {Msg_Font_Help,(STRPTR)Msg_Font_Help_STR},
    {Msg_WeekStartDay,(STRPTR)Msg_WeekStartDay_STR},
    {Msg_WeekStartDay_Help,(STRPTR)Msg_WeekStartDay_Help_STR},
    {Msg_Info_First,(STRPTR)Msg_Info_First_STR},
    {Msg_Info_Reserved,(STRPTR)Msg_Info_Reserved_STR},
    {Msg_Info_Rest,(STRPTR)Msg_Info_Rest_STR},
    {Msg_Info_Translator,(STRPTR)Msg_Info_Translator_STR},
    {Msg_Sample_Sample,(STRPTR)Msg_Sample_Sample_STR},
    {Msg_Sample_WinTitle,(STRPTR)Msg_Sample_WinTitle_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x0C"
    Msg_DaysNames_Title_STR "\x00\x00"
    "\x00\x00\x00\x01\x00\x06"
    Msg_Days_Title_STR "\x00\x00"
    "\x00\x00\x00\x02\x00\x06"
    Msg_DaysNamesTextPen_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x10"
    Msg_DaysNamesTextPen_Title_STR "\x00\x00"
    "\x00\x00\x00\x04\x00\x34"
    Msg_DaysNamesTextPen_Help_STR "\x00"
    "\x00\x00\x00\x05\x00\x0C"
    Msg_DaysNamesBackground_STR "\x00\x00"
    "\x00\x00\x00\x06\x00\x16"
    Msg_DaysNamesBackground_Title_STR "\x00"
    "\x00\x00\x00\x07\x00\x24"
    Msg_DaysNamesBackground_Help_STR "\x00\x00"
    "\x00\x00\x00\x08\x00\x08"
    Msg_DaysNamesDetailPen_STR "\x00\x00"
    "\x00\x00\x00\x09\x00\x14"
    Msg_DaysNamesDetailPen_Title_STR "\x00"
    "\x00\x00\x00\x0A\x00\x3C"
    Msg_DaysNamesDetailPen_Help_STR "\x00"
    "\x00\x00\x00\x0B\x00\x0C"
    Msg_DaysBackground_STR "\x00\x00"
    "\x00\x00\x00\x0C\x00\x10"
    Msg_DaysBackground_Title_STR "\x00"
    "\x00\x00\x00\x0D\x00\x1E"
    Msg_DaysBackground_Help_STR "\x00\x00"
    "\x00\x00\x00\x0E\x00\x06"
    Msg_DaysTextPen_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x10"
    Msg_DaysTextPen_Title_STR "\x00"
    "\x00\x00\x00\x10\x00\x2E"
    Msg_DaysTextPen_Help_STR "\x00"
    "\x00\x00\x00\x11\x00\x06"
    Msg_DaysShinePen_STR "\x00"
    "\x00\x00\x00\x12\x00\x10"
    Msg_DaysShinePen_Title_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x36"
    Msg_DaysShinePen_Help_STR "\x00"
    "\x00\x00\x00\x14\x00\x06"
    Msg_DaysFillPen_STR "\x00\x00"
    "\x00\x00\x00\x15\x00\x0E"
    Msg_DaysFillPen_Title_STR "\x00"
    "\x00\x00\x00\x16\x00\x48"
    Msg_DaysFillPen_Help_STR "\x00"
    "\x00\x00\x00\x17\x00\x08"
    Msg_SundaysPen_STR "\x00"
    "\x00\x00\x00\x18\x00\x0C"
    Msg_SundaysPen_Title_STR "\x00"
    "\x00\x00\x00\x19\x00\x2E"
    Msg_SundaysPen_Help_STR "\x00\x00"
    "\x00\x00\x00\x1A\x00\x0E"
    Msg_AnniversaryPen_STR "\x00"
    "\x00\x00\x00\x1B\x00\x12"
    Msg_AnniversaryPen_Title_STR "\x00"
    "\x00\x00\x00\x1C\x00\x34"
    Msg_AnniversaryPen_Help_STR "\x00\x00"
    "\x00\x00\x00\x1D\x00\x30"
    Msg_UseDaysNamesBackground_Help_STR "\x00\x00"
    "\x00\x00\x00\x1E\x00\x2A"
    Msg_UseDaysBackground_Help_STR "\x00\x00"
    "\x00\x00\x00\x1F\x00\x28"
    Msg_UseDaysFillPen_Help_STR "\x00\x00"
    "\x00\x00\x00\x20\x00\x04"
    Msg_UseDaysNamesBackground_STR "\x00"
    "\x00\x00\x00\x21\x00\x04"
    Msg_UseDaysBackground_STR "\x00"
    "\x00\x00\x00\x22\x00\x04"
    Msg_UseDaysFillPen_STR "\x00"
    "\x00\x00\x00\x32\x00\x06"
    Msg_Font_STR "\x00\x00"
    "\x00\x00\x00\x33\x00\x1E"
    Msg_Font_Help_STR "\x00"
    "\x00\x00\x00\x64\x00\x18"
    Msg_WeekStartDay_STR "\x00\x00"
    "\x00\x00\x00\x65\x00\x66"
    Msg_WeekStartDay_Help_STR "\x00"
    "\x00\x00\x02\x58\x00\x2C"
    Msg_Info_First_STR "\x00\x00"
    "\x00\x00\x02\x59\x00\x1A"
    Msg_Info_Reserved_STR "\x00\x00"
    "\x00\x00\x02\x5A\x00\x4A"
    Msg_Info_Rest_STR "\x00"
    "\x00\x00\x02\x5B\x00\x02"
    Msg_Info_Translator_STR "\x00\x00"
    "\x00\x00\x02\xBC\x00\x08"
    Msg_Sample_Sample_STR "\x00\x00"
    "\x00\x00\x02\xBD\x00\x12"
    Msg_Sample_WinTitle_STR "\x00\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};


#ifdef CATCOMP_CODE

STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
LONG   *l;
UWORD  *w;
STRPTR  builtIn;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum)
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtIn = (STRPTR)((ULONG)l + 6);

#undef LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#undef LocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* LOC_H */
