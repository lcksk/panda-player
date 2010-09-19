CC=gcc -std=gnu99
BIN=panda
PACKAGES=gtk+-2.0 gstreamer-0.10 glib-2.0 gstreamer-interfaces-0.10 gdk-2.0 sqlite
CFLAGS=`pkg-config --cflags $(PACKAGES)` -Wall -g
LIBS=`pkg-config --libs $(PACKAGES)` -Wall
OBJ=player.o playlist.o callbacks.o playlist_sqlite.o


all: $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $(LIBS) $(OBJ)
	
player.o: player.c player.h
	$(CC) -c $(CFLAGS) player.c

playlist.o: playlist.c playlist.h
	$(CC) -c $(CFLAGS) playlist.c
	
callbacks.o: callbacks.c callbacks.h
	$(CC) -c $(CFLAGS) callbacks.c
	
playlist_sqlite.o: playlist_sqlite.c playlist_sqlite.h
	$(CC) -c $(CFLAGS) playlist_sqlite.c
	
clean:
	@rm -vf $(OBJ) $(BIN)
	


