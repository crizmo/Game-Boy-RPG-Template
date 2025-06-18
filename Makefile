CC=$(HOME)/Documents/GitHub/gbdk/bin/lcc
CFLAGS=-Wa-l -Wl-m -Wl-j -Wf--opt-code-size

all: gbrpg.gb

gbrpg.gb: main.c gfx/tiles.c gfx/player.c maps/worldmap.c
	mkdir -p gfx maps
	$(CC) $(CFLAGS) -o gbrpg.gb main.c