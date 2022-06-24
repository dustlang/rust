-include ../tools.mk

# This tests ensure that atomic types are never lowered into runtime library calls that are not
# guaranteed to be lock-free.

all:
ifeq ($(UNAME),Linux)
ifeq ($(filter x86,$(LLVM_COMPONENTS)),x86)
	$(DUSTC) --target=i686-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=x86_64-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
ifeq ($(filter arm,$(LLVM_COMPONENTS)),arm)
	$(DUSTC) --target=arm-unknown-linux-gnueabi atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=arm-unknown-linux-gnueabihf atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=armv7-unknown-linux-gnueabihf atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=thumbv7neon-unknown-linux-gnueabihf atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
ifeq ($(filter aarch64,$(LLVM_COMPONENTS)),aarch64)
	$(DUSTC) --target=aarch64-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
ifeq ($(filter mips,$(LLVM_COMPONENTS)),mips)
	$(DUSTC) --target=mips-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=mipsel-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
ifeq ($(filter powerpc,$(LLVM_COMPONENTS)),powerpc)
	$(DUSTC) --target=powerpc-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=powerpc-unknown-linux-gnuspe atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=powerpc64-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
	$(DUSTC) --target=powerpc64le-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
ifeq ($(filter systemz,$(LLVM_COMPONENTS)),systemz)
	$(DUSTC) --target=s390x-unknown-linux-gnu atomic_lock_free.rs
	nm "$(TMPDIR)/libatomic_lock_free.rlib" | $(CGREP) -v __atomic_fetch_add
endif
endif
