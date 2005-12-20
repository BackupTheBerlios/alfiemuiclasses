#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

/***************************************************************************/

extern UBYTE                   lib_name[];
extern UBYTE           	       lib_ver[];
extern ULONG                   lib_version;
extern ULONG                   lib_revision;

extern struct ExecBase         *SysBase;
extern struct DosLibrary       *DOSBase;
extern struct Library          *UtilityBase;
extern struct IntuitionBase    *IntuitionBase;
extern struct LocaleBase       *LocaleBase;
extern struct Library          *IFFParseBase;
extern struct Library          *MUIMasterBase;

extern struct SignalSemaphore  lib_sem;
extern struct SignalSemaphore  lib_poolSem;
extern struct Library          *lib_base;
extern ULONG                   lib_segList;
extern struct Catalog          *lib_cat;
extern ULONG                   lib_flags;
extern APTR		               lib_pool;

extern struct MUI_CustomClass  *lib_attach;
extern struct MUI_CustomClass  *lib_class;

/***************************************************************************/

enum
{
    BASEFLG_Init  = 1<<0,
    BASEFLG_MUI20 = 1<<1,
};

/***************************************************************************/

#define MUIVER20 20

/***************************************************************************/
