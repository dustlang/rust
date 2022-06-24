-include ../tools.mk

# ignore-windows

# Notice the space in the end, this emulates the output of pkg-config
DUSTC_FLAGS = -C link-args="-lc "

all:
	$(DUSTC) $(DUSTC_FLAGS) empty.rs
