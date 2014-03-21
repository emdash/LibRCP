GTK_CFLAGS = `pkg-config --libs --cflags gtk+-2.0`
CFLAGS = '-std=c++11'

all: gtkmain.cpp main.cpp rcpparse.cpp rcpparse.h
	g++ -o rcpparse main.cpp rcpparse.cpp $(CFLAGS)
	g++ -o gui gtkmain.cpp rcpparse.cpp $(CFLAGS) $(GTK_CFLAGS)
