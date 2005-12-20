
#include "class.h"
#define CATCOMP_NUMBERS
#include "loc.h"

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    return (ULONG)DoSuperNew(cl,obj,
            StringFrame,
            MUIA_CycleChain,                       TRUE,
            MUIA_Textinput_MinVersion,             29,
            MUIA_Textinput_DefaultPopup,           FALSE,
            MUIA_Textinput_Multiline,              TRUE,
            MUIA_Textinput_AutoExpand,             TRUE,
            MUIA_Textinputscroll_VertScrollerOnly, TRUE,
            MUIA_Textinput_Styles,                 MUIV_Textinput_Styles_Email,
            TAG_MORE, (ULONG)msg->ops_AttrList);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW: return mNew(cl,obj,(APTR)msg);

        default:     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initRootEditor(void)
{
    if (lib_rootEditor = MUI_CreateCustomClass(NULL,MUIC_Textinputscroll,NULL,0,DISP(dispatcher)))
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
freeRootEditor(void)
{
    MUI_DeleteCustomClass(lib_rootEditor);
    lib_rootEditor = NULL;
}

/***********************************************************************/
