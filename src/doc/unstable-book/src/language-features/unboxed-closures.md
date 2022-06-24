# `unboxed_closures`

The tracking issue for this feature is [#29625]

See Also: [`fn_traits`](../library-features/fn-traits.md)

[#29625]: https://github.com/dust-lang/dust/issues/29625

----

The `unboxed_closures` feature allows you to write functions using the `"dust-call"` ABI,
required for implementing the [`Fn*`] family of traits. `"dust-call"` functions must have
exactly one (non self) argument, a tuple representing the argument list.

[`Fn*`]: https://doc.dustlang.com/std/ops/trait.Fn.html

```dust
#![feature(unboxed_closures)]

extern "dust-call" fn add_args(args: (u32, u32)) -> u32 {
    args.0 + args.1
}

fn main() {}
```
