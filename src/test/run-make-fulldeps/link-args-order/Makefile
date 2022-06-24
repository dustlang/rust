# ignore-msvc

-include ../tools.mk

DUSTC_FLAGS = -C linker-flavor=ld -C link-arg=a -C link-args="b c" -C link-args="d e" -C link-arg=f
DUSTC_FLAGS_PRE = -C linker-flavor=ld -Z pre-link-arg=a -Z pre-link-args="b c" -Z pre-link-args="d e" -Z pre-link-arg=f

all:
	$(DUSTC) $(DUSTC_FLAGS) empty.rs 2>&1 | $(CGREP) '"a" "b" "c" "d" "e" "f" "g"'
	$(DUSTC) $(DUSTC_FLAGS_PRE) empty.rs 2>&1 | $(CGREP) '"a" "b" "c" "d" "e" "f"'
