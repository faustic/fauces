# Faustic Inferno's C++ implementation

## Virtual system CPU instructions

## Instruction codification

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


## Instruction list

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
|      *** Load register ***      |
|------------------------------------|
|   0x10         |      lb           |
|------------------------------------|
|   0x11         |      lh           |
|------------------------------------|
|   0x12         |      lw           |
|------------------------------------|
|   0x13         |      ld           |
|------------------------------------|
|   0x14         |      popb         |
|------------------------------------|
|   0x15         |      poph         |
|------------------------------------|
|   0x16         |      popw         |
|------------------------------------|
|   0x17         |      popd         |
|------------------------------------|
|   0x18         |      ls           |
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
|   0x20         |      stb          |
|------------------------------------|
|   0x21         |      sth          |
|------------------------------------|
|   0x22         |      stw          |
|------------------------------------|
|   0x23         |      std          |
|------------------------------------|
|   0x24         |      pushb        |
|------------------------------------|
|   0x25         |      pushh        |
|------------------------------------|
|   0x26         |      pushw        |
|------------------------------------|
|   0x27         |      pushd        |
|------------------------------------|
|   0x28         |      sts          |
|------------------------------------|
|   0x29         |      pushs        |
|------------------------------------|
|   0x2a         |      strr          |
|------------------------------------|
|   0x2b         |      strs          |
|------------------------------------|
|   0x2c         |      stsr          |
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

## Instruction description

We use a C-like syntax to describe the action of CPU instructions. An implicit
`++PC` is executed before the action of every instruction.

The content of a register is always interpreted as an unsigned 64-bit value.

The expression `X & PM` is always interpreted as a pointer to an 8-bit value,
where X can be any of R0, R1, S0, S1 or PC.

A 64-bit value is always assumed to be converted into an 8-bit value before
assigning it to a byte in memory, by truncating it to the least significant 8
bits.

An 8-bit value is always assumed to be converted into a 64-bit value before
using it in any operation, by using the eight original bits as the least
significant bits of the 64-bit value and setting the most significant bits as
zero.

### and

Assembly: `and Rs, Rd`

Code: `0b110000{sd}`

Action: performs a bitwise AND operation between Rs and Rd, and saves the result
in Rd.

```
Rd &= Rs;
```

### call

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

### jmp

Assembly: `jmp Rd`

Code: `0b000001{0d}`

Action: transfers control to the address specified in register Rd.

```
PC = Rd
```

### jmpnz

Assembly: `jmpnz Rs, Rd`

Code: `0b000101{sd}`

Action: if Rs is not zero, control is transferred  to the address specified in
register Rd.

```
if (Rs != 0)
    PC = Rd;
```

### jmpz

Assembly: `jmpz Rs, Rd`

Code: `0b000100{sd}`

Action: if Rs is zero, control is transferred  to the address specified in
register Rd.

```
if (Rs == 0)
    PC = Rd;
```

### lb

Assembly: `lb Rs, Rd`

Code: `0b010000{sd}`

Action: loads 8-bit value from address specified in register Rs into register
Rd, zero-extended to 64 bits.

```
Rd = *(Rs & PM);
```

### ld

Assembly: `ld Rs, Rd`

Code: `0b010011{sd}`

Action: loads 64-bit value from address specified in register Rs into register
Rd.

```
Rd = 0;
for (tmp_count = 0; tmp_count < 8; ++tmp_count)
{
    Rd <<= 8;
    Rd |= *((Rs + tmp_count) & PM);
}
```

### least

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

### lh

Assembly: `lh Rs, Rd`

Code: `0b010001{sd}`

Action: loads 16-bit value from address specified in register Rs into register
Rd, zero-extended to 64 bits.

```
Rd = (*(Rs & PM)) << 8;
Rd |= *((Rs + 1) & PM);
```

### lrr

Assembly: `lrr Rs, Rd`

Code: `0b011010{sd}`

Action: loads value of register Rs into register Rd.

```
Rd = Rs;
```

### lrr

Assembly: `lrs Rs, Sd`

Code: `0b011011{sd}`

Action: loads value of register Rs into register Sd.

```
Sd = Rs;
```

### ls

Assembly: `ls Rs, Sd`

Code: `0b011000{sd}`

Action: loads value from address specified in register Rs into register Sd. The
loaded value has 16 bits, 32 bits or 64 bits depending on the value in register
PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
Sd = 0;
for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
    Sd |= *((Rs + tmp_count) & PM) << ((tmp_size - 1 - tmp_count) * 8);
```

### lsr

Assembly: `lsr Ss, Rd`

Code: `0b011100{sd}`

Action: loads value of register Ss into register Rd.

```
Rd = Ss;
```

### lw

Assembly: `lw Rs, Rd`

Code: `0b010010{sd}`

Action: loads 32-bit value from address specified in register Rs into register
Rd, zero-extended to 64 bits.

```
Rd = 0;
for (tmp_count = 0; tmp_count < 4; ++tmp_count)
{
    Rd <<= 8;
    Rd |= *((Rs + tmp_count) & PM);
}
```

### not

Assembly: `not Rs, Rd`

Code: `0b110011{sd}`

Action: saves into Rd the one's complement of Rs.

```
Rd = ~Rs;
```

### or

Assembly: `or Rs, Rd`

Code: `0b110001{sd}`

Action: performs a bitwise OR operation between Rs and Rd, and saves the result
in Rd.

```
Rd |= Rs;
```

### popb

Assembly: `popb [Rd]`

Code: `0b010100{ud}`

Action: pops an 8-bit value from the stack.

```
if (u)
    Rd = *(S0 & PM);
++S0;
```

### popd

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

### poph

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

### pops

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

### popw

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

### pushb

Assembly: `pushb [Rs]`

Code: `0b100100{su}`

Action: pushes an 8-bit value onto the stack.

```
--S0;
if (u)
    *(S0 & PM) = Rs;
```

### pushd

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

### pushh

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

### pushs

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
       --S0;
        *(tmp_sp & PM) = Ss >> (tmp_count * 8);
    }
}
else
    S0 -= tmp_size;
```

### pushw

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

### ret

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

### shl

Assembly: `shl Rs, Rd`

Code: `0b110101{sd}`

Action: shifts the value in Rd one bit to the left the number of times specified
by the value in Rs modulo 63.

```
Rd <<= (Rs & 63);
```

### shr

Assembly: `shr Rs, Rd`

Code: `0b110110{sd}`

Action: shifts the value in Rd one bit to the right the number of times
specified by the value in Rs modulo 63.

```
Rd >>= (Rs & 63);
```

### sori

Assembly: `sori imme, Rd`

Code: `0b111{imme}{d}`

Action: shifts the value in register Rd 4 bits to the left and sets its four
rightmost bits to the bits in `imme`.

```
Rd = (Rd << 4) | imme;
```

### stb

Assembly: `stb Rs, Rd`

Code: `0b100000{sd}`

Action: stores 8-bit value from rightmost bits in register Rs into address
specified in register Rd.

```
*(Rd & PM) = Rs;
```

### std

Assembly: `std Rs, Rd`

Code: `0b100011{sd}`

Action: stores 64-bit in register Rs into address specified in register Rd.

```
for (tmp_count = 0; tmp_count < 8; ++tmp_count)
    *((Rd + tmp_count) & PM) = Rs >> (56 - tmp_count * 8);
```

### sth

Assembly: `sth Rs, Rd`

Code: `0b100001{sd}`

Action: stores 16-bit value from rightmost bits in register Rs into address
specified in register Rd.

```
(*(Rd & PM)) = Rs >> 8;
*((Rd + 1) & PM) = Rs;
```

### strr

Assembly: `strr Rs, Rd`

Code: `0b101010{sd}`

Action: stores 8-bit value from address specified in Rs into address specified
in Rd.

```
*(Rd & PM) = *(Rs & PM);
```

### strs

Assembly: `strs Rs, Sd`

Code: `0b101011{sd}`

Action: stores 8-bit value from address specified in Rs into address specified
in Sd.

```
*(Sd & PM) = *(Rs & PM);
```

### sts

Assembly: `sts Ss, Rd`

Code: `0b101000{sd}`

Action: stores address from register Ss into address specified in Sd.  The
stored value has 16 bits, 32 bits or 64 bits depending on the value in register
PM.

```
if (PM >> 32)
    tmp_size = 8;
else if (PM >> 16)
    tmp_size = 4;
else
    tmp_size = 2;
for (tmp_count = 0; tmp_count < tmp_size; ++tmp_count)
    *((Rd + tmp_count) & PM) =  Ss >> ((tmp_size - 1 - tmp_count) * 8);
```

### stsr

Assembly: `strr Ss, Rd`

Code: `0b101100{sd}`

Action: stores 8-bit value from address specified in Ss into address specified
in Rd.

```
*(Rd & PM) = *(Ss & PM);
```

### stw

Assembly: `stw Rs, Rd`

Code: `0b100010{sd}`

Action: stores 32-bit value from rightmost bits in register Rs into address
specified in register Rd.

```
for (tmp_count = 0; tmp_count < 4; ++tmp_count)
    *((Rd + tmp_count) & PM) = Rs >> (24 - tmp_count * 8);
```

### sys

Assembly: `sys Rs, Rd`

Code: `0b000000{sd}`

Action: stops instruction fetching until the environment sends the signal to
continue.

### xor

Assembly: `xor Rs, Rd`

Code: `0b110010{sd}`

Action: performs a bitwise XOR operation between Rs and Rd, and saves the result
in Rd.

```
Rd ^= Rs;
```


