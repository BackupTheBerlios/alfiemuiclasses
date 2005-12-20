
#include "class.h"

/***********************************************************************/

#define _HELP(h) h ? MUIA_ShortHelp : TAG_IGNORE, h ? (ULONG)getString(help) : 0L

/***********************************************************************/

Object *
olabel(ULONG text)
{
    return Label((ULONG)getString(text));
}

/****************************************************************************/

Object *
ollabel(ULONG text)
{
    return LLabel((ULONG)getString(text));
}

/***********************************************************************/

Object *
olabel1(ULONG text)
{
    return Label1((ULONG)getString(text));
}

/***********************************************************************/

Object *
ollabel1(ULONG text)
{
    return LLabel1((ULONG)getString(text));
}

/***********************************************************************/

Object *
olabel2(ULONG text)
{
    return Label2((ULONG)getString(text));
}

/****************************************************************************/

Object *
ostring(ULONG label,ULONG help,ULONG weight,ULONG max)
{
    return TextinputObject,
        _HELP(help),
        StringFrame,
        MUIA_Weight,                weight,
        MUIA_ControlChar,           getKeyChar(getString(label)),
        MUIA_CycleChain,            TRUE,
        MUIA_Textinput_MaxLen,      max,
        MUIA_Textinput_AdvanceOnCR, TRUE,
    End;
}

/****************************************************************************/

Object *
otext(ULONG help)
{
    return TextinputscrollObject,
            _HELP(help),
            StringFrame,
            MUIA_CycleChain,                       TRUE,
            MUIA_Textinput_MinVersion,             29,
            MUIA_Textinput_DefaultPopup,           FALSE,
            MUIA_Textinput_Multiline,              TRUE,
            MUIA_Textinput_AutoExpand,             TRUE,
            MUIA_Textinputscroll_VertScrollerOnly, TRUE,
            MUIA_Textinput_Styles,                 MUIV_Textinput_Styles_Email,
    End;
}

/****************************************************************************/

Object *
obutton(ULONG label,ULONG help)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Button,(ULONG)getString(label)))
        SetAttrs(obj,MUIA_CycleChain, TRUE,_HELP(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

ULONG
xget(Object *obj,Tag attr)
{
    ULONG x;

    get(obj,attr,&x);

    return x;
}

/***********************************************************************/

ULONG
getKeyChar(UBYTE *string)
{
    register ULONG res = 0;

    if (string)
    {
        for (; *string && *string!='_'; string++);
        if (*string++) res = ToLower(*string);
    }

    return res;
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

void
sprintf(UBYTE *buf,UBYTE *fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}

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
snprintf(UBYTE *buf,int size,UBYTE *fmt,...)
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

ULONG STDARGS
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}

UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void STDARGS
sprintf(UBYTE *to,UBYTE *fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,to);
}

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
snprintf(UBYTE *buf,int size,UBYTE *fmt,...)
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

APTR
allocVecPooled(APTR pool,ULONG size)
{
    register ULONG *mem;

    if (mem = AllocPooled(pool,size = size+sizeof(ULONG))) *mem++ = size;

    return mem;
}

/****************************************************************************/

void
freeVecPooled(APTR pool,APTR mem)
{
    FreePooled(pool,(ULONG *)mem-1,*((ULONG *)mem-1));
}

/****************************************************************************/

APTR
allocArbitratePooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = AllocPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

void
freeArbitratePooled(APTR mem,ULONG size)
{
    ObtainSemaphore(&lib_poolSem);
    FreePooled(lib_pool,mem,size);
    ReleaseSemaphore(&lib_poolSem);
}

/***********************************************************************/

APTR
allocArbitrateVecPooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = allocVecPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

void
freeArbitrateVecPooled(APTR mem)
{
    ObtainSemaphore(&lib_poolSem);
    freeVecPooled(lib_pool,mem);
    ReleaseSemaphore(&lib_poolSem);
}

/***********************************************************************/


