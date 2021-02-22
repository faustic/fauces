# Fauces

An attempt to create a C++ implementation that can be easily ported to arbitrary
architectures and platforms.

## Purpose

This project is motivated by the problems that often arise when trying to build
well-established C++ implementations for platforms or architectures that are too
old or too new or not popular enough. Sometimes, on a particular version of a
well-supported platform, some dependencies fail or some conflicts occur that
take a lot of effort to fix. Sometimes, support is lacking for a particular
platform or architecture, and the source code needs to be modified, which
requires a good knowledge of how it works.

Maybe the sensible approach would be to carefully study an existing open source
product, such as GCC or Clang, get enough expertise to be able to fix build
problems with less effort and learn how to modify its source code skilfully. But
instead of going that route, we are going to first try the more adventurous
strategy of developing a new C++ implementation from scratch.

## Parts

The project is made of the following parts:

* Virtual system. An example of a system with a very simple architecture,
implemented as software, to test things before porting them to real systems.
* C++ compiler. The program that converts each C++ translation unit into an
object file. There will usually be one compiler for each supported architecture.
* Static linker. The program that combines all object files to form an
executable file or a library file. There will usually be one for each supported
platform. When a platform provides its own linker, it can be used instead, but
ours will still be needed in case of cross-platform building.
* Compiler helper library. A set of objects and functions that the code
generated by the compiler may use at runtime for the sake of saving size or
decreasing the number of architecture-dependent lines.
* Standard library. The set of objects, classes and functions required by the
standard as part of the C++ implementation. It may be in the same library file
as the compiler helper library.
* Tests. A set of programs to make experiments and help verify how the different
parts of the project work.

## Priorities

1. Functionality.
2. Portability.
3. Simplicity.
4. Good documentation.
5. Compiled code speed.
6. Compilation speed.
7. Completion

## Functionality

The prime objective is that the implementation works. Even if not every feature
required by the standard is implemented yet, care should be taken to ensure that
those features that are implemented comply with the standard. Which version of
the standard is something that should be decided at an early stage of
development.

## Portability

It is a high priority that the compiler and standard library can be ported to
multiple architectures and platforms.

### Architectures

In the context of this project, architecture means the aspects of the
environment that control how expressions are evaluated and how a program flows.
This includes things like CPU family, calling conventions, size and alignment of
basic types, byte ordering, etc.

Although architecture may affect each part of the project, it mainly affects the
compiler. To make the compiler easier to port across architectures, the
following strategy can be followed:

* Only require from the compiler to directly support a minimum set of operations
on basic types. The rest will be provided by the compiler helper library in a
manner that is either architecture-independent or dependent only on generic
features that are common to a variety of architectures.
* Only require from the compiler to directly support a minimum set of flow
control statements. The rest will be constructed from the former with the help
of the compiler helper library.
* Provide a model compiler that can be used as a starting point, making it clear
what should be mandatorily implemented and what can be added later when
optimisations are desired.

### Platforms

By platform we mean the aspects of the environment that control (or neglect to
control) how resources are shared between programs or within a program. This
includes memory allocation, file reading and writing, threading, timing...

Platform will have an effect mainly on the standard library. To make the
standard library easier to port across platforms, the following strategy can be
followed.

* When there is a choice, a function from the standard library should depend on
another function of the standard library rather than on a platform-dependent
function.
* Keep a clear separation between functions that should always be
platform-independent, functions that are always platform-dependent and functions
that might be implemented either way according to optimisation needs.
* Provide a model set of platform-dependent functions, making it clear
what should be mandatorily implemented and what can be implemented later when
optimisations are desired.

## Simplicity

Complexity makes programs more difficult to maintain. But keeping things simple
is not easy and programs tend to get bloated, especially those that target a
wide public. The reasons are varied: the wish of being flexible and supporting a
lot of options; obsolete lines that never get cleaned up out of fear of breaking
something; lengthy and repetitive code that no one finds the time to organise in
small reusable functions...

A few clues to keep it simple:

* Do not define pragmas or command line options just in case you need them some
time in the future. Add them only when you really need them, and try to think
first if there is a simpler way to solve the problem.
* Whenever you find a simpler or more efficient way to do something, try to
apply it everywhere in the project where it fits, rather than keeping multiple
pieces of code that do the same thing in different ways.
* Whenever you find yourself copying and pasting big blocks of code, stop for a
minute and try to think of a simpler way to organise your code.

## Good documentation

A few guidelines to create good documentation can be found
[here](doc/dev/general/doc_strategy.md).

## Compiled code speed

Once a compiler for a certain architecture is working, we should keep optimising
it, particularly addressing the compromises made earlier for the sake of quick
porting.

## Compilation speed

It should not take a lot of time to compile a source file, but it is usually not
as critical as the speed of compiled code.

## Completion

Although there is no hurry, all features required by the standard should
eventually be implemented.

