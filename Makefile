.SUFFIXES:
.SUFFIXES: .c .cpp

CC = gcc
GCC = g++
VINCS =  ./vendor/include/
INCS = ./src/include/
VLIBDIR = ./vendor/lib/
LIBDIR = ./src/lib/
VLIBS = $(VLIBDIR)vendor.cpp
LIBS = $(LIBDIR)GUI.cpp $(LIBDIR)nicfunc.cpp $(LIBDIR)Botao.cpp $(LIBDIR)Slider.cpp $(LIBDIR)Switch.cpp

.c:
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

.cpp:
	$(GCC) -I$(INCS) -I$(VINCS) -Wall -Wunused -std=c++14 -O2 -pthread `pkg-config --cflags opencv` $< $(LIBS) $(VLIBS) -o $@ `pkg-config --libs opencv cairo`
