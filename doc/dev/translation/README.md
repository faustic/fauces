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
* `Instantiantion_unit`. For now, it is just an alias for `Translated_unit`. Depending on how we implement template instantiation, we may need to differentiate both classes. Until then, we will use both terms interchangeably.
* `Translated_unit_loader`. An abstract class, whose derived classes must implement the `load()` function, which returns an object of type `Translated_unit`. When an input file is added to the supply, it is determined at run time which derived class must be used to load the `Translated_unit`, depending on the type of the file.
* `Fo16_unit_loader`: A class derived from `Translated_unit_loader` to load a Fo16 object file as a `Translated_unit`. This was the first object format we supported. Every other object format we support must have its corresponding `Translated_unit_loader` derived class.
* `Translator`: A class derived from `Translated_unit_loader` to load a C++ source file as a `Translated_unit` (by translating it).

### The `Translator` class

Most of the translation work happens inside the `Translator` class. When the translation library becomes aware that a C++ source file is being added to the supply, an object of class `Translator` is created and its `load()` member function is called. This call performs phases 1 to 8 of the translation process.

#### Initial implementation

* `preprocess`
    * `pretokenize`
    * `execute_directives`
* `convert_literals`
* `concatenate_literals`
* `analyze`
* `instantiate`

#### Aims

* Architecture management
* Platform management
* Incremental development
