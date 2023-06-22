# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## ROADMAP

Project phases:

1. Initial planning. -- *Done: 9 December 2020* --
2. Initial experimenting. -- *Done: 30 May 2022* --
3. Language.
4. Library.
5. Standard.

A phase may start before the previous phase has ended, unless there is something
not yet ready from the previous phase that is required by the phase being
started.

### Phase 1: Initial planning

Main goal: define project strategy.

Milestones:

* Roadmap. -- *Done: 9 December 2020* --
* Development strategy. -- *Done: 9 December 2020* --
* Documentation strategy. -- *Done: 9 December 2020* --
* Documentation map. -- *Done: 9 December 2020* --

### Phase 2: Initial experimenting

Main goal: consolidate project strategy and do some early experiments.

Milestones:

* Virtual system processor specification. -- *Done: 21 December 2020* --
* Virtual system runner. -- *Done: 11 January 2021* --
* Virtual target specification. -- *Done: 9 February 2021* --
* 64-bit reference target specification. -- *Done: 27 May 2022* --
* 32-bit reference target specification. -- *Done: 30 May 2022* --
* 16-bit reference target specification. -- *Done: 30 May 2022* --

### Phase 3: Language

Main goal: implement a compiler that supports the most common elements of the
C++ language.

Milestones:

* Virtual target essential operations.
    * Specification of translator source structure. -- *Done: 22 June 2023* --
    * Specification of essential operations.
    * Minimal implementation of language translation.
        * Adapt `cpp_visyv1_i16` experiment to specified structure.
        * Minimalistic compiler foundations.
        * Minimalistic linker foundations.
        * Minimalistic startup code.
        * Minimalistic test environment based on `visy1010` experiment.
    * Implementation of essential operations.
        * Flow control.
            * Call function with no parameters.
            * Call function with basic integer parameters.
            * Return void or basic integer from function.
            * Go to label.
            * Go to label if boolean is true.
        * Pointer to unsigned char.
            * Addition of size integer.
        * Assignment from identical type.
        * Unsigned basic integer.
            * One's complement.
            * Bitwise AND.
            * Left shift.
            * Right shift.
            * Equality.
            * Less than.
        * Other operations depending on what has been initially specified and on later changes to specification.
* Portable operations.
    * Abstract support for operations on objects.
        * Pass object to function.
        * Return object from function.
        * Type conversions.
        * Assignment operations.
        * Pointer operations.
        * Logical operations.
        * Bitwise operations 
        * Additive and multiplicative operations.
        * Conditional evaluation (`a ? b : c`).
        * Comma expression. (`a, b`).
        * Class operations.
    * Types. Implement operations applicable to each type.
        * Pointers.
        * Arrays.
        * Functions.
        * Integers and booleans.
        * Floating-point numbers.
        * References.
        * Classes and structures.
        * Pointers to members.
        * Unions.
    * Flow control.
        * If.
        * If else.
        * While.
        * Do while.
        * For.
        * Switch.
        * Break.
        * Continue.
    * Essential template support.
* 64-bit reference target essential operations.
* 32-bit reference target essential operations.
* 16-bit reference target essential operations.

### Phase 4: Library

Main goal: implement the most common parts of the C++ standard library.

Milestones:

* Virtual target linker.
* 64-bit target system linker.
* 32-bit target system linker.
* 16-bit target system linker.
* `cstddef`
* `limits`
* `climits`
* `cstdint`
* `cstdlib`
* `memory`
* `string`
* `vector`
* `unordered_map`
* `fstream`
* Self-building of compiler and library.

### Phase 5: Standard

Main goal: complete ISO C++ compliance.

We set a milestone for each language and library clause in the C++20 standard. These milestones will be revised based on the standard version that is current when we start work on this phase.

Milestones:

* Lexical conventions.
* Basics.
* Expressions.
* Statements.
* Declarations.
* Modules.
* Classes.
* Overloading.
* Templates.
* Exception handling.
* Preprocessing directives.
* Language support library.
* Concepts library
* Diagnostics library.
* General utilities library.
* Strings library.
* Containers library.
* Iterators library.
* Ranges library.
* Algorithms library.
* Numerics library.
* Time library.
* Localization library.
* Input/output library.
* Regular expressions library.
* Atomic operations library.
* Thread support library.
