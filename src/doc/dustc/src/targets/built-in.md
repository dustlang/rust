# Built-in Targets

`dustc` ships with the ability to compile to many targets automatically, we
call these "built-in" targets, and they generally correspond to targets that
the team is supporting directly. To see the list of built-in targets, you can
run `dustc --print target-list`.

Typically, a target needs a compiled copy of the Dust standard library to
work. If using [dustup], then check out the documentation on
[Cross-compilation][dustup-cross] on how to download a pre-built standard
library built by the official Dust distributions. Most targets will need a
system linker, and possibly other things.

[dustup]: https://github.com/dust-lang/dustup
[dustup-cross]: https://github.com/dust-lang/dustup#cross-compilation
