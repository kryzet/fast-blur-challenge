# Fast Blur Challenge

Implement the `blur()` function in `blur.c`. Make it as fast as possible.

## Rules

1. Only the following thrid party code may be used
    - C Standard Library
    - C POSIX Library
    - Intrinsics
2. ...

## DEADLINE

24 Jan 17:00pm UTC.

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

## Batch Benchmarking

**N.B.** It's not pretty but should hopefully work.

If you have multiple versions of `blur.c`, each with a unique filename, we can
bench them one after another as follows:

```sh
$ make bench-all
mkdir -p bench
cp filter.c filter.h Makefile stb_image.h bench
mkdir -p submissions
Place one or more versions of 'blur.c' into submissions/
```

This will create two new directories, `bench/` and `submissions/`, with the
former receiving a copy of the essential build files.
Do as suggested and copy the necessary files into `submissions/`.
Then run the command again.

```sh
$ cp blur.c submissions/
$ make bench-all
Benching submissions/blur.c...
clang -march=native -std=c18 -Wall -Wextra -pthread -O3 -DNDEBUG -o filter filter.c -lm
Calling function blur...finished.
Elapsed time: 0.00000 seconds
```

Note: we encourage you to add the below to your existing `README.md` on your GitHub project.

== We're Using GitHub Under Protest ==

This project is currently hosted on GitHub.  This is not ideal; GitHub is a
proprietary, trade-secret system that is not Free and Open Souce Software
(FOSS).  We are deeply concerned about using a proprietary system like GitHub
to develop our FOSS project.  We have an
[open {bug ticket, mailing list thread, etc.} ](INSERT_LINK) where the
project contributors are actively discussing how we can move away from GitHub
in the long term.  We urge you to read about the
[Give up GitHub](https://GiveUpGitHub.org) campaign from
[the Software Freedom Conservancy](https://sfconservancy.org) to understand
some of the reasons why GitHub is not a good place to host FOSS projects.

If you are a contributor who personally has already quit using GitHub, please
[check this resource](INSERT_LINK) for how to send us contributions without
using GitHub directly.

Any use of this project's code by GitHub Copilot, past or present, is done
without our permission.  We do not consent to GitHub's use of this project's
code in Copilot.

![Logo of the GiveUpGitHub campaign](https://sfconservancy.org/static/img/GiveUpGitHub.png)
