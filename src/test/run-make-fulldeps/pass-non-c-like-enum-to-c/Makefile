-include ../tools.mk

all: $(call NATIVE_STATICLIB,test)
	$(DUSTC) nonclike.rs -L$(TMPDIR) -ltest
	$(call RUN,nonclike)
