#ifndef _CLASS_PROTOS_H
#define _CLASS_PROTOS_H

#ifdef __MORPHOS__
#undef MUI_NewObject
Object *MUI_NewObject(char *classname,Tag tag1,...);
#undef NewObject
APTR NewObject( struct IClass *classPtr,UBYTE *classID, ULONG tag1, ... );
#undef CreateNewProcTags
struct Process *CreateNewProcTags( ULONG tag1type, ... );
#endif

/* init.c */
#ifdef __MORPHOS__
ULONG query ( void );
#else
ULONG SAVEDS ASM query ( REG (d0,LONG which )) ;
#endif
void freeBase ( void );
ULONG initBase ( void );

/* loc.c */
void initStrings ( void );
UBYTE *getString ( ULONG id );
void localizeNewMenu ( struct NewMenu *menu , ULONG *ids );

/* utils.c */
Object *olabel ( ULONG text );
Object *ollabel ( ULONG text );
Object *olabel1 ( ULONG text );
Object *ollabel1 ( ULONG text );
Object *olabel2 ( ULONG text );
Object *ostring ( ULONG label , ULONG help , ULONG weight , ULONG max );
Object *otext ( ULONG help );
Object *obutton ( ULONG label , ULONG help );
ULONG xget ( Object *obj , Tag attr );
ULONG getKeyChar ( UBYTE *string );
APTR allocVecPooled ( APTR pool , ULONG size );
void freeVecPooled ( APTR pool , APTR mem );
APTR allocArbitratePooled ( ULONG size );
void freeArbitratePooled ( APTR mem , ULONG size );
APTR allocArbitrateVecPooled ( ULONG size );
void freeArbitrateVecPooled ( APTR mem );
#ifdef __MORPHOS__
void sprintf ( UBYTE *buf , UBYTE *fmt , ...);
int snprintf ( UBYTE *buf , int size , UBYTE *fmt , ...) __attribute((varargs68k));
#else
ULONG STDARGS DoSuperNew ( struct IClass *cl , Object *obj , ULONG tag1 , ...);
void STDARGS sprintf ( UBYTE *to , UBYTE *fmt , ...);
int STDARGS snprintf ( UBYTE *buf , int size , UBYTE *fmt , ...);
#endif

/* options.c */
ULONG savePrefs ( UBYTE *file , struct prefs *prefs );
ULONG loadPrefs ( UBYTE *file , struct prefs *prefs );

/* proc.c */
#ifdef __MORPHOS__
void task ( void );
#else
void SAVEDS task ( void );
#endif

/* base64.c */
ULONG encodeB64 ( APTR data , ULONG (*func )(APTR ,STRPTR,int ), STRPTR source );

/* rootstring.c */
ULONG initRootString ( void );
void freeRootString ( void );

/* rootEditor.c */
ULONG initRootEditor ( void );
void freeRootEditor ( void );

/* attach.c */
ULONG initAttach ( void );
void freeAttach ( void );

/* class.c */
ULONG initMCC ( void );
void freeMCC ( void );

#endif /* _CLASS_PROTOS_H */

