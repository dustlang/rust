-include ../tools.mk

all:
	$(DUSTC) foo.rs -g
	cp foo.bat $(TMPDIR)/
	OUT_DIR="$(TMPDIR)" DUSTC="$(DUSTC_ORIGINAL)" $(call RUN,foo)
