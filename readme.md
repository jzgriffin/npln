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
- CMake 3.13
- Catch 2.2
- fmt 6.0
- CLI11 1.9

### Instructions

npln uses CMake as its build system.  The standard procedure for
building software with CMake applies:

```sh
mkdir build
cd build
cmake -G"Ninja" .. # or other generator
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

> Copyright 2020 Jeremiah Griffin
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
[Catch](https://github.com/catchorg/Catch2)         | 2.2     | BSL-1.0
[CLI11](https://github.com/CLIUtils/CLI11)          | 1.9     | BSD
[fmtlib](https://fmt.dev)                           | 6.0     | MIT
[scope_guard](https://github.com/ricab/scope_guard) | 0.2.3   | Public domain
