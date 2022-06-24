-include ../tools.mk
all:
	$(DUSTC) bar.rs --crate-type=rlib
	$(DUSTC) foo.rs
	$(call RUN,foo)
