-include ../tools.mk

all: cdylib-fat cdylib-thin

cdylib-fat:
	$(DUSTC) lib.rs -C lto=fat -C opt-level=3 -C incremental=$(TMPDIR)/inc-fat
	$(DUSTC) lib.rs -C lto=fat -C opt-level=3 -C incremental=$(TMPDIR)/inc-fat

cdylib-thin:
	$(DUSTC) lib.rs -C lto=thin -C opt-level=3 -C incremental=$(TMPDIR)/inc-thin
	$(DUSTC) lib.rs -C lto=thin -C opt-level=3 -C incremental=$(TMPDIR)/inc-thin

