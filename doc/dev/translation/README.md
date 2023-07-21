# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

The C++ standard defines nine phases of translation. Implementations do not need to explicitly implement these phases as long as the behaviour is the same as if they did.

In our first tries, we intend to implement each phase explicitly and clearly separated from the others. As we get more familiar with the translation process, we may instead decide to join several phases together or do things in a more optimal order.

* [Phase 1](phase1.md)
* [Phase 2](phase2.md)
* [Phase 3](phase3.md)
* [Phase 4](phase4.md)
* [Phase 5](phase5.md)
* [Phase 6](phase6.md)
* [Phase 7](phase7.md)
* [Phase 8](phase8.md)
* [Phase 9](phase9.md)

##  Translation library

To implement the phases of translation as required by the C++ standard, we provide a translation library. The translation library is still in a very preliminary state, so this documentation must be seen as list of initial ideas, many of them not implemented yet, rather than as a reference.

From the point of view of the translator program, the most important class provided by the translation library is the `Supply` class, which assigns all required translation work to the different parts of the library. All the translator program needs to do is provide compatible input to a `Supply` object and wait for it to perform its duties.

### Public classes and interfaces

* `Supply`. The `Supply` class represents a supply of translated code. The user of this class, normally a translator program, adds input files to the supply and then asks for an output. The particular kind of output depends on the corresponding member function of `Supply` that has been called:
    * `link`. Returns an object of type `Linked_program`, that can be saved as an executable file. Only this option is available in our first experiments.
    * `compile`. Returns a collection of objects that can be saved as object files, whose format depends on a number of parameters (architecture, platform and possibly other choices).
    * `linklib`. Returns an object of type `Linked_lib`, that can be saved as a static library.
    * `linkdlib`.  Returns an object of type `Linked_dlib`, that can be saved as a dynamic library.
* `Linked_program`. An object of this class is returned when calling `Supply::link()`.
* `Linked_lib`. An object of this class is returned when calling `Supply::linklib()`.
* `Linked_dlib`. An object of this class is returned when calling `Supply::linkdlib()`.
* `Architecture`. An interface for architecture parameters. For every supported architecture, there must be a class implementing this interface. It informs the translation library of everything it needs to know about the architecture, from integer sizes to the processor instruction set. Our first experiments did not bother with this and just assumed a particular architecture (our virtual system architecture) whenever architecture-dependent information was needed.
* `Platform`. An interface for platform parameters.

### Private classes and interfaces.

* `Translated_unit`. This is a representation, private to the translation library, of a translated translation unit.
* `Token`. It is a sequence of source code characters that cannot be further decomposed into smaller syntactically significant units.
* `Instantiantion_unit`. For now, it is just an alias for `Translated_unit`. Depending on how we implement template instantiation, we may need to differentiate both classes. Until then, we will use both terms interchangeably.
* `Translated_unit_loader`. An abstract class, whose derived classes must implement the `load()` function, which returns an object of type `Translated_unit`. When an input file is added to the supply, it is determined at run time which derived class must be used to load the `Translated_unit`, depending on the type of the file.
* `Fo16_unit_loader`: A class derived from `Translated_unit_loader` to load a Fo16 object file as a `Translated_unit`. This was the first object format we supported. Every other object format we support must have its corresponding `Translated_unit_loader` derived class.
* `Translator`: A class derived from `Translated_unit_loader` to load a C++ source file as a `Translated_unit` (by translating it).

### The `Translator` class

Most of the translation work happens inside the `Translator` class. When the translation library becomes aware that a C++ source file is being added to the supply, an object of class `Translator` is created and its `load()` member function is called. This call performs phases 1 to 8 of the translation process.

#### Initial implementation

When `Translator::load()` is called, it calls in turn a sequence of private functions to perform the translation:

* `preprocess`
    * `pretokenize`: three first phases of translation.
    * `execute_directives`: phase 4.
* `convert_literals`: phase 5.
* `concatenate_literals`: phase 6.
* `analyze`: phase 7.
* `instantiate`: phase 8.

Most of these functions are still incomplete. However, with the exception of `analyze`, each one already produces the expected kind of output, even if most potential inputs are still not accepted. This means we can start trying to translate very simple programs and progressively try to support more syntactic elements as we encounter them.

Currently, the `analyze` function always throws an error. Its prototype is:

    analyze(std::list<Token>& tokens, Translated_unit& unit);

The idea is to analyze the provided list of tokens and generate accordingly the appropriate machine code into the `Translated_unit` object. The initial implementation was interrupted without putting the idea into practice. To continue the work, we just need to better clarify our aims concerning the translation process.

#### Aims

* Architecture management
* Platform management
* Incremental development
* Minimalistic foundations

##### Architecture management

We will manage architecture-dependent features by means of an architecture class defined for each architecture we support. Probably, the most convenient approach is to define the translator class as a template that takes the architecture class as a parameter. Whenever we want to add support for an architecture-dependent feature, we will add it to our architecture interface. We can describe the architecture interface as a concept (possibly implicit, possibly explicitly expressed as a C++ concept) implemented by each different architecture. The exact way to do it is still subject to multiple rethinks and iterations, due to the immature stage of development.

##### Platform management

We will deal with platform-dependent features in a similar way to how we deal with architecture-dependent features. However, we do not know yet whether the translation library will be directly platform-dependent in any way. Most often, platform-dependence will be indirect through the standard library (for example, when calling `operator new`). For this reason, the platform classes we will define by analogy with the architecture classes should better reside in our standard library tree, although also available to the translation library just in case.

##### Incremental development

Although the lack of project management prevents us from strictly adhering to any development model, we vaguely follow an incremental development strategy. This means we have a known goal that we intend to achieve by systematically taking all the necessary steps one by one. This is particularly the case for the `Translator` class.

The goal we intend to achieve is determined by the C++ standard. Initially, however, we will not implement syntactic features in the order they are introduced in the C++ standard, but we will divide features into three groups, each one with a particular order:

1. Common features: features we have explicitly planned in our roadmap. They will be implemented first, in the order they appear in the roadmap, unless this order is found inconvenient during development.
2. Self-build features: features that our C++ implementation must support to be able to build itself. They will be implemented in the order suggested by our failed attempts to build the C++ implementation.
3. Other standard features: additional features that need to be implemented to comply with the C++ standard. They will be implemented last, in the order they are presented in the C++ standard.

##### Minimalistic foundations

We plan to start with a minimalistic implementation that does nothing useful but is a first stage to start systematically adding support for each element of the C++ language. We only require a few things before we consider the foundations ready:

1. Minimalistic linker foundations.
    * It must be able to link our sample object files `start.o` and `main.o`. Support for more complex object files can be added later.
    * It must keep architecture-dependent code identified and isolated.
    * Code that depends on object file format must also be identified and isolated.
2. Minimalistic compiler foundations.
    * It must be able to compile at least a source file containing a main function with an empty body.
    * It must keep architecture-dependent code identified and isolated.
3. Minimalistic startup code.
    * It must call the main function and return to the environment.
    * It must be provided either as source code that our current translator can translate or as an object file.
    * If it is not provided as source code, some pseudocode must be provided equivalent to the object file.
4. Minimalistic test environment.
    * It must be able to execute any executable file generated by the 
    * It must show the exit code on exit.
