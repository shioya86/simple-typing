all: a.out
CPPFLAGS=-std=c++11
LDFLAGS=-lncurses

a.out: main.cpp
	g++ $(CPPFLAGS) $< $(LDFLAGS)

clean:
	rm -r a.out
