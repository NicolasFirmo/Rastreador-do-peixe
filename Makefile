.SUFFIXES:
.SUFFIXES: .c .cpp

CC = gcc
GCC = g++
INCS = ./src/inc/
LIBDIR = ./src/lib/
LIBS = $(LIBDIR)GUI.cpp $(LIBDIR)nicfunc.cpp $(LIBDIR)Botao.cpp $(LIBDIR)Slider.cpp $(LIBDIR)Switch.cpp

.c:
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

.cpp:
	$(GCC) -I$(INCS) -Wall -Wunused -std=c++14 -O2 -pthread `pkg-config --cflags opencv` $< $(LIBS) -o $@ `pkg-config --libs opencv cairo`
