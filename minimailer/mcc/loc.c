
#include "class.h"

#define CATCOMP_ARRAY
#include "loc.h"

/***********************************************************************/

static struct Catalog *
openCatalog(UBYTE *name,ULONG minVer,ULONG minRev)
{
    register struct Catalog *cat;

    if (cat = OpenCatalogA(NULL,name,NULL))
    {
        register ULONG ver = cat->cat_Version;

        if ((ver<minVer) ? TRUE : ((ver==minVer) ? (cat->cat_Revision<minRev) : FALSE))
        {
            CloseCatalog(cat);
            cat = NULL;
        }
    }

    return cat;
}

/***********************************************************************/

void
initStrings(void)
{
    if (lib_cat = openCatalog(CATNAME,0,0))
    {
    	register struct CatCompArrayType *cca;
        register int                     cnt;

        for (cnt = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1, cca = (struct CatCompArrayType *)CatCompArray+cnt;
             cnt>=0;
             cnt--, cca--)
        {
            register UBYTE *s;

            if (s = GetCatalogStr(lib_cat,cca->cca_ID,cca->cca_Str)) cca->cca_Str = s;
        }
    }
}

/***********************************************************************/

UBYTE *
getString(ULONG id)
{
    register struct CatCompArrayType *cca;
    register int                     cnt;

    for (cnt = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1, cca = (struct CatCompArrayType *)CatCompArray+cnt;
         cnt>=0;
         cnt--, cca--) if (cca->cca_ID==id) return cca->cca_Str;

    return "";
}

/***********************************************************************/

void
localizeNewMenu(struct NewMenu *menu,ULONG *ids)
{
    while (menu->nm_Type!=NM_END)
    {
        register ULONG id = *ids++;

        if (id && menu->nm_Label!=NM_BARLABEL)
            menu->nm_Label = getString(id);

        menu++;
    }
}

/***********************************************************************/

