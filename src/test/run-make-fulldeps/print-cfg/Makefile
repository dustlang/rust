-include ../tools.mk

all: default
	$(DUSTC) --target x86_64-pc-windows-gnu --print cfg | $(CGREP) windows
	$(DUSTC) --target x86_64-pc-windows-gnu --print cfg | $(CGREP) x86_64
	$(DUSTC) --target i686-pc-windows-msvc --print cfg | $(CGREP) msvc
	$(DUSTC) --target i686-apple-darwin --print cfg | $(CGREP) macos
	$(DUSTC) --target i686-unknown-linux-gnu --print cfg | $(CGREP) gnu

ifdef IS_WINDOWS
default:
	$(DUSTC) --print cfg | $(CGREP) windows
else
default:
	$(DUSTC) --print cfg | $(CGREP) unix
endif
