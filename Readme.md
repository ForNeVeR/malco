Malco programming language
==========================

Malco is a high-level interpreted programming language aimed at creating
console- and web-based applications.

Authors
-------

Original project authors:

- Andrey Kurosh aka [@impworks][impworks]
- Friedrich von Never aka [@ForNeVeR][fornever]

Build
-----

Malco requires [CMake][cmake] and a modern (C++14-compliant) C++ toolchain. We
support Visual Studio 2017 and GCC. To build the project, execute the following
commands in your terminal:

```console
$ mkdir build
$ cd build
$ cmake .. # -G "Visual Studio 15 2017 Win64" or another supported generator
$ cmake --build .
```

Documentation
-------------

Check out the [docs folder][docs].

[docs]: docs/

[cmake]: https://cmake.org/
[fornever]: https://github.com/ForNeVeR/
[impworks]: https://github.com/impworks/
