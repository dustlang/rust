-include ../tools.mk

all:
	$(DUSTC) checkdust.rs
	$(CC) test.c $(call STATICLIB,checkdust) $(call OUT_EXE,test) $(EXTRACFLAGS)
	$(call RUN,test)
