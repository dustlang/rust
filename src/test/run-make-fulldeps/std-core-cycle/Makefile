-include ../tools.mk

ifeq ($(UNAME),Darwin)
FLAGS :=
else
ifdef IS_WINDOWS
FLAGS :=
else
FLAGS := -C link-args=-Wl,--no-undefined
endif
endif

all:
	$(DUSTC) bar.rs
	$(DUSTC) foo.rs $(FLAGS)
	$(DUSTC) foo.rs $(FLAGS) -C panic=abort
