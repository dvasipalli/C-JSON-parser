CC=gcc

# -fPIC   = Produce "Position Independent Code", suitable for shared objects
# -shared = Produce a shared object which can be linked with other objects.
# -fvisibility=hidden = By default, symbols (i.e internal functions, global
#    vars, etc.) are hidden and will be removed by the strip command to minimize
#    name-space pollution.

CFLAGS=-fPIC -shared -fvisibility=hidden -ggdb
OBJS=json.o lex.o parse.o print.o

all: libjson

libjson: $(OBJS)
	$(CC) $(CFLAGS) -o libjson.so $(OBJS); \
	strip --strip-all --discard-all libjson.so

test: test.c json.h
	$(CC) -o test test.c -L . -l json

lex.o: lex.c internal.h json.h
	$(CC) $(CFLAGS) -c lex.c

json.o: json.c internal.h json.h
	$(CC) $(CFLAGS) -c json.c

parse.o: parse.c internal.h json.h
	$(CC) $(CFLAGS) -c parse.c

print.o: print.c internal.h json.h
	$(CC) $(CFLAGS) -c print.c

objclean:
	rm -f .*~ *~ *.o

clean:	objclean
	rm -f libjson.so test
