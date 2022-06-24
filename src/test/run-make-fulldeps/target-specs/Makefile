-include ../tools.mk
all:
	$(DUSTC) foo.rs --target=my-awesome-platform.json --crate-type=lib --emit=asm
	$(CGREP) -v morestack < $(TMPDIR)/foo.s
	$(DUSTC) foo.rs --target=my-invalid-platform.json 2>&1 | $(CGREP) "Error loading target specification"
	$(DUSTC) foo.rs --target=my-incomplete-platform.json 2>&1 | $(CGREP) 'Field llvm-target'
	DUST_TARGET_PATH=. $(DUSTC) foo.rs --target=my-awesome-platform --crate-type=lib --emit=asm
	DUST_TARGET_PATH=. $(DUSTC) foo.rs --target=my-x86_64-unknown-linux-gnu-platform --crate-type=lib --emit=asm
	$(DUSTC) -Z unstable-options --target=my-awesome-platform.json --print target-spec-json > $(TMPDIR)/test-platform.json && $(DUSTC) -Z unstable-options --target=$(TMPDIR)/test-platform.json --print target-spec-json | diff -q $(TMPDIR)/test-platform.json -
