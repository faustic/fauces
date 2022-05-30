
# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Target `i386_debian`

### Supported hardware

The target `i386_debian` is intended to support computers compatible with Debian for the 32-bit PC (i386) architecture.

### Supported system software.

The target `i386_debian` is intended to support at least the most recent major version of Debian.

### Memory usage conventions for code and data.

#### Size and byte order

* Integer and pointer sizes are given by the convention ILP32.
* All numbers are stored in memory following the little-endian convention.

#### Alignment

* The alignment of a fundamental type with size 4 or less is equal to its size.
* The alignment of a fundamental type with size greater than 4 is equal to 4.
* The alignment of a compound type is equal to the biggest alignment of its members.

### Calling conventions.

* The callee must preserve registers ebx, esi, edi, esp and ebp.
* The caller passes parameters on the stack, pushing the last parameter first.
* Integer types with size 4 or less will be returned in register EAX.
* Integer types with size 8 will be returned in registers EAX (low) and EDX (high).
* Compound types will be returned in a block of memory reserved by the caller in an address passed as if it were the first parameter. This address will also be returned in EAX.
* Floating point types will be returned as compound types when using the `--no-fpu` option, and in ST0 when using the `--fpu` option.
* The callee does not need to preserve registers eax, ecx and edx.

