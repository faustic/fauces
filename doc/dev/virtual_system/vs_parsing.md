# Fauces

## Virtual system version 1: machine code parsing and processing

Machine code parsing is a process that consists of separating a piece of machine
code into its most basic elements and forwarding each basic element to an
instruction parser. The instruction parser transforms the basic element into an
instruction, which is then set to an instruction processor. The process is
repeated by means of a parsing loop.

### Parsing loop

The parsing loop sends machine code elements or references to them to the
instruction parser. While it is more related to parsing than to processing, we
usually implement the parsing loop as a member of the instruction processor,
because each instruction processor usually has its own requirements about where
to begin, where to end and which instruction should be parsed next.

### Instruction parser

The instruction parser might be implemented as a class (more adequate for
complex instruction sets or when for whatever reason the parser needs to
maintain a state) or as a simple function, more adequate for simple instruction
sets.

Because our virtual system has simple, fixed-size instructions that can be
parsed independently of each other, we are implementing its instruction parser
as a simple function:

`void parse_instruction(Processor& processor, std::uint_least8_t instruction);`

### Instruction processor

An instruction processor receives an instruction and performs a corresponding
action. Our typical approach to instruction processing uses up to four kinds of
instruction processor:

* Disassembler: generates assembly language from machine code.
* Interpreter: maintains the state of the virtual machine and performs the
action of each instruction as it is parsed.
* Dynamic compiler: maintains the state of the virtual machine, translates
fragments of the virtual machine code into the host machine code as they are
needed and starts execution of the translated fragments until more translations
are needed.
* Predictor: tries to predict whether a certain instruction will have an
observable effect on the state of the virtual machine, usually to optimise a
dynamic compiler.

Because performance is not a priority for our virtual system, no predictor nor
dynamic compiler is currently planned. We are implementing the following
classes as instruction processors:

* `Processor`: a virtual class to be used as a base for the actual processors.
* `Disassembler`: the disassembler.
* `Cpu`: both an interpreter and a representation of the CPU of the virtual
machine.

### Processor

The `Processor` class defines the set of instruction that every instruction
processor of this virtual system should  implement. There is only one pure
virtual function: 

`virtual void unimplemented() = 0;`

By default, every other function calls the `unimplemented` function. Thus, a
derived class can be instantiated as long as it defines the `unimplemented`
function. This facilitates testing of an instruction processor before it is
fully implemented.

### Disassembler

The `Disassembler` class represents an object that keeps a printable list of the
instructions present in a certain range of memory.

### Cpu

The `Cpu` class represents an object capable of holding the state of the machine
and performing the actions of each instruction.

