
#include "class.h"

/****************************************************************************/

struct b64
{
    APTR data;
    ULONG (*func)(APTR,STRPTR,int);
    BPTR   in;
    UBYTE  buf[256];
    int    lineCounter;
    int    bufCounter;
    ULONG  error;
};

/****************************************************************************/

#define MAXLINELEN 72

#undef EOF
#define EOF (-1)

/****************************************************************************/

static UBYTE etable[] =
{
       65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
       75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
       85,   86,   87,   88,   89,   90,   97,   98,   99,  100,
      101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
      111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
      121,  122,   48,   49,   50,   51,   52,   53,   54,   55,
       56,   57,   43,   47,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0
};

/****************************************************************************/

static int
inchar(struct b64 *b64)
{
    register int c;

    if ((c = FGetC((BPTR)b64->in))==EOF)
    {
	if (IoErr()) b64->error = B64_IoErr;
    }

    return c;
}

/****************************************************************************/

static int
ochar(struct b64 *b64,int c)
{
    if (b64->bufCounter==sizeof(b64->buf))
    {
        if ((*b64->func)(b64->data,b64->buf,b64->bufCounter)) return EOF;
    	b64->bufCounter = 0;
    }

    if (b64->lineCounter==MAXLINELEN)
    {
        b64->buf[b64->bufCounter++] = '\r';
        if (b64->bufCounter==sizeof(b64->buf))
    	{
            if ((*b64->func)(b64->data,b64->buf,b64->bufCounter)) return EOF;
            b64->bufCounter = 0;
    	}

        b64->buf[b64->bufCounter++] = '\n';
        if (b64->bufCounter==sizeof(b64->buf))
    	{
	    if ((*b64->func)(b64->data,b64->buf,b64->bufCounter)) return EOF;
            b64->bufCounter = 0;
    	}

       b64->lineCounter = 0;
    }

    b64->buf[b64->bufCounter++] = c;

    if (b64->bufCounter==sizeof(b64->buf))
    {
        if ((*b64->func)(b64->data,b64->buf,b64->bufCounter)) return EOF;
        b64->bufCounter = 0;
    }

    b64->lineCounter++;

    return 0;
}

/****************************************************************************/

ULONG
encodeB64(APTR data,ULONG (*func)(APTR,STRPTR,int),STRPTR source)
{
    struct b64     b64;
    register BPTR  in;
    register ULONG stop, flags;

    flags = 0;

    if (!(in = Open(source,MODE_OLDFILE)))
    	return B64_IoErr;

    b64.data	    = data;
    b64.func        = func;
    b64.in          = in;
    b64.lineCounter = 0;
    b64.bufCounter  = 0;
    b64.error       = B64_OK;

    for (stop = FALSE; !stop; )
    {
        UBYTE    igroup[3], ogroup[4];
        register int i, c, n;

        igroup[0] = igroup[1] = igroup[2] = 0;

        for (n = 0; n<3; n++)
        {
            c = inchar(&b64);
            if (c==EOF)
            {
                stop = TRUE;
                break;
            }
            igroup[n] = (UBYTE) c;
        }

        if (n>0)
        {
            ogroup[0] = etable[igroup[0]>>2];
            ogroup[1] = etable[((igroup[0] & 3)<<4) | (igroup[1]>>4)];
            ogroup[2] = etable[((igroup[1] & 0xF)<<2) | (igroup[2]>>6)];
            ogroup[3] = etable[igroup[2] & 0x3F];

            if (n<3)
            {
                ogroup[3] = '=';
                if (n<2) ogroup[2] = '=';
            }

            for (i = 0; i<4; i++)
            {
                c = ochar(&b64,ogroup[i]);
                if (c==EOF)
                {
                    stop = TRUE;
                    break;
                }
            }
        }
    }

    Close(in);

    if ((b64.error==B64_OK) && (b64.bufCounter>0))
    	b64.error= (*b64.func)(b64.data,b64.buf,b64.bufCounter);

    return b64.error;
}

/****************************************************************************/

