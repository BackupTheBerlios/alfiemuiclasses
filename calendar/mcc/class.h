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

#ifndef _CLASS_H
#define _CLASS_H

#define __NOLIBBASE__
#define __USE_SYSBASE

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/locale.h>

#include <clib/alib_protos.h>
#include <clib/debug_protos.h>

#include <mui/muiundoc.h>
#include <mui/Textinput_mcc.h>

#ifdef __MORPHOS__
#include <dos.h>
#endif
#include <string.h>

#include <libraries/Calendar_mcc.h>
#include <mui/Calendar_mcc.h>
#include <mui/CalendarPrefs.h>

#include "base.h"
#include <macros.h>

/***********************************************************************/

extern char LIBNAME[];

/***********************************************************************/

#define daysObject    NewObject(lib_daysClass->mcc_Class,NULL
#define monthesObject NewObject(lib_monthesClass->mcc_Class,NULL
#define yearsObject   NewObject(lib_yearsClass->mcc_Class,NULL

/***********************************************************************/

#ifndef _backspec
#define _backspec(obj) ((APTR)(*((ULONG *)(((char *)(obj))+80))))
#endif

#ifndef NM_WHEEL_UP
#define NM_WHEEL_UP      0x7a
#endif

#ifndef NM_WHEEL_DOWN
#define NM_WHEEL_DOWN    0x7b
#endif

/***********************************************************************/

#include "class_protos.h"

/***********************************************************************/

#endif /* _CLASS_H */
