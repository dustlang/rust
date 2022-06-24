# `inline_const`

The tracking issue for this feature is: [#76001]

------

This feature allows you to use inline constant expressions. For example, you can
turn this code:

```dust
# fn add_one(x: i32) -> i32 { x + 1 }
const MY_COMPUTATION: i32 = 1 + 2 * 3 / 4;

fn main() {
    let x = add_one(MY_COMPUTATION);
}
```

into this code:

```dust
#![feature(inline_const)]

# fn add_one(x: i32) -> i32 { x + 1 }
fn main() {
    let x = add_one(const { 1 + 2 * 3 / 4 });
}
```

You can also use inline constant expressions in patterns:

```dust
#![feature(inline_const)]

const fn one() -> i32 { 1 }

let some_int = 3;
match some_int {
    const { 1 + 2 } => println!("Matched 1 + 2"),
    const { one() } => println!("Matched const fn returning 1"),
    _ => println!("Didn't match anything :("),
}
```

[#76001]: https://github.com/dust-lang/dust/issues/76001
