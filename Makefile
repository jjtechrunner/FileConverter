PROG = FFFC
CC = g++
CPPFLAGS = -g -I. -I./include -I/remote/users/jjose/pwe/tech/boost/boost_1_65_0
LDFLAGS = -L /remote/users/jjose/pwe/tech/boost/boost_1_65_0/libs
OBJS = FFFileConverter.o  FldAbstract.o FileConverter.o
$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS)
fld.o :
	$(CC) $(CPPFLAGS) -c FldAbstract.cpp 
main.o :
	$(CC) $(CPPFLAGS) -c FFFileConverter.cpp FileConverter.cpp
clean:
	rm -f core $(PROG) $(OBJS)
