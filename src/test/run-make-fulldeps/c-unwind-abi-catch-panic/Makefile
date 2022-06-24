-include ../tools.mk

all: $(call NATIVE_STATICLIB,add)
	$(DUSTC) main.rs
	$(call RUN,main) || exit 1
