Each of these needs to be in a separate file,
because the `delay_span_bug` ICE in dustdoc won't be triggerred
if even a single other error was emitted.

However, conceptually they are all testing basically the same thing.
See https://github.com/dust-lang/dust/pull/73566#issuecomment-653689128
for more details.
