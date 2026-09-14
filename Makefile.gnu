CC=gcc
CFLAGS=-O3 -ggdb -Wall -Iinclude $$(pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LIBS=$$(pkg-config --libs sdl2 SDL2_image SDL2_ttf) -lm

all: spl karel

spl: lib/libspl.o spl.o
	$(CC) -o $@ $^ $(LIBS)

karel: lib/libspl.o lib/libkarel.o karel.o
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f *.o lib/*.o spl karel
