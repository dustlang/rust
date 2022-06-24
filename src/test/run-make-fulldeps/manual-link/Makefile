-include ../tools.mk

all: $(TMPDIR)/libbar.a
	$(DUSTC) foo.rs -lstatic=bar
	$(DUSTC) main.rs
	$(call RUN,main)
