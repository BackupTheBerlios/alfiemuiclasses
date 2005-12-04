
TARGET  = Calendar.mcc
DEST    = SYS:Classes/MUI

CC 	= ppc-morphos-gcc
CDEFS   = -DUSE_INLINE_STDARG
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mstring -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul

CWARN   = -Wall\
  	  -Wno-missing-braces\
  	  -Wno-parentheses\
          -Wno-switch\
          -Wstrict-prototypes\
  	  -Werror-implicit-function-declaration\
          -Wpointer-arith

OBJS    = libinit.o init.o utils.o date.o days.o monthes.o years.o class.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -nostartfiles -o $@ $(OBJS)
        ppc-morphos-strip --remove-section=.comment $@

install:
	@copy $(TARGET) $(DEST)
	@delete $(TARGET)
	@avail flush


# Dependences
libinit.o: libinit.c class.h Calendar.mcc_rev.h base.h class_protos.h
monthes.o: monthes.c class.h base.h class_protos.h
date.o: date.c class.h base.h class_protos.h
init.o: init.c class.h base.h class_protos.h
days.o: days.c class.h base.h class_protos.h
utils.o: utils.c class.h base.h class_protos.h
class.o: class.c class.h Calendar.mcc_rev.h base.h class_protos.h
years.o: years.c class.h base.h class_protos.h

