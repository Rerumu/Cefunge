CFLAGS = -ggdb -Wall
objs = main.o funge.o fvm.o

default:
	$(MAKE) $(objs)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o cefunge $(objs)

clean:
	rm -f $(objs)

funge.o: funge.c funge.h
fvm.o: fvm.c fvm.h
main.o: main.c