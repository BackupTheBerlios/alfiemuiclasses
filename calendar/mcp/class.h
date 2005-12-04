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
#include <proto/locale.h>

#include <clib/alib_protos.h>
#include <clib/debug_protos.h>

#include <mui/muiundoc.h>
#include <mui/Calendar_mcc.h>
#include <mui/CalendarPrefs.h>

#ifdef __MORPHOS__
#include <dos/dos.h>
#endif
#include <string.h>

#include "base.h"
#include <macros.h>

#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

#ifndef MUIM_Mccprefs_RegisterGadget
#define MUIM_Mccprefs_RegisterGadget 0x80424828
#endif

#ifndef MUII_PopFont
#define MUII_PopFont 42
#endif

#define wspace(w) RectangleObject, MUIA_Weight, w, End

/***********************************************************************/

#define wsdObject NewObject(lib_wsd->mcc_Class,NULL

/***********************************************************************/

#include "class_protos.h"

/***********************************************************************/

#endif /* _CLASS_H */
