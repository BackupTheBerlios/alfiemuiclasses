
#include "class.h"
#include <proto/iffparse.h>
#include <prefs/prefhdr.h>

/***********************************************************************/

#define ID_HOST     MAKE_ID('H','O','S','T')
#define ID_HOSTPORT MAKE_ID('H','P','O','R')
#define ID_REALNAME MAKE_ID('R','N','A','M')
#define ID_EMAIL    MAKE_ID('E','M','A','I')

static ULONG ids[] =
{
    ID_HOST,
    ID_HOSTPORT,
    ID_REALNAME,
    ID_EMAIL,

    0
};

/***********************************************************************/

ULONG
savePrefs(STRPTR file,struct prefs *prefs)
{
    register struct IFFHandle *iffh;
    register ULONG            res = FALSE;

    if (file && *file)
    {
        if (iffh = AllocIFF())
        {
            if (iffh->iff_Stream = Open(file,MODE_NEWFILE))
            {
                InitIFFasDOS(iffh);

                if (!OpenIFF(iffh,IFFF_WRITE))
                {
                    struct PrefHeader prhd;
                    ULONG             l;

                    if (PushChunk(iffh,ID_PREF,ID_FORM,IFFSIZE_UNKNOWN)) goto fail;
                    if (PushChunk(iffh,ID_PREF,ID_PRHD,sizeof(struct PrefHeader))) goto fail;

                    prhd.ph_Version = prefs->version;
                    prhd.ph_Type    = 0;
                    prhd.ph_Flags   = 0;

                    if (WriteChunkBytes(iffh,&prhd,sizeof(struct PrefHeader))!=sizeof(struct PrefHeader))
                        goto fail;

                    if (PopChunk(iffh)) goto fail;

                    if (prefs->host)
                    {
                        l = strlen(prefs->host);
                        if (PushChunk(iffh,ID_PREF,ID_HOST,l)) goto fail;
                        if (WriteChunkBytes(iffh,prefs->host,l)!=l) goto fail;
                        if (PopChunk(iffh)) goto fail;
                    }

                    if (PushChunk(iffh,ID_PREF,ID_HOSTPORT,sizeof(prefs->hostPort))) goto fail;
                    if (WriteChunkBytes(iffh,&prefs->hostPort,sizeof(prefs->hostPort))!=sizeof(prefs->hostPort)) goto fail;
                    if (PopChunk(iffh)) goto fail;

                    if (prefs->realName)
                    {
                        l = strlen(prefs->realName);
                        if (PushChunk(iffh,ID_PREF,ID_REALNAME,l)) goto fail;
                        if (WriteChunkBytes(iffh,prefs->realName,l)!=l) goto fail;
                        if (PopChunk(iffh)) goto fail;
                    }

                    if (prefs->email)
                    {
                        l = strlen(prefs->email);
                        if (PushChunk(iffh,ID_PREF,ID_EMAIL,l)) goto fail;
                        if (WriteChunkBytes(iffh,prefs->email,l)!=l) goto fail;
                        if (PopChunk(iffh)) goto fail;
                    }

                    res = TRUE;

                    fail: CloseIFF(iffh);
                }

                Close(iffh->iff_Stream);
                if (!res) DeleteFile(file);
            }

            FreeIFF(iffh);
        }
    }

    return res;
}

/**************************************************************************/

ULONG
loadPrefs(STRPTR file,struct prefs *prefs)
{
    register struct IFFHandle *iffh;
    register ULONG            res = FALSE;

    if (file && *file)
    {
        if (iffh = AllocIFF())
        {
            if (iffh->iff_Stream = Open(file,MODE_OLDFILE))
            {
                InitIFFasDOS(iffh);

                if (!OpenIFF(iffh,IFFF_READ))
                {
                    struct PrefHeader           prhd;
                    register struct ContextNode *cn;
                    register ULONG              *id, l;

                    if (StopChunk(iffh,ID_PREF,ID_PRHD)) goto fail;

                    for(id = ids; *id; id++) if (StopChunk(iffh,ID_PREF,*id)) goto fail;

                    if (ParseIFF(iffh,IFFPARSE_SCAN)) goto fail;

                    if (!(cn = CurrentChunk(iffh))) goto fail;

                    if ((cn->cn_Type!=ID_PREF) || (cn->cn_ID!=ID_PRHD) ||
                        (cn->cn_Size!=sizeof(struct PrefHeader))) goto fail;

                    if (ReadChunkBytes(iffh,&prhd,cn->cn_Size)!=cn->cn_Size) goto fail;

                    if (prhd.ph_Version>PREFSVERSION) goto fail;

                    for (;;)
                    {
                        register ULONG error;

                        error = ParseIFF(iffh,IFFPARSE_SCAN);
                        if (error==IFFERR_EOF) break;
                        else if (error) goto fail;

                        if (!(cn = CurrentChunk(iffh))) goto fail;

                        if (cn->cn_Type!=ID_PREF) continue;

                        if (cn->cn_ID==ID_HOST)
                        {
                            if ((l = cn->cn_Size)<sizeof(prefs->host))
                            {
                                if (ReadChunkBytes(iffh,prefs->host,l)!=l) goto fail;
                                prefs->host[l] = 0;
                            }
                        }

                        if ((cn->cn_ID==ID_HOSTPORT) && ((l = cn->cn_Size)==sizeof(prefs->hostPort)))
                        {
                            if (ReadChunkBytes(iffh,&prefs->hostPort,l)!=l) goto fail;
                            continue;
                        }

                        if (cn->cn_ID==ID_EMAIL)
                        {
                            if ((l = cn->cn_Size)<sizeof(prefs->email))
                            {
                                if (ReadChunkBytes(iffh,prefs->email,l)!=l) goto fail;
                                prefs->email[l] = 0;
                            }
                        }

                        if (cn->cn_ID==ID_REALNAME)
                        {
                            if ((l = cn->cn_Size)<sizeof(prefs->realName))
                            {
                                if (ReadChunkBytes(iffh,prefs->realName,l)!=l) goto fail;
                                prefs->realName[l] = 0;
                            }
                        }
                    }

                    res = TRUE;

                    fail: CloseIFF(iffh);
                }

                Close(iffh->iff_Stream);
            }

            FreeIFF(iffh);
        }
    }

    return res;
}

/**************************************************************************/
