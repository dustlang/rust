-include ../tools.mk

# only-windows

all:
	$(DUSTC) -o "$(TMPDIR)/hopefullydoesntexist bar.exe" hello.rs
	$(DUSTC) spawn.rs
	$(TMPDIR)/spawn.exe
