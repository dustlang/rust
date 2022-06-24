-include ../tools.mk

all:
	$(DUSTC) -C extra-filename=bar foo.rs -C save-temps
	rm $(TMPDIR)/foobar.foo*0.rcgu.o
	rm $(TMPDIR)/$(call BIN,foobar)
