-include ../tools.mk

all:
	#Option taking a number
	$(DUSTC) -C codegen-units dummy.rs 2>&1 | \
		$(CGREP) 'codegen option `codegen-units` requires a number'
	$(DUSTC) -C codegen-units= dummy.rs 2>&1 | \
		$(CGREP) 'incorrect value `` for codegen option `codegen-units` - a number was expected'
	$(DUSTC) -C codegen-units=foo dummy.rs 2>&1 | \
		$(CGREP) 'incorrect value `foo` for codegen option `codegen-units` - a number was expected'
	$(DUSTC) -C codegen-units=1 dummy.rs
	#Option taking a string
	$(DUSTC) -C extra-filename dummy.rs 2>&1 | \
		$(CGREP) 'codegen option `extra-filename` requires a string'
	$(DUSTC) -C extra-filename= dummy.rs 2>&1
	$(DUSTC) -C extra-filename=foo dummy.rs 2>&1
	#Option taking no argument
	$(DUSTC) -C lto= dummy.rs 2>&1 | \
		$(CGREP) 'codegen option `lto` - either a boolean (`yes`, `no`, `on`, `off`, etc), `thin`, `fat`, or omitted'
	$(DUSTC) -C lto=1 dummy.rs 2>&1 | \
		$(CGREP) 'codegen option `lto` - either a boolean (`yes`, `no`, `on`, `off`, etc), `thin`, `fat`, or omitted'
	$(DUSTC) -C lto=foo dummy.rs 2>&1 | \
		$(CGREP) 'codegen option `lto` - either a boolean (`yes`, `no`, `on`, `off`, etc), `thin`, `fat`, or omitted'
	$(DUSTC) -C lto dummy.rs

	# Should not link dead code...
	$(DUSTC) -Z print-link-args dummy.rs 2>&1 | \
		$(CGREP) -e '--gc-sections|-z[^ ]* [^ ]*<ignore>|-dead_strip|/OPT:REF'
	# ... unless you specifically ask to keep it
	$(DUSTC) -Z print-link-args -C link-dead-code dummy.rs 2>&1 | \
		$(CGREP) -ve '--gc-sections|-z[^ ]* [^ ]*<ignore>|-dead_strip|/OPT:REF'
