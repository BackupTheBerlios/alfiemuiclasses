#ifndef LOC_H
#define LOC_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define Msg_Welcome 1
#define Msg_To 2
#define Msg_To_Help 3
#define Msg_CC 4
#define Msg_CC_Help 5
#define Msg_Subject 6
#define Msg_Subject_Help 7
#define Msg_Text 8
#define Msg_Text_Help 9
#define Msg_RealName 10
#define Msg_RealName_Help 11
#define Msg_EMail 12
#define Msg_EMail_Help 13
#define Msg_Host 14
#define Msg_Host_Help 15
#define Msg_Port 16
#define Msg_Port_Help 17
#define Msg_Send 18
#define Msg_Send_Help 19
#define Msg_Stop 20
#define Msg_Stop_Help 21
#define Msg_Clear 22
#define Msg_Clear_Help 23
#define Msg_Cancel 24
#define Msg_Cancel_Help 25
#define Msg_Attach 26
#define Msg_Attach_Help 27
#define Msg_Attach_Title 28
#define Msg_CItem_Cut 100
#define Msg_CItem_Copy 101
#define Msg_CItem_Paste 102
#define Msg_CItem_Clear 103
#define Msg_CItem_ShowPrefs 104
#define Msg_CItem_Stop 105
#define Msg_CItem_Edit 106
#define Msg_CItem_MiniMailer 107
#define Msg_Resolving 300
#define Msg_Connecting 301
#define Msg_Sending 302
#define Msg_Done 303
#define Msg_NoMem 500
#define Msg_NoArg 501
#define Msg_InvalidArg 502
#define Msg_NoSocketBase 503
#define Msg_NoSocket 504
#define Msg_CantResolve 505
#define Msg_NoLocalInfo 506
#define Msg_CantConnect 507
#define Msg_ErrSend 508
#define Msg_ErrRecv 509
#define Msg_SeverError 510
#define Msg_ProtoError 511
#define Msg_Broken 512
#define Msg_IoErr 513

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define Msg_Welcome_STR "Welcome to MiniMail!"
#define Msg_To_STR "_To"
#define Msg_To_Help_STR "The recipient of the EMail."
#define Msg_CC_STR "_CC"
#define Msg_CC_Help_STR "Send also to this recipient."
#define Msg_Subject_STR "_Subject"
#define Msg_Subject_Help_STR "The subject of the EMail."
#define Msg_Text_STR "Te_xt"
#define Msg_Text_Help_STR "The text of the EMail."
#define Msg_RealName_STR "_Name"
#define Msg_RealName_Help_STR "Your real name."
#define Msg_EMail_STR "_EMail addr"
#define Msg_EMail_Help_STR "Your EMail address."
#define Msg_Host_STR "SMTP _host"
#define Msg_Host_Help_STR "Your SMTP host."
#define Msg_Port_STR "_Port"
#define Msg_Port_Help_STR "The SMPT host port"
#define Msg_Send_STR "_Send"
#define Msg_Send_Help_STR "Send EMail."
#define Msg_Stop_STR "_Stop"
#define Msg_Stop_Help_STR "Stop sending."
#define Msg_Clear_STR "_Clear"
#define Msg_Clear_Help_STR "Clear all the fields."
#define Msg_Cancel_STR "_Cancel"
#define Msg_Cancel_Help_STR "Exit without sending."
#define Msg_Attach_STR "Attach:"
#define Msg_Attach_Help_STR "List of attachments."
#define Msg_Attach_Title_STR "Selects file(s) to attach"
#define Msg_CItem_Cut_STR "Cut"
#define Msg_CItem_Copy_STR "Copy"
#define Msg_CItem_Paste_STR "Paste"
#define Msg_CItem_Clear_STR "Clear"
#define Msg_CItem_ShowPrefs_STR "Show prefs"
#define Msg_CItem_Stop_STR "Stop"
#define Msg_CItem_Edit_STR "Edit"
#define Msg_CItem_MiniMailer_STR "MiniMailer"
#define Msg_Resolving_STR "Resolving SMTP host..."
#define Msg_Connecting_STR "Connecting to SMTP host..."
#define Msg_Sending_STR "Sending message..."
#define Msg_Done_STR "Message sent."
#define Msg_NoMem_STR "Not enogh memory available."
#define Msg_NoArg_STR "Required argument missed."
#define Msg_InvalidArg_STR "Invalid argument format."
#define Msg_NoSocketBase_STR "Can't open bsdsocket.libray 4+."
#define Msg_NoSocket_STR "Can't create a STREAM socket."
#define Msg_CantResolve_STR "Can't resolve SMTP host name."
#define Msg_NoLocalInfo_STR "Can't obtain local info."
#define Msg_CantConnect_STR "Can't connect to SMTP host."
#define Msg_ErrSend_STR "Error sending."
#define Msg_ErrRecv_STR "Error receiving."
#define Msg_SeverError_STR "Server error: %s"
#define Msg_ProtoError_STR "Invalid answer from the SMTP server."
#define Msg_Broken_STR "Interrupted by user."
#define Msg_IoErr_STR "Can't send file(s)."

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {Msg_Welcome,(STRPTR)Msg_Welcome_STR},
    {Msg_To,(STRPTR)Msg_To_STR},
    {Msg_To_Help,(STRPTR)Msg_To_Help_STR},
    {Msg_CC,(STRPTR)Msg_CC_STR},
    {Msg_CC_Help,(STRPTR)Msg_CC_Help_STR},
    {Msg_Subject,(STRPTR)Msg_Subject_STR},
    {Msg_Subject_Help,(STRPTR)Msg_Subject_Help_STR},
    {Msg_Text,(STRPTR)Msg_Text_STR},
    {Msg_Text_Help,(STRPTR)Msg_Text_Help_STR},
    {Msg_RealName,(STRPTR)Msg_RealName_STR},
    {Msg_RealName_Help,(STRPTR)Msg_RealName_Help_STR},
    {Msg_EMail,(STRPTR)Msg_EMail_STR},
    {Msg_EMail_Help,(STRPTR)Msg_EMail_Help_STR},
    {Msg_Host,(STRPTR)Msg_Host_STR},
    {Msg_Host_Help,(STRPTR)Msg_Host_Help_STR},
    {Msg_Port,(STRPTR)Msg_Port_STR},
    {Msg_Port_Help,(STRPTR)Msg_Port_Help_STR},
    {Msg_Send,(STRPTR)Msg_Send_STR},
    {Msg_Send_Help,(STRPTR)Msg_Send_Help_STR},
    {Msg_Stop,(STRPTR)Msg_Stop_STR},
    {Msg_Stop_Help,(STRPTR)Msg_Stop_Help_STR},
    {Msg_Clear,(STRPTR)Msg_Clear_STR},
    {Msg_Clear_Help,(STRPTR)Msg_Clear_Help_STR},
    {Msg_Cancel,(STRPTR)Msg_Cancel_STR},
    {Msg_Cancel_Help,(STRPTR)Msg_Cancel_Help_STR},
    {Msg_Attach,(STRPTR)Msg_Attach_STR},
    {Msg_Attach_Help,(STRPTR)Msg_Attach_Help_STR},
    {Msg_Attach_Title,(STRPTR)Msg_Attach_Title_STR},
    {Msg_CItem_Cut,(STRPTR)Msg_CItem_Cut_STR},
    {Msg_CItem_Copy,(STRPTR)Msg_CItem_Copy_STR},
    {Msg_CItem_Paste,(STRPTR)Msg_CItem_Paste_STR},
    {Msg_CItem_Clear,(STRPTR)Msg_CItem_Clear_STR},
    {Msg_CItem_ShowPrefs,(STRPTR)Msg_CItem_ShowPrefs_STR},
    {Msg_CItem_Stop,(STRPTR)Msg_CItem_Stop_STR},
    {Msg_CItem_Edit,(STRPTR)Msg_CItem_Edit_STR},
    {Msg_CItem_MiniMailer,(STRPTR)Msg_CItem_MiniMailer_STR},
    {Msg_Resolving,(STRPTR)Msg_Resolving_STR},
    {Msg_Connecting,(STRPTR)Msg_Connecting_STR},
    {Msg_Sending,(STRPTR)Msg_Sending_STR},
    {Msg_Done,(STRPTR)Msg_Done_STR},
    {Msg_NoMem,(STRPTR)Msg_NoMem_STR},
    {Msg_NoArg,(STRPTR)Msg_NoArg_STR},
    {Msg_InvalidArg,(STRPTR)Msg_InvalidArg_STR},
    {Msg_NoSocketBase,(STRPTR)Msg_NoSocketBase_STR},
    {Msg_NoSocket,(STRPTR)Msg_NoSocket_STR},
    {Msg_CantResolve,(STRPTR)Msg_CantResolve_STR},
    {Msg_NoLocalInfo,(STRPTR)Msg_NoLocalInfo_STR},
    {Msg_CantConnect,(STRPTR)Msg_CantConnect_STR},
    {Msg_ErrSend,(STRPTR)Msg_ErrSend_STR},
    {Msg_ErrRecv,(STRPTR)Msg_ErrRecv_STR},
    {Msg_SeverError,(STRPTR)Msg_SeverError_STR},
    {Msg_ProtoError,(STRPTR)Msg_ProtoError_STR},
    {Msg_Broken,(STRPTR)Msg_Broken_STR},
    {Msg_IoErr,(STRPTR)Msg_IoErr_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x01\x00\x16"
    Msg_Welcome_STR "\x00\x00"
    "\x00\x00\x00\x02\x00\x04"
    Msg_To_STR "\x00"
    "\x00\x00\x00\x03\x00\x1C"
    Msg_To_Help_STR "\x00"
    "\x00\x00\x00\x04\x00\x04"
    Msg_CC_STR "\x00"
    "\x00\x00\x00\x05\x00\x1E"
    Msg_CC_Help_STR "\x00\x00"
    "\x00\x00\x00\x06\x00\x0A"
    Msg_Subject_STR "\x00\x00"
    "\x00\x00\x00\x07\x00\x1A"
    Msg_Subject_Help_STR "\x00"
    "\x00\x00\x00\x08\x00\x06"
    Msg_Text_STR "\x00"
    "\x00\x00\x00\x09\x00\x18"
    Msg_Text_Help_STR "\x00\x00"
    "\x00\x00\x00\x0A\x00\x06"
    Msg_RealName_STR "\x00"
    "\x00\x00\x00\x0B\x00\x10"
    Msg_RealName_Help_STR "\x00"
    "\x00\x00\x00\x0C\x00\x0C"
    Msg_EMail_STR "\x00"
    "\x00\x00\x00\x0D\x00\x14"
    Msg_EMail_Help_STR "\x00"
    "\x00\x00\x00\x0E\x00\x0C"
    Msg_Host_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x10"
    Msg_Host_Help_STR "\x00"
    "\x00\x00\x00\x10\x00\x06"
    Msg_Port_STR "\x00"
    "\x00\x00\x00\x11\x00\x14"
    Msg_Port_Help_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\x06"
    Msg_Send_STR "\x00"
    "\x00\x00\x00\x13\x00\x0C"
    Msg_Send_Help_STR "\x00"
    "\x00\x00\x00\x14\x00\x06"
    Msg_Stop_STR "\x00"
    "\x00\x00\x00\x15\x00\x0E"
    Msg_Stop_Help_STR "\x00"
    "\x00\x00\x00\x16\x00\x08"
    Msg_Clear_STR "\x00\x00"
    "\x00\x00\x00\x17\x00\x16"
    Msg_Clear_Help_STR "\x00"
    "\x00\x00\x00\x18\x00\x08"
    Msg_Cancel_STR "\x00"
    "\x00\x00\x00\x19\x00\x16"
    Msg_Cancel_Help_STR "\x00"
    "\x00\x00\x00\x1A\x00\x08"
    Msg_Attach_STR "\x00"
    "\x00\x00\x00\x1B\x00\x16"
    Msg_Attach_Help_STR "\x00\x00"
    "\x00\x00\x00\x1C\x00\x1A"
    Msg_Attach_Title_STR "\x00"
    "\x00\x00\x00\x64\x00\x04"
    Msg_CItem_Cut_STR "\x00"
    "\x00\x00\x00\x65\x00\x06"
    Msg_CItem_Copy_STR "\x00\x00"
    "\x00\x00\x00\x66\x00\x06"
    Msg_CItem_Paste_STR "\x00"
    "\x00\x00\x00\x67\x00\x06"
    Msg_CItem_Clear_STR "\x00"
    "\x00\x00\x00\x68\x00\x0C"
    Msg_CItem_ShowPrefs_STR "\x00\x00"
    "\x00\x00\x00\x69\x00\x06"
    Msg_CItem_Stop_STR "\x00\x00"
    "\x00\x00\x00\x6A\x00\x06"
    Msg_CItem_Edit_STR "\x00\x00"
    "\x00\x00\x00\x6B\x00\x0C"
    Msg_CItem_MiniMailer_STR "\x00\x00"
    "\x00\x00\x01\x2C\x00\x18"
    Msg_Resolving_STR "\x00\x00"
    "\x00\x00\x01\x2D\x00\x1C"
    Msg_Connecting_STR "\x00\x00"
    "\x00\x00\x01\x2E\x00\x14"
    Msg_Sending_STR "\x00\x00"
    "\x00\x00\x01\x2F\x00\x0E"
    Msg_Done_STR "\x00"
    "\x00\x00\x01\xF4\x00\x1C"
    Msg_NoMem_STR "\x00"
    "\x00\x00\x01\xF5\x00\x1A"
    Msg_NoArg_STR "\x00"
    "\x00\x00\x01\xF6\x00\x1A"
    Msg_InvalidArg_STR "\x00\x00"
    "\x00\x00\x01\xF7\x00\x20"
    Msg_NoSocketBase_STR "\x00"
    "\x00\x00\x01\xF8\x00\x1E"
    Msg_NoSocket_STR "\x00"
    "\x00\x00\x01\xF9\x00\x1E"
    Msg_CantResolve_STR "\x00"
    "\x00\x00\x01\xFA\x00\x1A"
    Msg_NoLocalInfo_STR "\x00\x00"
    "\x00\x00\x01\xFB\x00\x1C"
    Msg_CantConnect_STR "\x00"
    "\x00\x00\x01\xFC\x00\x10"
    Msg_ErrSend_STR "\x00\x00"
    "\x00\x00\x01\xFD\x00\x12"
    Msg_ErrRecv_STR "\x00\x00"
    "\x00\x00\x01\xFE\x00\x12"
    Msg_SeverError_STR "\x00\x00"
    "\x00\x00\x01\xFF\x00\x26"
    Msg_ProtoError_STR "\x00\x00"
    "\x00\x00\x02\x00\x00\x16"
    Msg_Broken_STR "\x00\x00"
    "\x00\x00\x02\x01\x00\x14"
    Msg_IoErr_STR "\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};


#ifdef CATCOMP_CODE

STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
LONG   *l;
UWORD  *w;
STRPTR  builtIn;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum)
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtIn = (STRPTR)((ULONG)l + 6);

#undef LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#undef LocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* LOC_H */
