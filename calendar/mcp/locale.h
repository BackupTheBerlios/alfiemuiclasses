#ifndef LOCALE_H
#define LOCALE_H


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

#define Msg_Title_Colours 0
#define Msg_Out 1
#define Msg_Out_Help 2
#define Msg_Out_Title 3
#define Msg_Over 4
#define Msg_Over_Help 5
#define Msg_Over_Title 6
#define Msg_Visited 7
#define Msg_Visited_Help 8
#define Msg_Visited_Title 9
#define Msg_Title_Settings 10
#define Msg_Underline 11
#define Msg_Underline_Help 12
#define Msg_Fallback 13
#define Msg_Fallback_Help 14
#define Msg_DoVisited 15
#define Msg_DoVisited_Help 16
#define Msg_Title_Examples 17
#define Msg_Info_Copyright 18
#define Msg_Font 19
#define Msg_Font_Help 20
#define Msg_Item_Send 40
#define Msg_Item_Copy 41
#define Msg_Item_OpenURLPrefs 42
#define Msg_Info_Translator 43
#define Msg_Info_Rest 44
#define Msg_Item_Defaults 45

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define Msg_Title_Colours_STR "Colours"
#define Msg_Out_STR "_Out"
#define Msg_Out_Help_STR "Pen when the mouse\nis not over the link."
#define Msg_Out_Title_STR "'Out' pen"
#define Msg_Over_STR "O_ver"
#define Msg_Over_Help_STR "Pen when the mouse\nis over the link."
#define Msg_Over_Title_STR "'Over' pen"
#define Msg_Visited_STR "Visi_ted"
#define Msg_Visited_Help_STR "Pen for a visited link."
#define Msg_Visited_Title_STR "'Visited' pen"
#define Msg_Title_Settings_STR "Settings"
#define Msg_Underline_STR "Underl_ine"
#define Msg_Underline_Help_STR "If selected the\nlink is underlined."
#define Msg_Fallback_STR "_Fallback"
#define Msg_Fallback_Help_STR "Active even if OpenURL\nis not installed."
#define Msg_DoVisited_STR "Us_e Visited Pen"
#define Msg_DoVisited_Help_STR "Use 'Visited' pen\nfor a visited link."
#define Msg_Title_Examples_STR "Examples"
#define Msg_Info_Copyright_STR "Copyright 2000-2002 Alfonso Ranieri <alforan@tin.it>"
#define Msg_Font_STR "Fo_nt"
#define Msg_Font_Help_STR "The font to use."
#define Msg_Item_Send_STR "Reach url..."
#define Msg_Item_Copy_STR "Copy to clipbord"
#define Msg_Item_OpenURLPrefs_STR "OpenURL prefs..."
#define Msg_Info_Translator_STR ""
#define Msg_Info_Rest_STR "Write to <alforan@tin.it> for\nprogrammer info and use terms"
#define Msg_Item_Defaults_STR "Reset to defaults"

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
    {Msg_Title_Colours,(STRPTR)Msg_Title_Colours_STR},
    {Msg_Out,(STRPTR)Msg_Out_STR},
    {Msg_Out_Help,(STRPTR)Msg_Out_Help_STR},
    {Msg_Out_Title,(STRPTR)Msg_Out_Title_STR},
    {Msg_Over,(STRPTR)Msg_Over_STR},
    {Msg_Over_Help,(STRPTR)Msg_Over_Help_STR},
    {Msg_Over_Title,(STRPTR)Msg_Over_Title_STR},
    {Msg_Visited,(STRPTR)Msg_Visited_STR},
    {Msg_Visited_Help,(STRPTR)Msg_Visited_Help_STR},
    {Msg_Visited_Title,(STRPTR)Msg_Visited_Title_STR},
    {Msg_Title_Settings,(STRPTR)Msg_Title_Settings_STR},
    {Msg_Underline,(STRPTR)Msg_Underline_STR},
    {Msg_Underline_Help,(STRPTR)Msg_Underline_Help_STR},
    {Msg_Fallback,(STRPTR)Msg_Fallback_STR},
    {Msg_Fallback_Help,(STRPTR)Msg_Fallback_Help_STR},
    {Msg_DoVisited,(STRPTR)Msg_DoVisited_STR},
    {Msg_DoVisited_Help,(STRPTR)Msg_DoVisited_Help_STR},
    {Msg_Title_Examples,(STRPTR)Msg_Title_Examples_STR},
    {Msg_Info_Copyright,(STRPTR)Msg_Info_Copyright_STR},
    {Msg_Font,(STRPTR)Msg_Font_STR},
    {Msg_Font_Help,(STRPTR)Msg_Font_Help_STR},
    {Msg_Item_Send,(STRPTR)Msg_Item_Send_STR},
    {Msg_Item_Copy,(STRPTR)Msg_Item_Copy_STR},
    {Msg_Item_OpenURLPrefs,(STRPTR)Msg_Item_OpenURLPrefs_STR},
    {Msg_Info_Translator,(STRPTR)Msg_Info_Translator_STR},
    {Msg_Info_Rest,(STRPTR)Msg_Info_Rest_STR},
    {Msg_Item_Defaults,(STRPTR)Msg_Item_Defaults_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x08"
    Msg_Title_Colours_STR "\x00"
    "\x00\x00\x00\x01\x00\x06"
    Msg_Out_STR "\x00\x00"
    "\x00\x00\x00\x02\x00\x2A"
    Msg_Out_Help_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x0A"
    Msg_Out_Title_STR "\x00"
    "\x00\x00\x00\x04\x00\x06"
    Msg_Over_STR "\x00"
    "\x00\x00\x00\x05\x00\x26"
    Msg_Over_Help_STR "\x00\x00"
    "\x00\x00\x00\x06\x00\x0C"
    Msg_Over_Title_STR "\x00\x00"
    "\x00\x00\x00\x07\x00\x0A"
    Msg_Visited_STR "\x00\x00"
    "\x00\x00\x00\x08\x00\x18"
    Msg_Visited_Help_STR "\x00"
    "\x00\x00\x00\x09\x00\x0E"
    Msg_Visited_Title_STR "\x00"
    "\x00\x00\x00\x0A\x00\x0A"
    Msg_Title_Settings_STR "\x00\x00"
    "\x00\x00\x00\x0B\x00\x0C"
    Msg_Underline_STR "\x00\x00"
    "\x00\x00\x00\x0C\x00\x24"
    Msg_Underline_Help_STR "\x00"
    "\x00\x00\x00\x0D\x00\x0A"
    Msg_Fallback_STR "\x00"
    "\x00\x00\x00\x0E\x00\x2A"
    Msg_Fallback_Help_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x12"
    Msg_DoVisited_STR "\x00\x00"
    "\x00\x00\x00\x10\x00\x26"
    Msg_DoVisited_Help_STR "\x00"
    "\x00\x00\x00\x11\x00\x0A"
    Msg_Title_Examples_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\x36"
    Msg_Info_Copyright_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x06"
    Msg_Font_STR "\x00"
    "\x00\x00\x00\x14\x00\x12"
    Msg_Font_Help_STR "\x00\x00"
    "\x00\x00\x00\x28\x00\x0E"
    Msg_Item_Send_STR "\x00\x00"
    "\x00\x00\x00\x29\x00\x12"
    Msg_Item_Copy_STR "\x00\x00"
    "\x00\x00\x00\x2A\x00\x12"
    Msg_Item_OpenURLPrefs_STR "\x00\x00"
    "\x00\x00\x00\x2B\x00\x02"
    Msg_Info_Translator_STR "\x00\x00"
    "\x00\x00\x00\x2C\x00\x3C"
    Msg_Info_Rest_STR "\x00"
    "\x00\x00\x00\x2D\x00\x12"
    Msg_Item_Defaults_STR "\x00"
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


#endif /* LOCALE_H */
