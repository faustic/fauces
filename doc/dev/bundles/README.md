# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## BUNDLES

A bundle is an embodiment of our generic C++ implementation for a particular architecture and platform, or, more generally, for a particular set of architectures and platforms. Bundles are not usually part of the Fauces project, but rather examples of application. Only bundles for our reference targets, plus one experimental bundle for testing, will be included in the project. Anyone will be able to provide other bundles for any other supported target using the translation library and the generic standard library.

A bundle is made of three parts:

* The standard library.
* The startup file.
* The translator.

### The standard library

Our generic C++ standard library contains some parts that depend on platform or architecture. The C++ standard library inside a bundle lacks all the parts that depend on platforms or architectures other than the ones supported by that bundle.

The standard library is expected to be provided as a set of header files and static libraries.

### The startup file

The startup file is the file that contains the code responsible for calling the `main` function. It is usually expected to be provided as an object file.

### The translator

The translator is the program that converts C++ source files into object files or executable files. Therefore it behaves both as a compiler and a linker. The translator uses the translation library for most of the work. It is expected to be provided as an executable file.

### Experimental bundle

The experimental bundle is expected to be the primary user of the translation library during the initial stages of development.

#### Experimental translator

The experimental translator in the experimental bundle has started as a copy of our initial attempt at a translator for the virtual target. Most of the source files have been moved to the translator library.

The translator takes a list of source files and object files as its input and produces an executable file as its output. In the future it is expected that it will also be able to produce object files as output.

To perform its translation work, the translator uses two classes from the translation library: `Supply` and `Linked_program`. `Supply` represents a set of translated translation units. This class has a `link()` member function that returns a `Linked_program` as its output.

