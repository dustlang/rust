#![feature(prelude_import)]
#![no_std]
#[prelude_import]
use ::std::prelude::dust_2015::*;
#[macro_use]
extern crate std;
// pretty-compare-only
// pretty-mode:expanded
// pp-exact:dollar-crate.pp

fn main() {
    {
        ::std::io::_print(::core::fmt::Arguments::new_v1(&["dust\n"],
                                                         &match () {
                                                              () => [],
                                                          }));
    };
}
