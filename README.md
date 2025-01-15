# Fast Blur Challenge

Implement the `blur()` function in `blur.c`. Make it as fast as possible.

## Rules

1. Only the following thrid party code may be used
    - C Standard Library
    - C POSIX Library
    - Intrinsics
2. ...

## DEADLINE

18 Jan 17:00pm UTC.
(Temporarily subject to alteration.)

## Target Platforms

The base target is the `cs50x codespace`:

- x86_64
- Linux
- 1 core; 2 threads
- No GPU

However, once solutions have been submitted, any participant may bench them
all locally, widening the target platforms to something more like:

- x86_64
- Linux/BSD
- Multi-core; multi-thread
- 1+ GPU

## Build

```sh
$ make [release]
$ make debug
$ make -B release
```

Using `-B` will be necessary to force a rebuild if switching between `release`
and `debug` when no changes have been made to `filter.c|h` since the last build.
