# npln

## Introduction

npln is an environment for the PL/0 programming language running on the
CHIP-8 architecture.  It provides a PL/0 compile, a CHIP-8 assembler,
a CHIP-8 disassembler, a CHIP-8 debugger, and a CHIP-8 virtual machine.
The PL/0 programming language is extended with procedures and data types
that appear in the CHIP-8 architecture.

## Building

### Dependencies

- C++17 compiler
- CMake
- Conan

### Instructions

npln uses CMake as its build system and Conan as its package manager.
On systems using GCC, it is necessary to configure Conan with
`compiler.libcxx=libstdc++11` to avoid issues with package ABI.

The standard procedure for building software with Conan/CMake applies:

```sh
mkdir build
cd build
conan install .. # NOTE: compiler.libcxx=stdc++11 is needed
cmake -G ..
cmake --build .
ctest # to run the test suite
```

## Usage

### Running a CHIP-8 executable

The `run` subcommand can be used to run a CHIP-8 executable in a hardware-
accelerated virtual machine:
```sh
npln run <path-to-executable>
```

## License

npln is licensed under the terms of the permissive ISC open source
license:

> Copyright 2020-2022 Jeremiah Griffin
>
> Permission to use, copy, modify, and/or distribute this software for
> any purpose with or without fee is hereby granted, provided that the
> above copyright notice and this permission notice appear in all
> copies.
>
> THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
> WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
> WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE
> AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
> DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
> PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
> TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
> PERFORMANCE OF THIS SOFTWARE.

### Dependencies

Name                                                | Version | License
--------------------------------------------------- | ------- | --------------
[CLI11](https://github.com/CLIUtils/CLI11)          | 2.1.1   | BSD
[Catch](https://github.com/catchorg/Catch2)         | 2.13.7  | BSL-1.0
[GLFW](https://www.glfw.org)                        | 3.3.4   | zlib/libpng
[GLM](https://github.com/g-truc/glm)                | 0.9.9.8 | MIT
[GSL](https://github.com/microsoft/GSL)             | 4377f6e | MIT
[fmtlib](https://fmt.dev)                           | 8.0.1   | MIT
[frequencypp](https://github.com/nokurn/frequencypp | 0.1.1   | ISC
[glbinding](https://glbinding.org)                  | 70e76f9 | MIT
[globjects](https://globjects.org)                  | 72285e2 | MIT
[spdlog](https://github.com/gabime/spdlog)          | 1.9.2   | MIT
[CHIP-8 Games Pack](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html) | | Public Domain
