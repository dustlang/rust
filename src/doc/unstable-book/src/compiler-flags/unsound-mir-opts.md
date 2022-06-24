# `unsound-mir-opts`

--------------------

The `-Zunsound-mir-opts` compiler flag enables [MIR optimization passes] which can cause unsound behavior.
This flag should only be used by MIR optimization tests in the dustc test suite.

[MIR optimization passes]: https://dustc-dev-guide.dustlang.com/mir/optimizations.html
