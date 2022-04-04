# enhanced-cstrings

Sized strings and string_view for C, with utility functions so it feels like a real string.
Not a ton of features yet, but the basic set of features you would expect from a string are there.

By default, the only real dependency is libc. Currently, the library depends on GNU extensions, but this may change in the future.

If you would like to use libgc instead of the system malloc, (since keeping track of allocation with owned strings can be difficult) you can define `ESTRING_USE_GC` or compile with `-DESTRING_USE_GC`.
