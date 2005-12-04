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
#define USE_CAL_BODY
#define USE_CAL_COLORS
#include "Cal_iff.h"

/****************************************************************************/

#ifdef __MORPHOS__
ULONG query(void)
{
    LONG which = (LONG)REG_D0;
#else
ULONG SAVEDS ASM query(REG(d0,LONG which))
{
#endif
    switch (which)
    {
        case 1:
            return (ULONG)lib_class;

        case 2:
            return (ULONG)BodychunkObject,
                MUIA_FixWidth,              CAL_WIDTH,
                MUIA_FixHeight,             CAL_HEIGHT,
                MUIA_Bitmap_Width,          CAL_WIDTH,
                MUIA_Bitmap_Height,         CAL_HEIGHT,
                MUIA_Bodychunk_Depth,       CAL_DEPTH,
                MUIA_Bodychunk_Body,        Cal_body,
                MUIA_Bodychunk_Compression, CAL_COMPRESSION,
                MUIA_Bodychunk_Masking,     CAL_MASKING,
                MUIA_Bitmap_SourceColors,   Cal_colors,
                MUIA_Bitmap_Transparent,    0,
            End;

        default:
            return 0;
    }
}

/****************************************************************************/

void
freeBase(void)
{
    if (LocaleBase)
    {
        if (lib_cat)
        {
            CloseCatalog(lib_cat);
            lib_cat = NULL;
        }

        if (lib_locale)
        {
            CloseLocale(lib_locale);
            lib_locale = NULL;
        }

        CloseLibrary((struct Library *)LocaleBase);
        LocaleBase = NULL;
    }

    if (MUIMasterBase)
    {
        if (lib_wsd)
        {
            MUI_DeleteCustomClass(lib_wsd);
            lib_wsd = NULL;
        }

        if (lib_class)
        {
            MUI_DeleteCustomClass(lib_class);
            lib_class = NULL;
        }

        CloseLibrary(MUIMasterBase);
        MUIMasterBase = NULL;
    }

    lib_flags &= ~(BASEFLG_Init|BASEFLG_MUI20|BASEFLG_MUI4);
}

/***********************************************************************/

ULONG
initBase(void)
{
    if ((MUIMasterBase = OpenLibrary("muimaster.library",0)) &&
        (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library",39)) &&
        (lib_locale = OpenLocale(NULL)) &&
        initWSD() &&
        initMCP())
    {
        if (MUIMasterBase->lib_Version>=20)
        {
            lib_flags |= BASEFLG_MUI20;

            if (MUIMasterBase->lib_Version>20 || MUIMasterBase->lib_Revision>=5341)
                lib_flags |= BASEFLG_MUI4;
        }

        DOSBase       = (struct DosLibrary *)lib_class->mcc_DOSBase;
        UtilityBase   = lib_class->mcc_UtilityBase;
        IntuitionBase = (struct IntuitionBase *)lib_class->mcc_IntuitionBase;

        initStrings();

        lib_flags |= BASEFLG_Init;

        return TRUE;
    }

    freeBase();

    return FALSE;
}

/***********************************************************************/
