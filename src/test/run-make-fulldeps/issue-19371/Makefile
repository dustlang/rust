-include ../tools.mk

# This test ensures that dustc compile_input can be called twice in one task
# without causing a panic.
# The program needs the path to dustc to get sysroot.

all:
	$(DUSTC) foo.rs
	$(call RUN,foo $(TMPDIR) $(DUSTC))
