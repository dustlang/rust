-include ../tools.mk

all:
	# Compile a `#![no_builtins]` rlib crate
	$(DUSTC) no_builtins.rs
	# Build an executable that depends on that crate using LTO. The no_builtins crate doesn't
	# participate in LTO, so its rlib must be explicitly linked into the final binary. Verify this by
	# grepping the linker arguments.
	$(DUSTC) main.rs -C lto -Z print-link-args | $(CGREP) 'libno_builtins.rlib'
