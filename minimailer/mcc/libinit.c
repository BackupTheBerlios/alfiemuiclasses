
#include "class.h"
#include "MiniMailer.mcc_rev.h"

/****************************************************************************/

UBYTE                   lib_name[] = PRG;
UBYTE           	    lib_ver[] = VSTRING;
ULONG                   lib_version = VERSION;
ULONG                   lib_revision = REVISION;

struct ExecBase         *SysBase = NULL;
struct DosLibrary       *DOSBase = NULL;
struct Library          *UtilityBase = NULL;
struct IntuitionBase    *IntuitionBase = NULL;
struct LocaleBase       *LocaleBase = NULL;
struct Library          *IFFParseBase;
struct Library          *MUIMasterBase = NULL;

struct Library          *lib_base = NULL;
ULONG                   lib_segList = NULL;
struct SignalSemaphore  lib_sem = {0};
struct SignalSemaphore  lib_poolSem = {0};
struct Catalog          *lib_cat = NULL;
ULONG                   lib_flags = 0;
APTR		            lib_pool = NULL;

struct MUI_CustomClass  *lib_attach = NULL;
struct MUI_CustomClass  *lib_class = NULL;

/****************************************************************************/

#ifdef __MORPHOS__
#include <exec/resident.h>

static struct Library *initLib ( struct Library *base , BPTR segList , struct ExecBase *sys );
static struct Library *openLib ( void );
static ULONG expungeLib ( void );
static ULONG closeLib ( void );
static ULONG nil (void );

static const APTR funcTable[] =
{
    (APTR)FUNCARRAY_32BIT_NATIVE,
    (APTR)openLib,
    (APTR)closeLib,
    (APTR)expungeLib,
    (APTR)nil,
    (APTR)query,
    (APTR)-1
};

static const ULONG initTable[] =
{
    sizeof(struct Library),
    (ULONG)funcTable,
    NULL,
    (ULONG)initLib
};

const struct Resident romTag =
{
    RTC_MATCHWORD,
    (struct Resident *)&romTag,
    (struct Resident *)&romTag+1,
    RTF_AUTOINIT|RTF_PPC|RTF_EXTENDED,
    VERSION,
    NT_LIBRARY,
    0,
    (UBYTE *)lib_name,
    (UBYTE *)lib_ver,
    (APTR)initTable,
    REVISION,
    NULL
};

const ULONG __abox__ = 1;
#endif

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *initLib(struct Library *base,BPTR segList,struct ExecBase *sys)
#else
struct Library *SAVEDS ASM initLib(REG(a0,ULONG segList),REG(a6,APTR sys),REG(d0, struct Library *base))
#endif
{
#define SysBase sys
    InitSemaphore(&lib_sem);
    InitSemaphore(&lib_poolSem);
#undef SysBase

    SysBase     = sys;
    lib_segList = segList;

    return lib_base = base;
}

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *openLib(void)
#else
struct Library * SAVEDS ASM openLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register struct Library *res;

    ObtainSemaphore(&lib_sem);

    base->lib_OpenCnt++;
    base->lib_Flags &= ~LIBF_DELEXP;

    if (!(lib_flags & BASEFLG_Init) && !initBase())
    {
        base->lib_OpenCnt--;
        res = NULL;
    }
    else res = base;

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG expungeLib(void)
#else
ULONG SAVEDS ASM expungeLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res;

    ObtainSemaphore(&lib_sem);

    if (!base->lib_OpenCnt)
    {
        Remove((struct Node *)base);
        FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

        res = lib_segList;
    }
    else
    {
        base->lib_Flags |= LIBF_DELEXP;
        res = NULL;
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG closeLib(void)
#else
ULONG SAVEDS ASM closeLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res = NULL;

    ObtainSemaphore(&lib_sem);

    if (!--base->lib_OpenCnt)
    {
        freeBase();

        if (base->lib_Flags & LIBF_DELEXP)
	    {
            Remove((struct Node *)base);
    	    FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

            res = lib_segList;
    	}
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG nil(void)
{
    return 0;
}
#endif

/****************************************************************************/


