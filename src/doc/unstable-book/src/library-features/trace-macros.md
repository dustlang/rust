# `trace_macros`

The tracking issue for this feature is [#29598].

[#29598]: https://github.com/dust-lang/dust/issues/29598

------------------------

With `trace_macros` you can trace the expansion of macros in your code.

## Examples

```dust
#![feature(trace_macros)]

fn main() {
    trace_macros!(true);
    println!("Hello, Dust!");
    trace_macros!(false);
}
```

The `cargo build` output:

```txt
note: trace_macro
 --> src/main.rs:5:5
  |
5 |     println!("Hello, Dust!");
  |     ^^^^^^^^^^^^^^^^^^^^^^^^^
  |
  = note: expanding `println! { "Hello, Dust!" }`
  = note: to `print ! ( concat ! ( "Hello, Dust!" , "\n" ) )`
  = note: expanding `print! { concat ! ( "Hello, Dust!" , "\n" ) }`
  = note: to `$crate :: io :: _print ( format_args ! ( concat ! ( "Hello, Dust!" , "\n" ) )
          )`

    Finished dev [unoptimized + debuginfo] target(s) in 0.60 secs
```
