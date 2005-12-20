#ifndef _CLASS_H
#define _CLASS_H

#define __NOLIBBASE__
#define __USE_SYSBASE

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/intuition.h>
#include <proto/locale.h>
#include <proto/muimaster.h>

#include <clib/alib_protos.h>
#include <clib/debug_protos.h>

#include <libraries/gadtools.h>
#include <utility/hooks.h>

#include <mui/muiundoc.h>
#include <mui/textinput_mcc.h>
#include <MUI/MiniMailer_mcc.h>

#ifndef __MORPHOS__
#include <dos.h>
#endif
#include <string.h>
#include <macros.h>

#include "base.h"

/***********************************************************************/

extern char LIBNAME[];

/***********************************************************************/

#define DEFHOSTPORT     25

#define TOLEN          128
#define CCLEN          128
#define SUBJECTLEN     128
#define HOSTLEN        256
#define EMAILLEN       128
#define REALNAMELEN     64
#define ERRBUFLEN      256
#define CODESETNAMELEN  64

#define CATNAME "MiniMailer_mcc.catalog"

/***********************************************************************/

#define attachObject   NewObject(lib_attach->mcc_Class,NULL

/***********************************************************************/

#define MTITLE(t)      {NM_TITLE,(STRPTR)(t),0,0,0,(APTR)(t)}
#define MITEM(t,d)     {NM_ITEM,(STRPTR)(t),0,0,0,(APTR)(d)}
#define MTITEM(t,d)    {NM_ITEM,(STRPTR)(t),0,CHECKIT|MENUTOGGLE,0,(APTR)(d)}
#define MFITEM(t,f)    {NM_ITEM,(STRPTR)(t),0,(UWORD)(f),0,(APTR)(t)}
#define MFMITEM(t,f,m) {NM_ITEM,(STRPTR)(t),0,(UWORD)(f),(LONG)(m),(APTR)(t)}
#define MBAR           {NM_ITEM,(STRPTR)NM_BARLABEL,0,0,0,NULL}
#define MEND           {NM_END,NULL,0,0,0,NULL}

#define nfset(obj,attr,value)         SetAttrs(obj,MUIA_Group_Forward,FALSE,attr,value,TAG_DONE)
#define wspace(w)                     RectangleObject, MUIA_Weight, w, End
#define _sbbut(sb,ID)                 ((Object *)DoMethod((Object *)(sb),MUIM_TheBar_GetObject,(ULONG)(ID)))

#define HDATA(h) ((struct data *)((struct Hook *)(h))->h_Data)

/***********************************************************************/

#define PREFSVERSION 1

struct prefs
{
    ULONG  version;

    char   host[HOSTLEN];
    long   hostPort;
    char   email[EMAILLEN];
    char   realName[REALNAMELEN];
};

/***********************************************************************/

enum
{
    B64_OK = 0,
    B64_IoErr,
    B64_NoMem,
};

#include "class_protos.h"

/***********************************************************************/

#endif /* _CLASS_H */
