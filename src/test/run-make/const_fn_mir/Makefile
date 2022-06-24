-include ../../run-make-fulldeps/tools.mk

all:
	$(DUSTC) main.rs --emit=mir -o "$(TMPDIR)"/dump.mir

ifdef DUSTC_BLESS_TEST
	cp "$(TMPDIR)"/dump.mir dump.mir
else
	$(DIFF) dump.mir "$(TMPDIR)"/dump.mir
endif
