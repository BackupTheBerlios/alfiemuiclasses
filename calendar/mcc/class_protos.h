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

/* date.c */
#ifdef __MORPHOS__
ULONG CALGregToDate ( ULONG greg , ULONG *day , ULONG *month , ULONG *year );
ULONG LIB_CALGregToDate ( void );
ULONG CALDateToGreg ( ULONG day , ULONG month , ULONG year );
ULONG LIB_CALDateToGreg ( void );
ULONG CALIsLeap ( ULONG year );
ULONG LIB_CALIsLeap ( void );
ULONG CALGetDay ( ULONG greg );
ULONG LIB_CALGetDay ( void );
ULONG CALGetMonth ( ULONG greg );
ULONG LIB_CALGetMonth ( void );
ULONG CALGetYear ( ULONG greg );
ULONG LIB_CALGetYear ( void );
ULONG CALGetMonthDays ( ULONG month , ULONG year );
ULONG LIB_CALGetMonthDays ( void );
ULONG CALGetToday ( void );
ULONG LIB_CALGetToday ( void );
ULONG CALDateWeekday ( ULONG day , ULONG month , ULONG year );
ULONG LIB_CALDateWeekday ( void );
ULONG CALGregWeekday ( ULONG greg );
ULONG LIB_CALGregWeekday ( void );
ULONG CALGetJulian ( ULONG greg );
ULONG LIB_CALGetJulian ( void );
void CALGetEaster ( ULONG year , ULONG *d , ULONG *m );
void LIB_CALGetEaster ( void );
void CALValidateDate ( ULONG *day , ULONG *month , ULONG *year );
void LIB_CALValidateDate ( void );
STRPTR CALFormatDate ( struct Locale *locale , STRPTR buf , int size , STRPTR fmt , ULONG greg );
STRPTR LIB_CALFormatDate ( void );
#else
ULONG ASM SAVEDS CALGregToDate ( REG (d0,ULONG greg) , REG (a0,ULONG *day) , REG (a1,ULONG *month) , REG (a2,ULONG *year ));
ULONG ASM SAVEDS CALDateToGreg ( REG (d0,ULONG day) , REG (d1,ULONG month) , REG (d2,ULONG year ));
ULONG ASM SAVEDS CALIsLeap ( REG (d0,ULONG year) );
ULONG ASM SAVEDS CALGetDay ( REG (d0,ULONG greg) );
ULONG ASM SAVEDS CALGetMonth ( REG (d0,ULONG greg) );
ULONG ASM SAVEDS CALGetYear ( REG (d0,ULONG greg) );
ULONG ASM SAVEDS CALGetMonthDays ( REG (d0,ULONG month) , REG (d1,ULONG year ));
ULONG ASM SAVEDS CALGetToday ( void );
ULONG ASM SAVEDS CALDateWeekday ( REG (d0,ULONG day) , REG (d1,ULONG month) , REG (d2,ULONG year ));
ULONG ASM SAVEDS CALGregWeekday ( REG (d0,ULONG greg ));
ULONG ASM SAVEDS CALGetJulian ( REG (d0,ULONG greg ));
void ASM SAVEDS CALGetEaster ( REG (d0,ULONG year) , REG (a0,ULONG *d) , REG (a1,ULONG *m ));
void ASM SAVEDS CALValidateDate ( REG (a0,ULONG *day) , REG (a1,ULONG *month) , REG (a2,ULONG *year ));
STRPTR ASM SAVEDS CALFormatDate ( REG (a0,struct Locale *locale) , REG (a1,STRPTR buf) , REG (d0,int size) , REG (a2,STRPTR fmt) , REG (d1,ULONG greg ));
#endif

/* utils.c */
#ifdef __MORPHOS__
void sprintf ( STRPTR buf , STRPTR fmt , ...) __attribute((varargs68k));
#else
ULONG STDARGS DoSuperNew ( struct IClass *cl , Object *obj , ULONG tag1 , ...);
void STDARGS sprintf ( STRPTR to , STRPTR fmt , ...);
#endif
LONG xget ( Object *obj , ULONG attribute );
Object *oibutton ( STRPTR spec );
struct TextFont *openFont ( STRPTR name );
STRPTR *makeMonthesArray ( ULONG ab );
STRPTR *makeDaysArray ( ULONG ab );
void readHolidays ( void );
APTR allocVecPooled ( APTR pool , ULONG size );
void freeVecPooled ( APTR pool , APTR mem );

/* days.c */
ULONG initDaysClass ( void );

/* monthes.c */
ULONG initMonthesClass ( void );

/* years.c */
ULONG initYearsClass ( void );

/* class.c */
ULONG initMCC ( void );

#endif /* _CLASS_PROTOS_H */
