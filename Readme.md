Malco programming language [![Appveyor build][badge-appveyor]][appveyor] [![Travis build][badge-travis]][travis]
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

### Note for Visual Studio users

You may also open a folder with `CMakeLists.txt` directly in Visual Studio 2017
(using menu item File → Open → Folder…).

Documentation
-------------

Check out the [docs folder][docs].

[docs]: docs/

[appveyor]: https://ci.appveyor.com/project/ForNeVeR/malco/branch/master
[cmake]: https://cmake.org/
[fornever]: https://github.com/ForNeVeR/
[impworks]: https://github.com/impworks/
[travis]: https://travis-ci.org/ForNeVeR/malco

[badge-appveyor]: https://ci.appveyor.com/api/projects/status/mngu2ri0wwr09vfn/branch/master?svg=true
[badge-travis]: https://travis-ci.org/ForNeVeR/malco.svg?branch=master
