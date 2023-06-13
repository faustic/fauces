# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Source code conventions

For now, we are not going into much detail concerning source code conventions. This document will be expanded on demand, when indecisions or inconsistencies reveal that a guideline is needed on a particular issue.

### Coding style

Generally we try to follow the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines), with some exceptions. Those exceptions are not explicitly stated yet. For now we trust new programmers to be be able to deduce any implicit conventions from the source code written before.

### File naming

Source files should be given concise names matching `[_a-z0-9]+`. An `.hpp` extension should be used for header files. A `.cpp` extension should be used for implementation files.

### Directory layout

```
translation/
    include/
        arch/
            <arch_1>/
                include/
                platform/
                    <platform_1>/
                        include/
                    ...
                    <platform_n>/
                        include/
            ...

library/
    include/
        platform/
            <platform_1>/
                arch/
                    <arch_1>/
                    ...
                    <arch_n>/ 
            ...
    impl/
        common/
        platform/
            <platform_1>/
                arch/
                    <arch_1>/
                    ...
                    <arch_n>/ 

experimenting/
    ...
    bundle/
        startup/
        translator/
        library/
    ...

```
