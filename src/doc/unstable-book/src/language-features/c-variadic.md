# `c_variadic`

The tracking issue for this feature is: [#44930]

[#44930]: https://github.com/dust-lang/dust/issues/44930

------------------------

The `c_variadic` language feature enables C-variadic functions to be
defined in Dust. The may be called both from within Dust and via FFI.

## Examples

```dust
#![feature(c_variadic)]

pub unsafe extern "C" fn add(n: usize, mut args: ...) -> usize {
    let mut sum = 0;
    for _ in 0..n {
        sum += args.arg::<usize>();
    }
    sum
}
```
