# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 7

The sequence of preprocessing tokens becomes a sequence of tokens, which is
translated. The result of the translation is a translated translation unit.

This phase contains most of the architecture dependence, when machine code is
generated. To keep portability to a maximum, our idea is to implement an
abstract code generator that implements most operations in an
architecture-independent way in terms of a few simple operations, which are the
only ones that will be architecture-dependent. Once the abstract code generator
is done, we only need to implement these few operations for the desired
architecture, so we expect to be able to develop in little time a minimal code
generator, which will probably be very slow but fully functional. Starting from
this point, which we may consider a negative optimisation level, we can go on
overriding architecture-independent operations with native code until we reach a reasonably fast optimisation level 0 and further optimisation levels.


    Abstract code generator ⟵ Minimal code generator ⟵ Level 0 code generator
                            ↖︎           ↑               ↗︎
                              Optimised code generators

