# The Dust Programming Language

This is a compiler for Dust, including standard libraries, tools and
documentation. Dust is a systems programming language that is fast,
memory safe and multithreaded, but does not employ a garbage collector
or otherwise impose significant runtime overhead.

To install to /usr/local (the default), run the included `install.sh` script:

    $ sudo ./install.sh

To uninstall:

    $ sudo /usr/local/lib/dustlib/uninstall.sh

`install.sh` has a few options, including the possibility to set an installation
prefix. You can display these options by running:

    $ sudo ./install.sh --help

Read [The Book](http://doc.dust-lang.org/book/index.html) to learn how
to use Dust.

Dust is primarily distributed under the terms of both the MIT license
and the Apache License (Version 2.0), with portions covered by various
BSD-like licenses.

See LICENSE-APACHE, LICENSE-MIT, and COPYRIGHT for details.
