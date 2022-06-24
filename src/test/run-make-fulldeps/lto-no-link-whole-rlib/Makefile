-include ../tools.mk

all: $(call NATIVE_STATICLIB,foo) $(call NATIVE_STATICLIB,bar)
	$(DUSTC) lib1.rs
	$(DUSTC) lib2.rs
	$(DUSTC) main.rs -Clto
	$(call RUN,main)

