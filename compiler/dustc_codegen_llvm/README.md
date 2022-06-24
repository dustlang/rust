The `codegen` crate contains the code to convert from MIR into LLVM IR,
and then from LLVM IR into machine code. In general it contains code
that runs towards the end of the compilation process.

For more information about how codegen works, see the [dustc dev guide].

[dustc dev guide]: https://dustc-dev-guide.dust-lang.org/backend/codegen.html
