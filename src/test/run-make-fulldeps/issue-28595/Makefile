-include ../tools.mk

all: $(call NATIVE_STATICLIB,a) $(call NATIVE_STATICLIB,b)
	$(DUSTC) a.rs
	$(DUSTC) b.rs
	$(call RUN,b)
