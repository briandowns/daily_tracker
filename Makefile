VERSION          := 0.1.0
BINDIR           := bin
BINARY           := daily_tracker
override LDFLAGS +=
override CFLAGS  += -static -Dbin_name=$(BINARY) -Ddaily_tracker__version=$(VERSION) -Dgit_sha=$(shell git rev-parse HEAD) -O3

PREFIX = /usr/local

UNAME_S = $(shell uname -s)

MACOS_MANPAGE_LOC = /usr/share/man
LINUX_MAPPAGE_LOC = $(PREFIX)/man/man8

$(BINDIR)/$(BINARY): $(BINDIR) clean
ifeq ($(UNAME_S),FreeBSD)
	$(CC) main.c $(CFLAGS) -o $@ $(LDFLAGS) -I$(PREFIX)/include -L$(PREFIX)/lib
else
	$(CC) main.c $(CFLAGS) -o $@ $(LDFLAGS)
endif

$(BINDIR):
	mkdir -p $@

$(DEPDIR):
	mkdir -p $@

.PHONY: install
install: $(BINDIR)/$(BINARY)
	mkdir -p $(PREFIX)/bin
	cp $(BINDIR)/$(BINARY) $(PREFIX)/bin

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/$(BINDIR)/$(BINARY)

.PHONY: deps
deps: $(DEPDIR)

.PHONY: clean
clean:
	rm -f $(BINDIR)/*
