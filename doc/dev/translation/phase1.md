# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 1

If possible, physical source file characters are mapped to the basic source
character set; those that cannot be mapped are replaced by the corresponding
universal character names (or by any other internal representations as long as
they are handled the same way as the corresponding universal character
names).

#### New line

Fauces expects physical source file characters to be encoded as UTF-8, with the
LF character (`\n`, encoded as `0x0a`) marking the end of line. Other encodings
or conventions may require [prior processing](phase0.md) to be performed before
feeding the source file to the translator.

#### Trigraphs

Trigraph translation was part of the C++ standard in the past, but it is no
longer required. Implementing it is not against the standard, as it may be
deemed just a part of the mapping to the basic source character set, but this
will not be a feature in our C++ implementation. If needed, it can be supported
through [prior processing](phase0.md) of the source file.

#### Universal character names

Rather than converting UTF-8 codes to universal character names, our translator
will convert universal character names to UTF-8 codes, which is equivalent
because they will be subsequently handled as if they were the corresponding
universal character names. More precisely, the situations where handling might
be different are undefined behaviour anyway:

* Undefined behaviour: universal character names formed after splicing lines.
* Undefined behaviour: universal character names formed after token concatenation.

#### Raw strings

Universal character name conversion must be reverted in [phase 3](phase3.md) if
it happened inside a raw string literal. Our translator will rather display the
equivalent behaviour of not doing such conversion during phase 1 inside raw
string literals. This will be accomplished by executing phase 1 under control of
the token parser in phase 3.

While string literals are not implemented, mapping will be done with a simple
`readline` function with no awareness of C++ syntax. At some point, it is
planned to make `readline` aware of string literals and raw string literals,
which do not deal with universal character names the same way. A `readraw`
function should be implemented when support for raw string literals is added.
