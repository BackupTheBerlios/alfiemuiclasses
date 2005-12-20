VERSION EQU 20
REVISION EQU 1
DATE MACRO
    dc.b '28.7.2005'
    ENDM
VERS MACRO
    dc.b 'MiniMailer.mcc 20.1'
    ENDM
VSTRING MACRO
    dc.b '$VER: MiniMailer.mcc 20.1 (28.7.2005) 2004-2005 Alfonso Ranieri <alforan@tin.it>',13,10,0
    ENDM
VERSTAG MACRO
    dc.b 0,'$VER: MiniMailer.mcc 20.1 (28.7.2005)',0
    ENDM
