-include ../tools.mk

all:
	$(DUSTC) -o "" blank.rs 2>&1 | $(CGREP) -i 'No such file or directory'
