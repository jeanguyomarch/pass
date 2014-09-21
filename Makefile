FLAGS = -Wall -O0 -g
PKG_LIBS = eina eet ecore ecore-file
PKG_CLFAGS = `pkg-config --cflags $(PKG_LIBS)`
PKG_LDFLAGS = `pkg-config --libs $(PKG_LIBS)`

OBJS = main.o file.o clipboard.o tty.o
BIN = pass

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $^ $(PKG_CLFAGS)

$(BIN): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^ $(PKG_LDFLAGS)

clean:
	rm -f $(OBJS)
	rm -f $(BIN)

