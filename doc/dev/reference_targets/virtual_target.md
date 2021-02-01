# Faustic Inferno's C++ implementation

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

The required real hardware has as its only requirement the ability to run an implementation of the required virtual hardwarre.

### Supported system software

The required virtual operating system consists of the following:

* Operating system for the virtual system as described [here](../virtual_system/vs_os.md)

The required host operating system has as its only requirement the ability to run an implementation of the required virtual operating system.

### Memory usage conventions for code and data

### Calling conventions

