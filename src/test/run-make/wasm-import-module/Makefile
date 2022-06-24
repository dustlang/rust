-include ../../run-make-fulldeps/tools.mk

 # only-wasm32-bare

all:
	$(DUSTC) foo.rs --target wasm32-unknown-unknown
	$(DUSTC) bar.rs -C lto -O --target wasm32-unknown-unknown
	$(NODE) foo.js $(TMPDIR)/bar.wasm
