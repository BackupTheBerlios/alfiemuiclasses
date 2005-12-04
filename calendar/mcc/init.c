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

void
freeMUI(void)
{
    if (GfxBase)
    {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase = NULL;
    }

    if (DiskfontBase)
    {
        CloseLibrary(DiskfontBase);
        DiskfontBase = NULL;
    }

    if (MUIMasterBase)
    {
        if (lib_daysClass)
        {
            MUI_DeleteCustomClass(lib_daysClass);
            lib_daysClass = NULL;
        }

        if (lib_monthesClass)
        {
            MUI_DeleteCustomClass(lib_monthesClass);
            lib_monthesClass = NULL;
        }

        if (lib_yearsClass)
        {
            MUI_DeleteCustomClass(lib_yearsClass);
            lib_yearsClass = NULL;
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

void
freeBase(void)
{
    freeMUI();

    if (LocaleBase)
    {
        if (lib_locale)
        {
            CloseLocale(lib_locale);
            lib_locale = NULL;
        }

        CloseLibrary((struct Library *)LocaleBase);
        LocaleBase = NULL;
    }

    if (UtilityBase)
    {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
    }

    if (DOSBase)
    {
        CloseLibrary((struct Library *)DOSBase);
        DOSBase = NULL;
    }

    if (lib_pool)
    {
        DeletePool(lib_pool);
        lib_pool = NULL;
    }

    lib_flags &= ~(BASEFLG_Init|BASEFLG_MUI20|BASEFLG_MUI4);
}

/***********************************************************************/

ULONG
initBase(void)
{
    if ((DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",39)) &&
        (UtilityBase = OpenLibrary("utility.library",39)) &&
        (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library",39)) &&
        (lib_pool = CreatePool(MEMF_ANY|MEMF_CLEAR,2048,1024)) &&
        (lib_locale = OpenLocale(NULL)) &&
        (lib_abmonthesArray = makeMonthesArray(TRUE)) &&
        (lib_monthesArray = makeMonthesArray(FALSE)) &&
        (lib_daysArray = makeDaysArray(TRUE)))
    {
        readHolidays();

        lib_flags |= BASEFLG_Init;

        return TRUE;
    }

    freeBase();

    return FALSE;
}

/***********************************************************************/

ULONG
initMUI(void)
{
    if ((MUIMasterBase = OpenLibrary("muimaster.library",19)) &&
        (GfxBase       = (struct GfxBase *)OpenLibrary("graphics.library",40)) &&
        (DiskfontBase  = OpenLibrary("diskfont.library",37)) &&
        initDaysClass() &&
        initMonthesClass() &&
        initYearsClass() &&
        initMCC())
    {
        if (MUIMasterBase->lib_Version>=20)
        {
            lib_flags |= BASEFLG_MUI20;

            if (MUIMasterBase->lib_Version>20 || MUIMasterBase->lib_Revision>=5341)
                lib_flags |= BASEFLG_MUI4;
        }

        IntuitionBase = (struct IntuitionBase *)lib_class->mcc_IntuitionBase;

        lib_flags |= BASEFLG_MUI;

        return TRUE;
    }

    freeMUI();

    return FALSE;
}

/***********************************************************************/

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
        case 0:
            ObtainSemaphore(&lib_sem);
            if (!(lib_flags & BASEFLG_MUI)) initMUI();
            ReleaseSemaphore(&lib_sem);
            return (ULONG)lib_class;

        default:
            return 0;
    }
}

/****************************************************************************/
