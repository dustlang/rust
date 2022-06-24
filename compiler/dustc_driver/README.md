The `driver` crate is effectively the "main" function for the dust
compiler.  It orchestrates the compilation process and "knits together"
the code from the other crates within dustc. This crate itself does
not contain any of the "main logic" of the compiler (though it does
have some code related to pretty printing or other minor compiler
options).

For more information about how the driver works, see the [dustc dev guide].

[dustc dev guide]: https://dustc-dev-guide.dust-lang.org/dustc-driver.html
