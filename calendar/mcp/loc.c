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

#define CATCOMP_ARRAY
#include "class.h"

/***********************************************************************/

void
initStrings(void)
{
    if (lib_cat = OpenCatalog(NULL,"Calendar.catalog",OC_BuiltInLanguage,(ULONG)"english",TAG_DONE))
    {
        register struct CatCompArrayType *cca;
        register int                     cnt;

        for (cnt = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1, cca = (struct CatCompArrayType *)CatCompArray+cnt;
             cnt>=0;
             cnt--, cca--)
        {
            register STRPTR s;

            if (s = GetCatalogStr(lib_cat,cca->cca_ID,cca->cca_Str)) cca->cca_Str = s;
        }
    }
}

/***********************************************************************/

STRPTR
getString(ULONG id)
{
    register struct CatCompArrayType    *cca;
    register int                        cnt;

    for (cnt = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1, cca = (struct CatCompArrayType *)CatCompArray+cnt;
         cnt>=0;
         cnt--, cca--) if (cca->cca_ID==id) return cca->cca_Str;

    return "";
}

/***********************************************************************/

ULONG
getKeyChar(STRPTR string)
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
