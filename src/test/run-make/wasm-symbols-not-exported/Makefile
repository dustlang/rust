-include ../../run-make-fulldeps/tools.mk

# only-wasm32-bare

all:
	$(DUSTC) foo.rs --target wasm32-unknown-unknown
	$(NODE) verify-exported-symbols.js $(TMPDIR)/foo.wasm
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -O
	$(NODE) verify-exported-symbols.js $(TMPDIR)/foo.wasm
	$(DUSTC) bar.rs --target wasm32-unknown-unknown
	$(NODE) verify-exported-symbols.js $(TMPDIR)/bar.wasm
	$(DUSTC) bar.rs --target wasm32-unknown-unknown -O
	$(NODE) verify-exported-symbols.js $(TMPDIR)/bar.wasm
