# Compiler-Principle Course Project

This project implements a compiler that can compile a C-like language into binary codes. It is based on flex, bison, LLVM.

## Dependency

1. Flex & Bison

    The lexer and parser of this project are implemented using flex and bison, respectively.

2. LLVM-14

    The code generation of this project is implemented using the C++ API of LLVM (version 14).

3. CMake

    We use CMake to build our project.

## Installation

1. Windows

   - Install Flex

     Flex can be downloaded from the GnuWin32.

     Link: http://gnuwin32.sourceforge.net/packages/flex.htm

     **Make sure that there is no space characters in your installation path.**

   - Install Bison

     Bison can be downloaded from the GnuWin32.

     http://gnuwin32.sourceforge.net/packages/bison.htm

     **Make sure that there is no space characters in your installation path.**

   - Add `<GNU Flex&Bison installation path>\GnuWin32\bin` to `PATH` environment variable.

   - Install LLVM C++ API

     We recommend using VS2019 on Windows. For instructions on installing LLVM on Windows using Visual Studio, refer to [Getting Started with the LLVM System using Microsoft Visual Studio](https://llvm.org/docs/GettingStartedVS.html). Please make sure that your LLVM's version is at least 14.

2. Ubuntu

   - Install Flex & Bison

     `sudo apt-get update && sudo apt-get upgrade && sudo apt-get install flex bison`

   - Install LLVM C++ API

     `sudo apt-get install llvm-14`

## Build & Compile

1. Clone this repository to your local computer:

   `cd <Your working directory>`

   `git clone git@github.com:YJJfish/C-Compiler.git`

   `cd ./C-Compiler`

2. Use CMake to construct the project:

   `cmake -S . -B ./build `

   `cd ./build`

   This step will automatically execute `flex` and `bison` command to generate `Lexer.cpp`, `Parser.hpp` and `Parser.cpp` in directory `C-Compiler/src`.

   If CMake throws an error due to "zlib" package, please install it on your computer:

   `sudo apt install zlib1g-dev`

3. If you are using Windows and have installed Visual Studio on your computer, by default CMake will construct a Visual Studio solution `C-Compiler.sln` in the `build` directory. Open the solution and change compilation mode to `Release x64`, then you can compile the codes using Visual Studio.

   If you are using Ubuntu, by default CMake will construct a project with `Makefile`. Using `make` command to compile the code.

## Usage

- `-i`: Specify input file (source code). REQUIRED.

- `-o`: Specify output file (object code). DEFAULT: `a.o`.

- `-l`: Specify where to dump llvm IR code. If \"`-l`\" is used but no file is specified, IR code will be printed to the console.

- `-v`: Specify where to dump visualization file.

- `-O`: Specify the level of optimization. Supported: `-O0`, `-O1`, `-O2`, `-O3`, `-Oz`, `-Os`.

For example, if you have successfully compiled our code and get an executable whose name is `C-Compiler` or `C-Compiler.exe`. In the same directory, there is a test code whose name is `Test.c`. Execute the following command:

`./C-Compiler -i ./Test.c -o Test.o -O3 -l`

After compiling `Test.c` with optimization level `O3` successfully, the IR code will be printed to the console, and the binary code will be written to `Test.o`.

To run the target code on your machine, you can use any linker to transform `Test.o` into an executable. For example:

`gcc ./Test.o`

`./a.exe`(Windows) or `./a.out`(Linux)

## Test Samples

We provide many test samples, from simple ones like `HelloWorld.c` to complex ones like `B+Tree.c`. All codes are placed in `./C-Compiler/test/`.

## Contributors

- #### [Jinjiang You](https://yjjfish.github.io/)(尤锦江)

  Context Free Grammar Design & Code Generation & Test Samples & Documentation

- #### Zhiyun Zhang(张之昀)

  Lexer & Parser

- #### Yihang Chen(陈一航)

  Visualization & Test Samples
