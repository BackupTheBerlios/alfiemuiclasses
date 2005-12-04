VERSION EQU 18
REVISION EQU 3
DATE MACRO
    dc.b '27.7.2005'
    ENDM
VERS MACRO
    dc.b 'Calendar.mcp 18.3'
    ENDM
VSTRING MACRO
    dc.b '$VER: Calendar.mcp 18.3 (27.7.2005) 2004-2005 Alfonso Ranieri <alforan@tin.it>',13,10,0
    ENDM
VERSTAG MACRO
    dc.b 0,'$VER: Calendar.mcp 18.3 (27.7.2005)',0
    ENDM
