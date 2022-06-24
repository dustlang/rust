-include ../tools.mk

all:
	$(DUSTC) panic.rs
	$(DUSTC) -C panic=abort --emit=obj app.rs -L $(TMPDIR)
