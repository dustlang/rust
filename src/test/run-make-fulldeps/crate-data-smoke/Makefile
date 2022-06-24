-include ../tools.mk

all:
	[ `$(DUSTC) --print crate-name crate.rs` = "foo" ]
	[ `$(DUSTC) --print file-names crate.rs` = "$(call BIN,foo)" ]
	[ `$(DUSTC) --print file-names --crate-type=lib \
		--test crate.rs` = "$(call BIN,foo)" ]
	[ `$(DUSTC) --print file-names --test lib.rs` = "$(call BIN,mylib)" ]
	$(DUSTC) --print file-names lib.rs
	$(DUSTC) --print file-names rlib.rs
