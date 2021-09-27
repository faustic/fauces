# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase zero

Phase zero of translation does not exist in the C++ standard. It consists of the
steps necessary to convert a source file incompatible with the implementation
into a compatible one. Therefore, it is unnecessary when such incompatibility
is not present.

Incompatibilities with a source file can be due for example to some of the
following:

* The character encoding is not UTF-8.
* Characters other than LF (e.g. CR or sequence CR LF) are used as new line characters.
* Trigraphs are used with the expectation of trigraph replacement being supported, which is not the case with the C++ compiler in Fauces (trigraphs are no longer part of the C++ standard).

It is likely that the LF requirement will be softened at some point inside the
translator itself. In a few other cases, tools may be included as part of Fauces
to perform the necessary conversions before feeding the source files to the
translator. In other cases, such tools will need to be provided by others.
