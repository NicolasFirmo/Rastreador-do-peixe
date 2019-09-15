.SUFFIXES:
.SUFFIXES: .c .cpp

CC = gcc
GCC = g++

.c:
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

.cpp:
	$(GCC) -Wall -Wunused -std=c++14 -O2 -pthread `pkg-config --cflags opencv` $< gui.cpp nicfunc.cpp -o $@ `pkg-config --libs opencv cairo`
