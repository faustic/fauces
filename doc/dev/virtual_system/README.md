# Fauces

## Virtual system version 1

The virtual system is made of a very simple CPU and an environment. Its main
goal is to provide an easy to implement architecture where compiler features
can be tested before porting them to real architectures. Its design principles
should be **simplicity** and **flexibility** rather than performance.

The CPU consists of a few registers and a connection to some memory provided by
the environment. Both registers and memory can be seen and changed by the
environment while the CPU is paused.

The environment represents what devices and operating system would represent in
a real system. Also, if we want services that are typical of most real
microprocessors but are not present in our virtual CPU, such as exception
handling, they should be provided by the environment.

* [CPU state](vs_state.md)
* [CPU instruction set](vs_instructions.md)
* [Instruction parsing and processing](vs_parsing.md)
* [Operating system](vs_os.md)
