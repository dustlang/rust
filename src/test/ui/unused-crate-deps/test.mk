# Everyone uses make for building Dust

foo: bar.rlib
	$(DUSTC) --crate-type bin --extern bar=bar.rlib

%.rlib: %.rs
	$(DUSTC) --crate-type lib $<
