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

Fauces expects physical source file characters to be encoded as UTF-8, with the
LF character (`\n`, encoded as `0x0a`) marking the end of line. Other encodings
or conventions may require [prior processing](phase0.md) to be performed before
feeding the source file to the translator. Prior processing may also be
necessary if trigraph replacement, which was part of the C++ standard in the
past, is expected.

Rather than converting UTF-8 codes to universal character names, our translator
will convert universal character names to UTF-8 codes, which is equivalent
because they will be subsequently handled as if they were the corresponding
universal character names.

Universal character name conversion must be reverted in [phase 3](phase3.md) if
it happened inside a raw string. Our translator will rather display the
equivalent behaviour of not doing such conversion during
phase 1 inside raw strings. This will be accomplished by executing phase 1 under control of the token parser in phase 3.
