-include ../../run-make-fulldeps/tools.mk

# FIXME(eddyb) provide `HOST_DUSTC` and `TARGET_DUSTC`
# instead of hardcoding them everywhere they're needed.
ifeq ($(IS_MUSL_HOST),1)
ADDITIONAL_ARGS := $(DUSTFLAGS)
endif
all:
	$(BARE_DUSTC) $(ADDITIONAL_ARGS) foo.rs --out-dir $(TMPDIR)
	$(DUSTC) bar.rs --target $(TARGET) --emit dep-info
	$(CGREP) -v "proc-macro source" < $(TMPDIR)/bar.d
