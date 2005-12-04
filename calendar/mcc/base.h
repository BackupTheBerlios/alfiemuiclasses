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

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
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
extern struct Library          *MUIMasterBase;
extern struct GfxBase          *GfxBase;
extern struct LocaleBase       *LocaleBase;
extern struct Library          *DiskfontBase;

extern struct Locale           *lib_locale;
extern BYTE                    *lib_holidays;

extern struct SignalSemaphore  lib_sem;
extern struct Library          *lib_base;
extern ULONG                   lib_segList;
extern APTR                    lib_pool;
extern ULONG                   lib_flags;

extern STRPTR                  *lib_abmonthesArray;
extern STRPTR                  *lib_monthesArray;
extern STRPTR                  *lib_daysArray;
extern STRPTR                  lib_dayNumbers[];

extern struct MUI_CustomClass  *lib_daysClass;
extern struct MUI_CustomClass  *lib_monthesClass;
extern struct MUI_CustomClass  *lib_yearsClass;
extern struct MUI_CustomClass  *lib_class;

/***************************************************************************/

enum
{
    BASEFLG_Init  = 1<<0,
    BASEFLG_MUI   = 1<<1,
    BASEFLG_MUI20 = 1<<2,
    BASEFLG_MUI4  = 1<<3,
};

/***************************************************************************/
