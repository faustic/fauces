# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 5

Characters in character literals and string literals are converted to the
execution character set.

In our early experiments, we will assume that the execution character set is
always UTF-8. Because our implementation of phase 1 already converted universal
character names to UTF-8 codes, this phase will act like a no-op.

