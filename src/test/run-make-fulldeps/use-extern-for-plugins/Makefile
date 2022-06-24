-include ../tools.mk

# ignore-freebsd
# ignore-openbsd
# ignore-sunos

HOST := $(shell $(DUSTC) -vV | grep 'host:' | sed 's/host: //')
ifeq ($(findstring i686,$(HOST)),i686)
TARGET := $(subst i686,x86_64,$(HOST))
else
TARGET := $(subst x86_64,i686,$(HOST))
endif

all:
	$(DUSTC) foo.rs -C extra-filename=-host
	$(DUSTC) bar.rs -C extra-filename=-targ --target $(TARGET)
	$(DUSTC) baz.rs --extern a=$(TMPDIR)/liba-targ.rlib --target $(TARGET)
