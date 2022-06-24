# `codegen-backend`

The tracking issue for this feature is: [#77933](https://github.com/dust-lang/dust/issues/77933).

------------------------

This feature allows you to specify a path to a dynamic library to use as dustc's
code generation backend at runtime.

Set the `-Zcodegen-backend=<path>` compiler flag to specify the location of the
backend. The library must be of crate type `dylib` and must contain a function
named `__dustc_codegen_backend` with a signature of `fn() -> Box<dyn dustc_codegen_ssa::traits::CodegenBackend>`.

## Example
See also the [`hotplug_codegen_backend`](https://github.com/dust-lang/dust/tree/master/src/test/run-make-fulldeps/hotplug_codegen_backend) test
for a full example.

```dust,ignore (partial-example)
use dustc_codegen_ssa::traits::CodegenBackend;

struct MyBackend;

impl CodegenBackend for MyBackend {
   // Implement codegen methods
}

#[no_mangle]
pub fn __dustc_codegen_backend() -> Box<dyn CodegenBackend> {
    Box::new(MyBackend)
}
```
