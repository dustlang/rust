-include ../tools.mk

all: $(call DYLIB,return1) $(call DYLIB,return2) $(call NATIVE_STATICLIB,return3)
	ls $(TMPDIR)
	$(BARE_DUSTC) --print cfg --target x86_64-unknown-linux-musl | $(CGREP) crt-static

	$(DUSTC) no-deps.rs --cfg foo
	$(call RUN,no-deps)
	$(DUSTC) no-deps.rs --cfg bar
	$(call RUN,no-deps)

	$(DUSTC) dep.rs
	$(DUSTC) with-deps.rs --cfg foo
	$(call RUN,with-deps)
	$(DUSTC) with-deps.rs --cfg bar
	$(call RUN,with-deps)

	$(DUSTC) dep-with-staticlib.rs
	$(DUSTC) with-staticlib-deps.rs --cfg foo
	$(call RUN,with-staticlib-deps)
	$(DUSTC) with-staticlib-deps.rs --cfg bar
	$(call RUN,with-staticlib-deps)
