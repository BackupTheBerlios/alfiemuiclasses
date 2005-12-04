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

#ifndef __MORPHOS__
ULONG STDARGS
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}
#endif

Object *
oflabel(ULONG id)
{
    return FreeLabel((ULONG)getString(id));
}

/***********************************************************************/

Object *
ofhspace(STRPTR string)
{
    return RectangleObject, MUIA_FixHeightTxt, (ULONG)string, End;
}

/***********************************************************************/

Object *
ocheck(ULONG key,ULONG help)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Checkmark,(ULONG)getString(key)))
        SetAttrs(obj,MUIA_CycleChain,TRUE,MUIA_ShortHelp,(ULONG)getString(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

Object *
opop(ULONG type,ULONG key)
{
    register Object *obj;

    if (!(lib_flags & BASEFLG_MUI4) && (type==MUII_PopFont))
        type = MUII_PopUp;

    if (obj = MUI_MakeObject(MUIO_PopButton,type))
    {
        set(obj,MUIA_CycleChain,TRUE);

        if (key)
        {
            ULONG k;

            if (k = getKeyChar(getString(key)))
                set(obj,MUIA_ControlChar,ToLower(k));
        }
    }

    return obj;
}

/***********************************************************************/

Object *
opoppen(ULONG key,ULONG title,ULONG help)
{
    return PoppenObject,
        //MUIA_FixWidthTxt,  (ULONG)"nn",
        //MUIA_FixHeightTxt, (ULONG)"\n",
        MUIA_Window_Title, (ULONG)getString(title),
        MUIA_ControlChar,  getKeyChar(getString(key)),
        MUIA_Draggable,    TRUE,
        MUIA_CycleChain,   1,
        MUIA_ShortHelp,    getString(help),
    End;
}

/***********************************************************************/

#define PopimageObject MUI_NewObject(MUIC_Popimage

Object *
opopimage(ULONG type,ULONG key,ULONG title,ULONG help)
{
    return PopimageObject,
        //MUIA_FixWidthTxt,      (ULONG)"nn",
        //MUIA_FixHeightTxt,     (ULONG)"\n",
        MUIA_Imageadjust_Type, type,
        MUIA_Window_Title,     (ULONG)getString(title),
        MUIA_ControlChar,      getKeyChar(getString(key)),
        MUIA_Draggable,        TRUE,
        MUIA_CycleChain,       1,
        MUIA_ShortHelp,        getString(help),
    End;
}

/***********************************************************************/

Object *
opopfri(ULONG key,ULONG title,ULONG help)
{
    return MUI_NewObject("Popfrimage.mui",
        MUIA_Window_Title,     (ULONG)getString(title),
        MUIA_ControlChar,      (ULONG)getKeyChar(getString(key)),
        MUIA_CycleChain,       TRUE,
        MUIA_ShortHelp,        (ULONG)getString(help),
        0x80421794, 0,
        0x8042a547, 0,
        0x80426a55, 1,
        0x8042a92b, 0,
    End;
}

/***********************************************************************/

struct stream
{
    UBYTE   *buf;
    int     size;
    int     counter;
    int     stop;
};

#ifdef __MORPHOS__

static void
snprintfStuff(void)
{
    register struct stream *s = (struct stream *)REG_A3;
    register UBYTE         c  = (UBYTE)REG_D0;

    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

static struct EmulLibEntry snprintfStuffTrap = {TRAP_LIB,0,(void *)&snprintfStuff};

int
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;
    va_list       va;

    va_start(va,fmt);

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,va->overflow_arg_area,(APTR)&snprintfStuffTrap,&s);

    va_end(va);

    return s.counter-1;
}

#else

static void ASM
snprintfStuff(REG(d0,UBYTE c),REG(a3,struct stream *s))
{
    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

int STDARGS
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,&fmt+1,(APTR)snprintfStuff,&s);

    return s.counter-1;
}

#endif

/***********************************************************************/

