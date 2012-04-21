TARGET = ZMagic

CMDOBJS = cmd_key.o cmd_sleep.o cmd_click.o cmd_getmouselocation.o \
          cmd_help.o cmd_mousedown.o cmd_mousemove.o cmd_mousemove_relative.o \
          cmd_mouseup.o cmd_type.o cmd_version.o
          
LIBS = -lX11 -lXtst

ZMagic : Z_do.o $(CMDOBJS) ZMagic.o
	cc -o $(TARGET) $(CMDOBJS) Z_do.o ZMagic.o $(LIBS)
	
Z_do.o : Z_do.c
	cc -c Z_do.c

ZMagic.o : ZMagic.c
	cc -c ZMagic.c
	
clean :
	-rm -f *.o $(TARGET)
