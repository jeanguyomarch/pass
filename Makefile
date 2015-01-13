FLAGS = -Wall -O2
PKG_LIBS = eina eet ecore ecore-file
PKG_CFLAGS = $(shell pkg-config --cflags $(PKG_LIBS))
PKG_LDFLAGS = $(shell pkg-config --libs $(PKG_LIBS))

OBJS = main.o file.o clipboard.o tty.o
BIN = pass

PREFIX = /usr/local

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
   FLAGS += -DHAVE_OSX
   PKG_LDFLAGS += -lobjc -framework Foundation -framework AppKit -fobjc-arc
   OBJS += clipboard_cocoa.o
endif

%.o: %.m
ifeq ($(UNAME), Darwin)
	$(CC) $(FLAGS) -c -o $@ $^ $(PKG_CFLAGS)
endif

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $^ $(PKG_CFLAGS)

$(BIN): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^ $(PKG_LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(BIN)

install:
	install -m 0755 $(BIN) "$(PREFIX)/bin"
	install -g 0 -o 0 -m 0644 $(BIN).1 "$(PREFIX)/share/man/man1"

