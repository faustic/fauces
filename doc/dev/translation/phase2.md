# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 2

In the source file, every new line character after a backslash character (`\`)
must be deleted.

This deletion must be reverted in [phase 3](phase3.md) if it happened inside a
raw string. Our translator will rather display the equivalent behaviour of not
deleting the new line characters during phase 2 inside raw strings. This will be
accomplished by executing phase 2 under control of the token parser in phase 3.
 