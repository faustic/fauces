# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Virtual system version 1: CPU instructions

### Application of proposed design principles

* Simplicity: few instructions; no built-in arithmetic instructions, only
bitwise operations; no built-in signed integer format; fixed-length
instructions; separate load, store and operation instructions.
* Flexibility: pointer size adapted to memory size; derivatives with a different
operand size are easy to design.

### Instruction codification

Every instruction takes exactly eight bits, forming one byte. Six bits
represent the opcode, one bit represents the source register and one bit
represents the destination register. Instructions that do not take two
parameters may ignore the corresponding unused bits or require fixed values
for them or use them to define instruction variants.

```
|------------------------------------------------|
|                 OPCODE             | SRC | DST |
|------------------------------------------------|
```

As an exception, the `sori` instruction, does not expect a source register,
but a 4-bit immediate value overlapping with the opcode.

```
|------------------------------------------------|
|                                    |     |     |
|           OPCODE |------------------ IMM | DST |
|                  |                       |     |
|------------------------------------------------|
```


### Instruction list

```
|------------------------------------|
|   Opcode       |      Instruction  |
|------------------------------------|
|       *** Flow control ***         |
|------------------------------------|
|   0x00         |      sys          |
|------------------------------------|
|   0x01         |      jmp          |
|------------------------------------|
|   0x02         |      call         |
|------------------------------------|
|   0x03         |      ret          |
|------------------------------------|
|   0x04         |      jmpz         |
|------------------------------------|
|   0x05         |      jmpnz        |
|------------------------------------|
|      *** Load register ***         |
|------------------------------------|
|   0x10         |      lmb          |
|------------------------------------|
|   0x11         |      lmh          |
|------------------------------------|
|   0x12         |      lmw          |
|------------------------------------|
|   0x13         |      lmd          |
|------------------------------------|
|   0x14         |      popb         |
|------------------------------------|
|   0x15         |      poph         |
|------------------------------------|
|   0x16         |      popw         |
|------------------------------------|
|   0x17         |      popd         |
|------------------------------------|
|   0x18         |      lrx          |
|------------------------------------|
|   0x19         |      pops         |
|------------------------------------|
|   0x1a         |      lrr          |
|------------------------------------|
|   0x1b         |      lrs          |
|------------------------------------|
|   0x1c         |      lsr          |
|------------------------------------|
|     *** Store into memory ***      |
|------------------------------------|
|   0x20         |      stmb         |
|------------------------------------|
|   0x21         |      stmh         |
|------------------------------------|
|   0x22         |      stmw         |
|------------------------------------|
|   0x23         |      stmd         |
|------------------------------------|
|   0x24         |      pushb        |
|------------------------------------|
|   0x25         |      pushh        |
|------------------------------------|
|   0x26         |      pushw        |
|------------------------------------|
|   0x27         |      pushd        |
|------------------------------------|
|   0x29         |      pushs        |
|------------------------------------|
|   0x2a         |      strr         |
|------------------------------------|
|   0x2b         |      strs         |
|------------------------------------|
|   0x2c         |      stsr         |
|------------------------------------|
|      *** Change register ***       |
|------------------------------------|
|   0x30         |      and          |
|------------------------------------|
|   0x31         |      or           |
|------------------------------------|
|   0x32         |      xor          |
|------------------------------------|
|   0x33         |      not          |
|------------------------------------|
|   0x34         |      least        |
|------------------------------------|
|   0x35         |      shl          |
|------------------------------------|
|   0x36         |      shr          |
|------------------------------------|
|   0x38-0x3f    |      sori         |
|------------------------------------|
```

### Instruction description

We use a C-like syntax to describe the action of CPU instructions. An implicit
`++PC` is executed before the action of every instruction.

The content of a register is always interpreted as an unsigned 64-bit value.

The expression `A & PM` (where A can be any of R0, R1, S0, S1, X0, X1, PC or the
sum of two of them) is always interpreted as a pointer to an 8-bit value.

A 64-bit value is always assumed to be converted into an 8-bit value before
assigning it to a byte in memory, by truncating it to the least significant 8
bits.

An 8-bit value is always assumed to be converted into a 64-bit value before
using it in any operation, by using the eight original bits as the least
significant bits of the 64-bit value and setting the most significant bits as
zero.

#### and

Assembly: `and Rs, Rd`

Code: `0b110000{sd}`

Action: performs a bitwise AND operation between Rs and Rd, and saves the result
in Rd.

```
Rd &= Rs;
```

#### call

Assembly: `call Rd`

Code: `0b000010{0d}`

Action: pushes the current program counter (i.e. the address of the next
instruction) onto the stack and transfers control to the address specified in
register Rd.  The pushed value has 16 bits, 32 bits or 64 bits depending on the
value in register PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
{
    --S0;
    *(S0 & PM) = PC
    PC >>= 8;
}
PC = Rd;
```

#### jmp

Assembly: `jmp Rd`

Code: `0b000001{0d}`

Action: transfers control to the address specified in register Rd.

```
PC = Rd
```

#### jmpnz

Assembly: `jmpnz Rs, Rd`

Code: `0b000101{sd}`

Action: if Rs is not zero, control is transferred  to the address specified in
register Rd.

```
if (Rs != 0)
    PC = Rd;
```

#### jmpz

Assembly: `jmpz Rs, Rd`

Code: `0b000100{sd}`

Action: if Rs is zero, control is transferred  to the address specified in
register Rd.

```
if (Rs == 0)
    PC = Rd;
```

#### least

Assembly: `least Rs, Rd`

Code: `0b110100{sd}`

Action: loads 0, 1 or 2 into Rd, depending on whether Rs is equal, less or
greater than Rd, respectively.

```
if (Rs == Rd)
    Rd = 0;
else if ( Rs < Rd)
    Rd = 1;
else
    Rd = 2;
```

#### lmb

Assembly: `lmb Rs, Rd`

Code: `0b010000{sd}`

Action: loads 8-bit value from address specified as the sum of Rs and Xs into
register Rd, zero-extended to 64 bits.

```
Rd = *((Rs + Xs) & PM);
```

#### lmd

Assembly: `lmd Rs, Rd`

Code: `0b010011{sd}`

Action: loads 64-bit value from address specified as the sum of Rs and Xs into
register Rd.

```
Rd = 0;
for (tmp_count = 0; tmp_count < 8; ++tmp_count)
{
    Rd <<= 8;
    Rd |= *((Rs + Xs + tmp_count) & PM);
}
```

#### lmh

Assembly: `lmh Rs, Rd`

Code: `0b010001{sd}`

Action: loads 16-bit value from address specified as the sum of Rs and Xs into
register Rd, zero-extended to 64 bits.

```
Rd = (*((Rs + Xs) & PM)) << 8;
Rd |= *((Rs + Xs + 1) & PM);
```

#### lmw

Assembly: `lmw Rs, Rd`

Code: `0b010010{sd}`

Action: loads 32-bit value from address specified as the sum of Rs and Xs into
register Rd, zero-extended to 64 bits.

```
Rd = 0;
for (tmp_count = 0; tmp_count < 4; ++tmp_count)
{
    Rd <<= 8;
    Rd |= *((Rs + Xs + tmp_count) & PM);
}
```

#### lrr

Assembly: `lrr Rs, Rd`

Code: `0b011010{sd}`

Action: loads value of register Rs into register Rd.

```
Rd = Rs;
```

#### lrs

Assembly: `lrs Rs, Sd`

Code: `0b011011{sd}`

Action: loads value of register Rs into register Sd.

```
Sd = Rs;
```

#### lrx

Assembly: `lrx Rs, Xd`

Code: `0b011000{sd}`

Action: loads value of register Rs into register Xd.

```
Xd = Rs
```

#### lsr

Assembly: `lsr Ss, Rd`

Code: `0b011100{sd}`

Action: loads value of register Ss into register Rd.

```
Rd = Ss;
```

#### not

Assembly: `not Rs, Rd`

Code: `0b110011{sd}`

Action: saves into Rd the one's complement of Rs.

```
Rd = ~Rs;
```

#### or

Assembly: `or Rs, Rd`

Code: `0b110001{sd}`

Action: performs a bitwise OR operation between Rs and Rd, and saves the result
in Rd.

```
Rd |= Rs;
```

#### popb

Assembly: `popb [Rd]`

Code: `0b010100{ud}`

Action: pops an 8-bit value from the stack.

```
if (u)
    Rd = *(S0 & PM);
++S0;
```

#### popd

Assembly: `popd [Rd]`

Code: `0b010111{ud}`

Action: pops a 64-bit value from the stack.

```
if (u)
{
    Rd = 0;
    for (tmp_count = 0; tmp_count < 8; ++tmp_count)
    {
        Rd <<= 8;
        Rd |= *(S0 & PM);
        ++S0;
    }
}
else
    S0 += 8;
```

#### poph

Assembly: `poph [Rd]`

Code: `0b010101{ud}`

Action: pops a 16-bit value from the stack.

```
if (u)
{
    Rd = (*(S0 & PM)) << 8;
    ++S0;
    Rd |= *(S0 & PM);
    ++S0;
}
else
    S0 += 2;
```

#### pops

Assembly: `pops [Sd]`

Code: `0b011001{ud}`

Action: pops an address from the stack. The popped value has 16 bits, 32 bits
or 64 bits depending on the value in register PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
if (u)
{    
    Sd = 0;
    tmp_sp = S0;
    for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
    {
        Sd <<= 8;
        Sd |= *(tmp_sp & PM);
       ++tmp_sp;
    }
    if (d != 0)
        S0 = tmp_sp;
}
else
    S0 += tmp_size;
```

#### popw

Assembly: `popw [Rd]`

Code: `0b010110{ud}`

Action: pops a 32-bit value from the stack.

```
if (u)
{
    Rd = 0;
    for (tmp_count = 0; tmp_count < 4; ++tmp_count)
    {
        Rd <<= 8;
        Rd |= *(S0 & PM);
        ++S0;
    }
}
else
    S0 += 4;
```

#### pushb

Assembly: `pushb [Rs]`

Code: `0b100100{su}`

Action: pushes an 8-bit value onto the stack.

```
--S0;
if (u)
    *(S0 & PM) = Rs;
```

#### pushd

Assembly: `pushd [Rs]`

Code: `0b100111{su}`

Action: pushes a 64-bit value onto the stack.

```
if (u)
{
    for (tmp_count = 0; tmp_count < 8; ++tmp_count)
    {
        --S0
        *(S0 & PM) = Rd >> (tmp_count * 8);
    }
}
else
    S0 -= 8;
```

#### pushh

Assembly: `pushh [Rs]`

Code: `0b100101{su}`

Action: pushes a 16-bit value onto the stack.

```
if (u)
{
    --S0
    (*(S0 & PM)) = Rs;
    --S0;
    *(S0 & PM) = Rs >> 8;
}
else
    S0 -= 2;
```

#### pushs

Assembly: `pushs [Ss]`

Code: `0b101001{su}`

Action: pushes an address onto the stack. The pushed value has 16 bits, 32 bits
or 64 bits depending on the value in register PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
if (u)
{    
    tmp_sp = S0;
    for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
    {
       --tmp_sp;
        *(tmp_sp & PM) = Ss >> (tmp_count * 8);
    }
    S0 = tmp_sp;
}
else
    S0 -= tmp_size;
```

#### pushw

Assembly: `pushw [Rs]`

Code: `0b100110{su}`

Action: pushes a 32-bit value onto the stack.

```
if (u)
{
    for (tmp_count = 0; tmp_count < 4; ++tmp_count)
    {
        --S0
        *(S0 & PM) = Rd >> (tmp_count * 8);
    }
}
else
    S0 -= 4;
```

#### ret

Assembly: `ret`

Code: `0b000011{00}`

Action: pops an address from the stack and transfers control to it. The popped
value has 16 bits, 32 bits or 64 bits depending on the value in register PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
PC = 0;
for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
{
    PC <<= 8;
    PC |= *(S0 & PM);
   ++S0;
}
```

#### shl

Assembly: `shl Rs, Rd`

Code: `0b110101{sd}`

Action: shifts the value in Rd one bit to the left the number of times specified
by the value in Rs modulo 63.

```
Rd <<= (Rs & 63);
```

#### shr

Assembly: `shr Rs, Rd`

Code: `0b110110{sd}`

Action: shifts the value in Rd one bit to the right the number of times
specified by the value in Rs modulo 63.

```
Rd >>= (Rs & 63);
```

#### sori

Assembly: `sori imme, Rd`

Code: `0b111{imme}{d}`

Action: shifts the value in register Rd four positions to the left and sets its
four rightmost bits to the bits in `imme`.

```
Rd = (Rd << 4) | imme;
```

#### stmb

Assembly: `stmb Rs, Rd`

Code: `0b100000{sd}`

Action: stores 8-bit value from rightmost bits in register Rs into address
specified as the sum of Rd and Xd.

```
*((Rd + Xd) & PM) = Rs;
```

#### stmd

Assembly: `stmd Rs, Rd`

Code: `0b100011{sd}`

Action: stores 64-bit in register Rs into address specified as the sum of Rd and
Xd.

```
for (tmp_count = 0; tmp_count < 8; ++tmp_count)
    *((Rd + Xd + tmp_count) & PM) = Rs >> (56 - tmp_count * 8);
```

#### stmh

Assembly: `stmh Rs, Rd`

Code: `0b100001{sd}`

Action: stores 16-bit value from rightmost bits in register Rs into address
specified as the sum of Rd and Xd.

```
*((Rd + Xd) & PM) = Rs >> 8;
*((Rd + Xd + 1) & PM) = Rs;
```

#### stmw

Assembly: `stmw Rs, Rd`

Code: `0b100010{sd}`

Action: stores 32-bit value from rightmost bits in register Rs into address
specified as the sum of Rd and Xd.

```
for (tmp_count = 0; tmp_count < 4; ++tmp_count)
    *((Rd + Xd + tmp_count) & PM) = Rs >> (24 - tmp_count * 8);
```

#### strr

Assembly: `strr Rs, Rd`

Code: `0b101010{sd}`

Action: stores 8-bit value from address specified as the sum of Rs and Xs into
address specified as the sum of Rd and Xd.

```
*((Rd + Xd) & PM) = *((Rs + Xs) & PM);
```

#### strs

Assembly: `strs Rs, Sd`

Code: `0b101011{sd}`

Action: stores 8-bit value from address specified as the sum of Rs and Xs into
address specified as the sum of Sd and Xd.

```
*((Sd + Xd) & PM) = *((Rs + Xs) & PM);
```

#### stsr

Assembly: `strr Ss, Rd`

Code: `0b101100{sd}`

Action: stores 8-bit value from address specified as the sum of Ss and Xs into
address specified as the sum of Rd and Xd.

```
*((Rd + Xd) & PM) = *((Ss + Xd) & PM);
```

#### sys

Assembly: `sys Rs, Rd`

Code: `0b000000{sd}`

Action: stops instruction fetching until the environment sends the signal to
continue.

#### xor

Assembly: `xor Rs, Rd`

Code: `0b110010{sd}`

Action: performs a bitwise XOR operation between Rs and Rd, and saves the result
in Rd.

```
Rd ^= Rs;
```


