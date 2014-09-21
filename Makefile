FLAGS = -Wall -O0 -g
PKG_LIBS = eina eet ecore ecore-file
PKG_CFLAGS = $(shell pkg-config --cflags $(PKG_LIBS))
PKG_LDFLAGS = $(shell pkg-config --libs $(PKG_LIBS))

OBJS = main.o file.o clipboard.o tty.o
BIN = pass

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
	rm -f $(OBJS)
	rm -f $(BIN)

