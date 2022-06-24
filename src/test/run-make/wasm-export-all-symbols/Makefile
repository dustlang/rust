-include ../../run-make-fulldeps/tools.mk

# only-wasm32-bare

all:
	$(DUSTC) bar.rs --target wasm32-unknown-unknown
	$(DUSTC) foo.rs --target wasm32-unknown-unknown
	$(NODE) verify.js $(TMPDIR)/foo.wasm
	$(DUSTC) main.rs --target wasm32-unknown-unknown
	$(NODE) verify.js $(TMPDIR)/main.wasm
	$(DUSTC) bar.rs --target wasm32-unknown-unknown -O
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -O
	$(NODE) verify.js $(TMPDIR)/foo.wasm
	$(DUSTC) main.rs --target wasm32-unknown-unknown -O
	$(NODE) verify.js $(TMPDIR)/main.wasm
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -C lto
	$(NODE) verify.js $(TMPDIR)/foo.wasm
	$(DUSTC) main.rs --target wasm32-unknown-unknown -C lto
	$(NODE) verify.js $(TMPDIR)/main.wasm
