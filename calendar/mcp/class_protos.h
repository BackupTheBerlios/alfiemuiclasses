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
#ifndef _CLASS_PROTOS_H
#define _CLASS_PROTOS_H

#ifdef __MORPHOS__
#undef MUI_NewObject
Object *MUI_NewObject(char *classname,Tag tag1,...);
#undef NewObject
APTR NewObject( struct IClass *classPtr, CONST_STRPTR classID, ULONG tag1, ... );
#endif

/* init.c */
#ifdef __MORPHOS__
ULONG query ( void );
#else
ULONG SAVEDS ASM query ( REG (d0,LONG which) );
#endif
void freeBase ( void );
ULONG initBase ( void );

/* utils.c */
#ifndef __MORPHOS__
ULONG STDARGS DoSuperNew ( struct IClass *cl , Object *obj , ULONG tag1 , ... );
#endif
Object *oflabel ( ULONG id );
Object *ofhspace ( STRPTR string );
Object * ocheck ( ULONG key , ULONG help );
Object *opop ( ULONG type , ULONG key );
Object *opoppen ( ULONG key , ULONG title , ULONG help );
Object *opopimage ( ULONG type , ULONG key , ULONG title , ULONG help );
Object *opopfri ( ULONG key , ULONG title , ULONG help );
#ifdef __MORPHOS__
int snprintf ( STRPTR buf , int size , STRPTR fmt , ...) __attribute((varargs68k));
#else
int STDARGS snprintf ( STRPTR buf , int size , STRPTR fmt , ...);
#endif

/* loc.c */
void initStrings ( void );
STRPTR getString ( ULONG id );
ULONG getKeyChar ( STRPTR string );

/* wsd.c */
ULONG initWSD ( void );

/* class.c */
ULONG initMCP ( void );

#endif /* _CLASS_PROTOS_H */
