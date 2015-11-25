## Cygwin/GNU C++
CC = g++
CFLAGS = -Wall -O
LFLAGS = -lwsock32 -o $@
OBJEXT = o


.SUFFIXES: .cpp .$(OBJEXT)


## Build rule for Cygwin/GNU C++
.cpp.$(OBJEXT):
	$(CC) -c $(CFLAGS) $< -o $*.o


#### Major targets #####################################################

all: basic-client.exe basic-server.exe

ctags:
	ctags *.cpp *.h

clean:
	rm -f *.$(OBJEXT) *.bak

distclean: clean
	rm -f *.exe *.swp


#### Common dependencies ###############################################

read_pe.$(OBJEXT): read_pe.cpp protocol.h
main.$(OBJEXT): main.cpp protocol.h
ws-util.$(OBJEXT): ws-util.cpp ws-util.h 


#### Basic Winsock client ##############################################

BASIC_CLIENT_OBJS = basic-client.$(OBJEXT) main.$(OBJEXT) \
		ws-util.$(OBJEXT) read_pe.$(OBJEXT)
basic-client.exe: $(BASIC_CLIENT_OBJS)
	$(CC) $(BASIC_CLIENT_OBJS) $(LFLAGS)

basic-client.$(OBJEXT): basic-client.cpp ws-util.h protocol.h


#### Basic Winsock server ##############################################

BASIC_SERVER_OBJS = basic-server.$(OBJEXT) main.$(OBJEXT) \
		ws-util.$(OBJEXT) read_pe.$(OBJEXT)
basic-server.exe: $(BASIC_SERVER_OBJS)
	$(CC) $(BASIC_SERVER_OBJS) $(LFLAGS)

basic-server.$(OBJEXT): basic-server.cpp ws-util.h protocol.h




