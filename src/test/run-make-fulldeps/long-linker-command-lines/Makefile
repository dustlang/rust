-include ../tools.mk

all:
	$(DUSTC) foo.rs -g -O
	DUSTC="$(DUSTC_ORIGINAL)" $(call RUN,foo)
