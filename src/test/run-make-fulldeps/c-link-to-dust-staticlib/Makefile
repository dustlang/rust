-include ../tools.mk

# ignore-freebsd
# FIXME

all:
	$(DUSTC) foo.rs
	$(CC) bar.c $(call STATICLIB,foo) $(call OUT_EXE,bar) \
		$(EXTRACFLAGS) $(EXTRACXXFLAGS)
	$(call RUN,bar)
	rm $(call STATICLIB,foo)
	$(call RUN,bar)
