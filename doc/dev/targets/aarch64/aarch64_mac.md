
# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Target `aarch64_mac`

We will not go into much detail here. For more detailed information, we will rely on [documentation from Apple](https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms) and [documentaion from ARM](https://github.com/ARM-software/abi-aa).

### Supported hardware

The target `aarch64_mac` is intended to support all current Apple Mac systems with ARM CPU.

### Supported system software.

The target `aarch64_mac` is intended to support at least the two most recent major versions of macOS.

### Memory usage conventions for code and data.

#### Size and byte order

* Integer and pointer sizes are given by the convention I32-LP64.
* All numbers are stored in memory following the little-endian convention.

#### Alignment

* The alignment of a fundamental type is equal to its size.
* The alignment of a compound type is equal to the biggest alignment of its members.

### Calling conventions.

* The callee must preserve registers x19-x29 and SP.
* Register x18 is reserved by the platform and must not be used.
* The caller can pass parameters in registers x0-x7, in registers v0-v7 and on the stack, according to the rules stated in the [Procedure Call Standard](https://github.com/ARM-software/abi-aa/blob/main/aapcs64/aapcs64.rst).
* Depending on the result type, the result will be returned in registers x0-x7, in registers v0-v7 or in a block of memory reserved by the caller in an address passed in x8.
* The callee does not need to preserve registers x0-x17.

