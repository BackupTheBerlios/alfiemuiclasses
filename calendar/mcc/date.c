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

#ifdef __MORPHOS__
ULONG CALGregToDate(ULONG greg,ULONG *day,ULONG *month,ULONG *year)
#else
ULONG ASM SAVEDS CALGregToDate(REG(d0,ULONG greg),REG(a0,ULONG *day),REG(a1,ULONG *month),REG(a2,ULONG *year))
#endif
{
    register ULONG a, b, c, d, e, m;

    a = greg+32044;
    b = (4*a+3)/146097;
    c = a-(b*146097)/4;

    d = (4*c+3)/1461;
    e = c - (1461*d)/4;
    m = (5*e+2)/153;

    *day   = e - (153*m+2)/5 + 1;
    *month = m + 3 - 12*(m/10);
    *year  = b*100 + d - 4800 + m/10;

    return 0;
}

#ifdef __MORPHOS__
ULONG LIB_CALGregToDate(void)
{
    return CALGregToDate((ULONG)REG_D0,(ULONG *)REG_A0,(ULONG *)REG_A1,(ULONG *)REG_A2);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALDateToGreg(ULONG day,ULONG month,ULONG year)
#else
ULONG ASM SAVEDS CALDateToGreg(REG(d0,ULONG day),REG(d1,ULONG month),REG(d2,ULONG year))
#endif
{
    register ULONG a, y, m;

    a = (14-month)/12;
    y = year+4800-a;
    m = month + 12*a - 3;

    return day + (153*m+2)/5 + y*365 + y/4 - y/100 + y/400 - 32045;
}

#ifdef __MORPHOS__
ULONG LIB_CALDateToGreg(void)
{
    return CALDateToGreg((ULONG)REG_D0,(ULONG)REG_D1,(ULONG)REG_D2);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALIsLeap(ULONG year)
#else
ULONG ASM SAVEDS CALIsLeap(REG(d0,ULONG year))
#endif
{
    if (year%4!=0) return 0;
    if (year%100!=0) return 1;
    if (year%400!=0) return 0;
    return 1;
}

#ifdef __MORPHOS__
ULONG LIB_CALIsLeap(void)
{
    return CALIsLeap((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGetDay(ULONG greg)
#else
ULONG ASM SAVEDS CALGetDay(REG(d0,ULONG greg))
#endif
{
    ULONG d, m, y;

    CALGregToDate(greg,&d,&m,&y);

    return d;
}

#ifdef __MORPHOS__
ULONG LIB_CALGetDay(void)
{
    return CALGetDay((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGetMonth(ULONG greg)
#else
ULONG ASM SAVEDS CALGetMonth(REG(d0,ULONG greg))
#endif
{
    ULONG d, m, y;

    CALGregToDate(greg,&d,&m,&y);

    return m;
}

#ifdef __MORPHOS__
ULONG LIB_CALGetMonth(void)
{
    return CALGetMonth((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGetYear(ULONG greg)
#else
ULONG ASM SAVEDS CALGetYear(REG(d0,ULONG greg))
#endif
{
    ULONG d, m, y;

    CALGregToDate(greg,&d,&m,&y);

    return y;
}

#ifdef __MORPHOS__
ULONG LIB_CALGetYear(void)
{
    return CALGetYear((ULONG)REG_D0);
}
#endif

/***********************************************************************/

static ULONG monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

#ifdef __MORPHOS__
ULONG CALGetMonthDays(ULONG month,ULONG year)
#else
ULONG ASM SAVEDS CALGetMonthDays(REG(d0,ULONG month),REG(d1,ULONG year))
#endif
{
    register ULONG res;

    month--;
    if (month<0 || month>11) return 0;

    res = monthDays[month];

    if (month!=1) return res;
    return res+CALIsLeap(year);
}

#ifdef __MORPHOS__
ULONG LIB_CALGetMonthDays(void)
{
    return CALGetMonthDays((ULONG)REG_D0,(ULONG)REG_D1);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGetToday(void)
#else
ULONG ASM SAVEDS CALGetToday(void)
#endif
{
    struct DateStamp ds;

    DateStamp(&ds);
    return (ULONG)(TODAYOFS+ds.ds_Days);
}

#ifdef __MORPHOS__
ULONG LIB_CALGetToday(void)
{
    return CALGetToday();
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALDateWeekday(ULONG day,ULONG month,ULONG year)
#else
ULONG ASM SAVEDS CALDateWeekday(REG(d0,ULONG day),REG(d1,ULONG month),REG(d2,ULONG year))
#endif
{
    register ULONG a, y, m;

    a = (14 - month) / 12;
    y = year - a;
    m = month + 12*a - 2;

    return ((day + y + y/4 - y/100 + y/400 + (31*m)/12)) % 7;
}

#ifdef __MORPHOS__
ULONG LIB_CALDateWeekday(void)
{
    return CALDateWeekday((ULONG)REG_D0,(ULONG)REG_D1,(ULONG)REG_D2);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGregWeekday(ULONG greg)
#else
ULONG ASM SAVEDS CALGregWeekday(REG(d0,ULONG greg))
#endif
{
    ULONG day, month, year;

    CALGregToDate(greg,&day,&month,&year);

    return CALDateWeekday(day,month,year);
}

#ifdef __MORPHOS__
ULONG LIB_CALGregWeekday(void)
{
    return CALGregWeekday((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CALGetJulian(ULONG greg)
#else
ULONG ASM SAVEDS CALGetJulian(REG(d0,ULONG greg))
#endif
{
    ULONG day, month, year;

    CALGregToDate(greg,&day,&month,&year);

    return greg-CALDateToGreg(1,1,year)+1;
}

#ifdef __MORPHOS__
ULONG LIB_CALGetJulian(void)
{
    return CALGetJulian((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
void CALGetEaster(ULONG year,ULONG *d,ULONG *m)
#else
void ASM SAVEDS CALGetEaster(REG(d0,ULONG year),REG(a0,ULONG *d),REG(a1,ULONG *m))
#endif
{
    register ULONG G, C, H, I, J, L;

    G = year%19;

    C = year/100;
    H = (C - C/4 - (8*C+13)/25 + 19*G + 15)%30;
    I = H - (H/28)*(1 - (29/(H + 1))*((21 - G)/11));
    J = (year + year/4 + I + 2 - C + C/4)%7;
    L = I - J;

    *m = 3 + (L + 40)/44;
    *d = L + 28 - 31*(*m/4);
}

#ifdef __MORPHOS__
void LIB_CALGetEaster(void)
{
    CALGetEaster((ULONG)REG_D0,(ULONG *)REG_A0,(ULONG *)REG_A1);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
void CALValidateDate(ULONG *day,ULONG *month,ULONG *year)
#else
void ASM SAVEDS CALValidateDate(REG(a0,ULONG *day),REG(a1,ULONG *month),REG(a2,ULONG *year))
#endif
{
    register ULONG d, m, y;

    d = *day;
    m = *month;
    y = *year;

    if (y<MINYEAR) y = MINYEAR;
    else if (y>MAXYEAR) y = MAXYEAR;

    if (m<1) m = 1;
    else if (m>12) m = 12;

    if (d<1) d = 1;
    else
        if (d>=29)
        {
            ULONG md;

            md = CALGetMonthDays(m,y);
            if (d>md) d = md;
        }

    *day   = d;
    *month = m;
    *year  = y;
}

#ifdef __MORPHOS__
void LIB_CALValidateDate(void)
{
    CALValidateDate((ULONG *)REG_A0,(ULONG *)REG_A1,(ULONG *)REG_A2);
}
#endif

/***********************************************************************/

static void
putChar(STRPTR buf,UBYTE c,int *size)
{
    if (*size>0)
    {
        *buf = c;
        *size = *size-1;
    }
}

static STRPTR
mystrcat(STRPTR a,STRPTR b,int *size)
{
    while (*b) putChar(a++,*b++,size);

    return a;
}

#ifdef __MORPHOS__
STRPTR CALFormatDate(struct Locale *locale,STRPTR buf,int size,STRPTR fmt,ULONG greg)
#else
STRPTR ASM SAVEDS CALFormatDate(REG(a0,struct Locale *locale),REG(a1,STRPTR buf),REG(d0,int size),REG(a2,STRPTR fmt),REG(d1,ULONG greg))
#endif
{
    register STRPTR b, f;
    register ULONG  sp, wd;
    ULONG           d, m, y;
    int             s;

    CALGregToDate(greg,&d,&m,&y);
    wd = CALGregWeekday(greg);

    if (size<=0) return buf;
    s = size-1;
    sp = 0;

    if (!locale) locale = lib_locale;

    if (fmt==FORMTDATEFMT_SYSTEM) fmt = locale->loc_DateFormat;
    else if (fmt==FORMTDATEFMT_SYSTEMSHORT) fmt = locale->loc_ShortDateFormat;

    for (b = buf, f = fmt; *f && s; f++)
    {
        if (sp)
        {
            switch (*f)
            {
                case 'a':
                {
                    STRPTR c;

                    c = GetLocaleStr(locale,ABDAY_1+wd);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'A':
                {
                    STRPTR c;

                    c = GetLocaleStr(locale,DAY_1+wd);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'b': case 'h':
                {
                    STRPTR c;

                    c = GetLocaleStr(locale,ABMON_1+m-1);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'B':
                {
                    STRPTR c;

                    c = GetLocaleStr(locale,MON_1+m-1);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'd':
                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }
                    break;

                case 'e':
                    if (d<10)
                    {
                        putChar(b++,' ',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }
                    break;

                case 'm':
                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }
                    break;

                case 'M':
                    if (m<10)
                    {
                        putChar(b++,' ',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }
                    break;

                case 'U':
                    putChar(b++,48+wd,&s);
                    break;

                case 'y': case 'Y':
                {
                    REGARRAY UBYTE yb[16];

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);
                    break;
                }

                case 'g':
                {
                    REGARRAY UBYTE gb[16];

                    sprintf(gb,"%ld",greg);
                    b = mystrcat(b,gb,&s);
                    break;
                }

                case 'I':
                {
                    REGARRAY UBYTE yb[16];

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    break;
                }

                case 'x':
                {
                    REGARRAY UBYTE yb[16];

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    break;
                }

                case 'E':
                {
                    REGARRAY UBYTE yb[16];

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    break;
                }

                case '%':
                    putChar(b++,'%',&s);
                    break;

                case 'n':
                    putChar(b++,'\n',&s);
                    break;

                case 't':
                    putChar(b++,'\t',&s);
                    break;
            }

            sp = 0;

            continue;
        }

        if (*f=='%') sp = 1;
        else putChar(b++,*f,&s);
    }

    *(buf+size-s-1) = 0;

    return buf+size-s-1;
}

#ifdef __MORPHOS__
STRPTR LIB_CALFormatDate(void)
{
    return CALFormatDate((struct Locale *)REG_A0,(STRPTR)REG_A1,(int)REG_D0,(STRPTR)REG_A2,(ULONG)REG_D1);
}
#endif

/***********************************************************************/
