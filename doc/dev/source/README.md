# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Source code conventions

For now, we are not going into much detail concerning source code conventions. This document will be expanded on demand, when indecisions or inconsistencies reveal that a guideline is needed on a particular issue.

### Coding style

Generally we try to follow the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines), with some exceptions. Those exceptions are not explicitly stated yet. For now we trust new programmers to be be able to deduce any implicit conventions from the source code written before.

### File naming

Source files should be given concise names matching `[_a-z0-9]+`. A `.cpp` extension should be used for implementation files. An `.hpp` extension should be used for header files, except for those intended to be compatible with C programs, which should have an `.h` extension.

### Directory layout

As most things at this stage, directory layout is subject to change, but for now we propose something like this:

```
experimenting/
    ...
    bundle/
        library/
        startup/
        translator/
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
            <platform_n>/
    impl/
        common/
        platform/
            <platform_1>/
                arch/
                    <arch_1>/
                    ...
                    <arch_n>/ 
            ...
            <platform_n>/

refbundles/
    bundle_1/
        library/
        startup/
        translator/
    ...
    bundle_n/

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
            <arch_n>/
    impl/
        arch/
            <arch_1>/
                platform/
                    <platform_1>/
                    ...
                    <platform_n>/
            ...
            <arch_n>/
        common/

```

The project essentially consists of a translation library and a standard library. Everything else represents either experiments or particular instances of the C++ implementation. Project directories are organised with this in mind.

* `experimenting`. This is for anything we want to try. For now, we only reserve one name inside: `bundle`, for the C++ implementation instance where we usually try things first.
* `library`. The parameterised C++ Standard library. We are of the idea that standard library features will more likely be dependent on platform than on architecture. That's why we branch by platform first and only then by architecture. The `include` directory is only for the standard headers.  Internally used headers will be inside `impl` together with the implementation files.
* `refbundles`. C++ implementation instances for select architectures and platforms.
* `translation`. The C++ translation library. We believe that translation tasks will more likely be dependent on architecture than on platform. That's why here we branch by architecture first and only then by platform. The `include` directory is only for headers needed by users of the library.  Internally used headers will be inside `impl` together with the implementation files. The `impl` might not be present if we end implementing this as a header-only library.

All source code should be within one of those four trees, although there may be additional directories for other purposes, such as those for documentation.

