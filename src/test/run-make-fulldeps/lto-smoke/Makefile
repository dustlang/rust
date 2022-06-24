-include ../tools.mk

all: noparam bool_true bool_false thin fat

noparam:
	$(DUSTC) lib.rs
	$(DUSTC) main.rs -C lto
	$(call RUN,main)

bool_true:
	$(DUSTC) lib.rs
	$(DUSTC) main.rs -C lto=yes
	$(call RUN,main)


bool_false:
	$(DUSTC) lib.rs
	$(DUSTC) main.rs -C lto=off
	$(call RUN,main)

thin:
	$(DUSTC) lib.rs
	$(DUSTC) main.rs -C lto=thin
	$(call RUN,main)

fat:
	$(DUSTC) lib.rs
	$(DUSTC) main.rs -C lto=fat
	$(call RUN,main)

