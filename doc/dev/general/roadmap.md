# Fauces

## ROADMAP

Project phases:

1. Initial planning. -- *Done: 9 December 2020* --
2. Experimenting.
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

### Phase 2: Experimenting

Main goal: consolidate project strategy.

Milestones:

* Virtual system processor specification. -- *Done: 21 December 2020* --
* Virtual system runner. -- *Done: 11 January 2021* --
* Virtual target specification. -- *Done: 9 February 2021* --
* 64-bit reference target specification.
* 32-bit reference target specification.
* 16-bit reference target specification.

### Phase 3: Language

Main goal: implement a compiler that supports the most common elements of the
C++ language.

Milestones:

* Virtual target essential operations.
* Portable operations.
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

We set a milestone for each language and library clause in the C++20 standard.
This may be revised at a later time if a new revision of the standard is
published before phase 5 is near completion.

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
