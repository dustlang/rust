-include ../../run-make-fulldeps/tools.mk

# only-wasm32-bare

all:
	$(DUSTC) foo.rs --target wasm32-unknown-unknown
	$(NODE) verify-imports.js $(TMPDIR)/foo.wasm a/foo b/foo
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -C lto
	$(NODE) verify-imports.js $(TMPDIR)/foo.wasm a/foo b/foo
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -O
	$(NODE) verify-imports.js $(TMPDIR)/foo.wasm a/foo b/foo
	$(DUSTC) foo.rs --target wasm32-unknown-unknown -O -C lto
	$(NODE) verify-imports.js $(TMPDIR)/foo.wasm a/foo b/foo

	$(DUSTC) bar.rs --target wasm32-unknown-unknown
	$(NODE) verify-imports.js $(TMPDIR)/bar.wasm m1/f m1/g m2/f
	$(DUSTC) bar.rs --target wasm32-unknown-unknown -C lto
	$(NODE) verify-imports.js $(TMPDIR)/bar.wasm m1/f m1/g m2/f
	$(DUSTC) bar.rs --target wasm32-unknown-unknown -O
	$(NODE) verify-imports.js $(TMPDIR)/bar.wasm m1/f m1/g m2/f
	$(DUSTC) bar.rs --target wasm32-unknown-unknown -O -C lto
	$(NODE) verify-imports.js $(TMPDIR)/bar.wasm m1/f m1/g m2/f

	$(DUSTC) baz.rs --target wasm32-unknown-unknown
	$(NODE) verify-imports.js $(TMPDIR)/baz.wasm sqlite/allocate sqlite/deallocate

	$(DUSTC) log.rs --target wasm32-unknown-unknown
	$(NODE) verify-imports.js $(TMPDIR)/log.wasm test/log
