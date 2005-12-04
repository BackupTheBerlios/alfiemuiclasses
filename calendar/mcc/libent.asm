;
;
; Calendar - Calendar MUI custom class
;
; Copyright 2004-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>
; All Rights Are Reserved.
;
; Destributed Under The Terms Of The LGPL II
;
;

        SECTION TheBar,CODE

        NOLIST

        INCLUDE "Calendar.mcc_rev.i"
        INCLUDE "exec/libraries.i"
        INCLUDE "exec/resident.i"
        INCLUDE "exec/initializers.i"
        INCLUDE "exec/lists.i"
        INCLUDE "exec/semaphores.i"

        LIST

        XREF    _LinkerDB
        XREF    _lib_name
        XREF    ENDCODE

        XREF    _initLib
        XREF    _openLib
        XREF    _expungeLib
        XREF    _closeLib
        XREF    _query

        XREF    _CALDateToGreg
        XREF    _CALGregToDate
        XREF    _CALIsLeap
        XREF    _CALGetMonthDays
        XREF    _CALValidateDate
        XREF    _CALGetToday
        XREF    _CALGregWeekday
        XREF    _CALDateWeekday
        XREF    _CALGetDay
        XREF    _CALGetMonth
        XREF    _CALGetYear
        XREF    _CALGetEaster
        XREF    _CALGetJulian
        XREF    _CALFormatDate

        XDEF    _ID

PRI     EQU 0

start:  moveq   #-1,d0
        rts

romtag:
        dc.w    RTC_MATCHWORD
        dc.l    romtag
        dc.l    ENDCODE
        dc.b    RTF_AUTOINIT
        dc.b    VERSION
        dc.b    NT_LIBRARY
        dc.b    PRI
        dc.l    _lib_name
        dc.l    _ID
        dc.l    init

_ID:    VSTRING

        CNOP    0,4

init:   dc.l    LIB_SIZE
        dc.l    funcTable
        dc.l    dataTable
        dc.l    _initLib

V_DEF   MACRO
    dc.w    \1+(*-funcTable)
    ENDM

funcTable:
        DC.W    -1

        V_DEF   _openLib
        V_DEF   _closeLib
        V_DEF   _expungeLib
        V_DEF   nil
        V_DEF   _query

        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil
        V_DEF   nil

        V_DEF   _CALDateToGreg
        V_DEF   _CALGregToDate
        V_DEF   _CALIsLeap
        V_DEF   _CALGetMonthDays
        V_DEF   _CALValidateDate
        V_DEF   _CALGetToday
        V_DEF   _CALGregWeekday
        V_DEF   _CALDateWeekday
        V_DEF   _CALGetDay
        V_DEF   _CALGetMonth
        V_DEF   _CALGetYear
        V_DEF   _CALGetEaster
        V_DEF   _CALGetJulian
        V_DEF   _CALFormatDate

        DC.W    -1

dataTable:
        INITBYTE LN_TYPE,NT_LIBRARY
        INITLONG LN_NAME,_lib_name
        INITBYTE LIB_FLAGS,(LIBF_SUMUSED!LIBF_CHANGED)
        INITWORD LIB_VERSION,VERSION
        INITWORD LIB_REVISION,REVISION
        INITLONG LIB_IDSTRING,_ID
        dc.w     0

        CNOP    0,4

nil:    moveq   #0,d0
        rts

        END
