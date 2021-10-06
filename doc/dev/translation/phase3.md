# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 3

The source file is decomposed into preprocessing tokens.

The first kinds of preprocessing tokens we plan to implement are `identifier`,
`pp-number` and `preprocessing-op-or-punc`, initially only to the extent
required by our early experiments.

Eventually `string-literal` tokens will also be implemented, including raw
strings. During phase 3, transformations performed inside raw string literals
during [phase 1](phase1.md) and [phase 2](phase2.md) must be reverted. What we
will rather do is to control phases 1 and 2 from the token parser in phase 3,
avoiding to perform those transformations inside raw string literals.

Each preprocessing token is defined by a string and a type. For diagnosis
purposes, each preprocessing token should contain at least information about its
source file, line number and column number.
