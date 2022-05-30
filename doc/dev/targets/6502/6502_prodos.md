
# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Target `6502_prodos`

### Supported hardware

The target `6502_prodos` is intended to support Apple II computers with either a 6502, 65C02 or 65816 CPU.

### Supported system software.

The target `6502_prodos` is intended to support Apple ProDOS 8 1.0.1 or later.

### Memory usage conventions for code and data.

#### Size and byte order

* Integer and pointer sizes are given by the convention C8-SIPE16-L32-LL64.
* All numbers are stored in memory following the little-endian convention.

#### Alignment

* All types have alignment 1.

### Calling conventions.

* 16 bytes in page zero are used as expanded registers, named r0-r25. Expanded registers r24-r25 combine to form register esp, which points to an expanded stack.
* The caller passes parameters first in register A, then in expanded registers r1-r7 (except r0-r1 may be used instead as the address of the returned value; see below), then on the expanded stack. 
* Types with size 1 will be returned in register A.
* Types with size greater than 1 and less than 9 will be returned in register A combined with expanded registers r1-r7 (register A holding the least significant byte).
* Types with size greater than 8 will be returned in a block of memory reserved by the caller in an address passed in expanded registers r0-r1.
* The callee must preserve expanded registers r16-r25.
* The callee does not need to preserve registers A, X nor Y, nor expanded registers r2-r15. Expanded registers r0 and r1 only need to be preserved when they hold the address of the returned value for types bigger than 8 bytes.

