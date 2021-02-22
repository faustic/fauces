# Fauces

## Virtual target

We are defining a target for a virtual system of our own:

[Virtual system](../virtual_system/README.md)

Its purpose is to provide a testing ground where we can try features as we add them to the compiler or standard library.

In its first incarnation, we will call this target `visyv1_i16`.

### Choice explained

#### Hardware choice
 
We have decided to use a virtual CPU for our main reference target so we can have it available anywhere we go. The particular choices for the initial specification of the CPU programming model were made for the sake of easily and quickly implementing the virtual machine. The specification may be revised later if the choices prove too limiting.

#### Environment choice

The virtual environment is initially unspecified. Because its only purpose is to support the C++ implementation, it will grow as the implementation grows.

#### Implementation choices.

The only choice that requires comment is the size of `int` and pointer variables, which has been chosen as 16 bits (the minimum). This will help keep memory requirements low. However, because it also limits the size of programs, this choice may be revised at a later time if needed.

### Supported hardware

The required virtual hardware consists of the following:

* Virtual CPU as described [here](../virtual_system/vs_state.md) and [here](../virtual_system/vs_instructions.md).
* Code memory of at least 4096 bytes and no more than 65536 bytes.
* Data memory of at least 4096 bytes and no more than 65536 bytes.

The required real hardware has as its only requirement the ability to run an implementation of the required virtual hardware.

### Supported system software

The required virtual operating system consists of the following:

* Operating system for the virtual system as described [here](../virtual_system/vs_os.md)

The required host operating system has as its only requirement the ability to run an implementation of the required virtual operating system.

### Memory usage conventions for code and data

#### Size and byte order

* Integer and pointer sizes are given by the convention C8-SIPE16-L32-LL64.
* Floating point number sizes are as specified by the default [language library](../language_library/README.md).
* All numbers are stored in memory following the big-endian convention.

#### Alignment

* By default, all types have alignment 1.

#### Memory areas

* The code memory is fully dedicated to the executable code of the program.
* The data memory is divided into three parts:
    * Static area: allocated at lower addresses starting at zero up to the size automatically calculated at link time.
    * Stack area: allocated at higher addresses up to the maximum available data memory and taking a total size configurable at link time.
    * Heap area: the space remaining between the end of the static area and the beginning of the stack area.

### Calling conventions

* The callee must preserve the following registers: `S0`, `S1`.
* The caller will pass parameters on the stack, pushing the last parameter first. The first parameter (i.e. the last one pushed) will also be in register `R0`, that the callee does not need to preserve.
* If the return type is greater than 64 bits or a non-trivial class, then the caller will allocate space for it and will push its address onto the stack right after the first parameter (i.e. after the last parameter pushed onto the stack). The caller will also put this address in register R0 instead of the first parameter. In this case, the callee must preserve register R0.
* If the return type is a basic type with size less than 64 bits, or a trivial class with size less than 64 bits, then the callee will return its value in register `R0`.
* The callee does not need to preserve the following registers: `R1`, `X0`, `X1`.

