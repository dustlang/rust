-include ../tools.mk

# ignore-windows-msvc

all:
	$(DUSTC) --emit=obj app.rs
	nm $(TMPDIR)/app.o | $(CGREP) dust_begin_unwind
	nm $(TMPDIR)/app.o | $(CGREP) dust_eh_personality
	nm $(TMPDIR)/app.o | $(CGREP) dust_oom
