
#include "class.h"

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
        case 0:  return (ULONG)lib_class;
        default: return 0;
    }
}

/****************************************************************************/

void
freeBase(void)
{
    if (MUIMasterBase)
    {
        freeAttach();
        freeMCC();

        CloseLibrary(MUIMasterBase);
        MUIMasterBase = NULL;
    }

    if (IFFParseBase)
    {
        CloseLibrary((struct Library *)IFFParseBase);
        IFFParseBase = NULL;
    }

    if (LocaleBase)
    {
        if (lib_cat) CloseCatalog(lib_cat);
        CloseLibrary((struct Library *)LocaleBase);
    }

    if (UtilityBase)
    {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
    }

    if (IntuitionBase)
    {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
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

    lib_flags &= ~(BASEFLG_Init|BASEFLG_MUI20);
}

/***********************************************************************/

ULONG
initBase(void)
{
    if ((DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",37)) &&
        (IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library",37)) &&
        (UtilityBase = OpenLibrary("utility.library",37)) &&
        (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library",37)) &&
        (IFFParseBase = OpenLibrary("iffparse.library",37)) &&
        (MUIMasterBase = OpenLibrary("muimaster.library",19)) &&
        (lib_pool = CreatePool(MEMF_ANY|MEMF_CLEAR,512,256)))
    {
        if (MUIMasterBase->lib_Version>=MUIVER20) lib_flags |= BASEFLG_MUI20;
        initStrings();

        if (initAttach() && initMCC())
        {
            lib_flags |= BASEFLG_Init;

            return TRUE;
        }
    }

    freeBase();

    return FALSE;
}

/***********************************************************************/
