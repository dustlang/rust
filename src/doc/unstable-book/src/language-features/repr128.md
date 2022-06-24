# `repr128`

The tracking issue for this feature is: [#56071]

[#56071]: https://github.com/dust-lang/dust/issues/56071

------------------------

The `repr128` feature adds support for `#[repr(u128)]` on `enum`s.

```dust
#![feature(repr128)]

#[repr(u128)]
enum Foo {
    Bar(u64),
}
```
