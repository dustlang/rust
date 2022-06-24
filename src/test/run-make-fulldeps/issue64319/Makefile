-include ../../run-make-fulldeps/tools.mk

# Different optimization levels imply different values for `-Zshare-generics`,
# so try out a whole bunch of combinations to make sure everything is compatible
all:
	# First up, try some defaults
	$(DUSTC) --crate-type rlib foo.rs
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=3

	# Next try mixing up some things explicitly
	$(DUSTC) --crate-type rlib foo.rs -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=no
	$(DUSTC) --crate-type rlib foo.rs -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=yes
	$(DUSTC) --crate-type rlib foo.rs -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=no
	$(DUSTC) --crate-type rlib foo.rs -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=yes

	# Now combine a whole bunch of options together
	$(DUSTC) --crate-type rlib foo.rs
	$(DUSTC) --crate-type dylib bar.rs
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=1
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=1 -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=1 -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=2
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=2 -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=2 -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=3
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=3 -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=3 -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=s
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=s -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=s -Z share-generics=yes
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=z
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=z -Z share-generics=no
	$(DUSTC) --crate-type dylib bar.rs -C opt-level=z -Z share-generics=yes
