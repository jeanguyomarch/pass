FLAGS := -Wall -Wextra -Wshadow -O2
PKG_LIBS := eina eet ecore ecore-file
OBJS := main.o file.o clipboard.o tty.o clipboard_x.o

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
   FLAGS += -DHAVE_OSX
   PKG_LDFLAGS += -lobjc -framework Foundation -framework AppKit -fobjc-arc
   OBJS += clipboard_cocoa.o
endif
ifeq ($(UNAME), Linux)
   FLAGS += -DHAVE_LINUX
   PKG_LIBS += elementary
   OBJS += clipboard_elm.o
endif

PKG_CFLAGS := $(shell pkg-config --cflags $(PKG_LIBS))
PKG_LDFLAGS := $(shell pkg-config --libs $(PKG_LIBS))

BIN := pass

PREFIX := /usr/local

V := 0
AT_0 := @
AT_1 :=
AT = $(AT_$(V))

.PHONY: all clean install

all: $(BIN) Makefile pass.h

%.o: %.m
	@echo "  OBJC    $<"
	$(AT)$(CC) $(FLAGS) -c -o $@ $^ $(PKG_CFLAGS)

%.o: %.c
	@echo "  CC      $<"
	$(AT)$(CC) $(FLAGS) -c -o $@ $^ $(PKG_CFLAGS)

$(BIN): $(OBJS)
	@echo "  LD      $@"
	$(AT)$(CC) $(FLAGS) -o $@ $^ $(PKG_LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(BIN)

install:
	install -m 0755 $(BIN) "$(PREFIX)/bin"
	install -g 0 -o 0 -m 0644 $(BIN).1 "$(PREFIX)/share/man/man1"

