-include ../tools.mk
DUSTC_FLAGS = -C link-arg="-lfoo" -C link-arg="-lbar" -Z print-link-args

all:
	$(DUSTC) $(DUSTC_FLAGS) empty.rs | $(CGREP) lfoo lbar
