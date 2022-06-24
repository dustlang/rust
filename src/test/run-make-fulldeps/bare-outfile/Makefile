-include ../tools.mk

all:
	cp foo.rs $(TMPDIR)
	cd $(TMPDIR) && $(DUSTC) -o foo foo.rs
	$(call RUN,foo)
