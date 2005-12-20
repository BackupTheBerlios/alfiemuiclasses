
TARGET  = MiniMailer.mcc
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

OBJS    = libinit.o init.o loc.o utils.o options.o base64.o proc.o attach.o class.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -nostartfiles -o $@ $(OBJS)
	ppc-morphos-strip $@

install:
	@copy $(TARGET) $(DEST)
	@delete $(TARGET)
	@avail flush


libinit.o:    libinit.c class.h MiniMailer.mcc_rev.h base.h class_protos.h
init.o:       init.c class.h base.h class_protos.h
loc.o:        loc.c class.h loc.h base.h class_protos.h
utils.o:      utils.c class.h base.h class_protos.h
options.o:    options.c class.h base.h class_protos.h
proc.o:       proc.c class.h http.h MiniMailer.mcc_rev.h loc.h base.h class_protos.h
base64.o:     base64.c class.h base.h class_protos.h
attach.o:     attach.c class.h loc.h base.h class_protos.h
class.o:      class.c class.h http.h loc.h base.h class_protos.h
