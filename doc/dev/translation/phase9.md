# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Phases of translation

### Phase 9

The **instantiation units** generated in phase 8 plus any required library components are combined and converted into a **program image**.

Our translator uses two representations of the program image:

1. A representation in memory, that is more abstract and not particularly attached to any executable formats.
2. The final representation as an executable file.

### Executable file

The format of the executable file is normally determined by the operating system where its execution is intended. If no executable format is defined for a certain target or if the defined formats are inconvenient to use, we will provide executable formats of our own. For now we are only defining the following formats:

1. [Fo16](fo16.md)
