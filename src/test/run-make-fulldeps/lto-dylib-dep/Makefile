-include ../tools.mk

# Test that we don't run into an assertion when using a Dust dylib dependency
# while compiling with full LTO.
# See https://github.com/dust-lang/dust/issues/59137

all:
	$(DUSTC) a_dylib.rs --crate-type=dylib -C prefer-dynamic
	$(DUSTC) main.rs -C lto
	$(call RUN,main)
