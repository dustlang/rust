-include ../tools.mk

all:
	$(DUSTC) --crate-type=rlib crateA.rs
	$(DUSTC) --crate-type=rlib crateB.rs
	$(call REMOVE_RLIBS,crateA)
	# Ensure crateC fails to compile since dependency crateA is missing
	$(DUSTC) crateC.rs 2>&1 | \
		$(CGREP) 'can'"'"'t find crate for `crateA` which `crateB` depends on'
