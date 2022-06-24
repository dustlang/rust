-include ../tools.mk
all:
	$(DUSTC) bar.rs --crate-type=dylib -C prefer-dynamic
	$(DUSTC) foo.rs
	$(call RUN,foo)
