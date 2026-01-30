# 65C02 emulator

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## Implementation for Java

Our emulated CPU has been made to run in its own thread. This way we can keep a better separation between the CPU and other devices we may need to emulate.

* **Test_machine**: program to emulate a 65C02-based machine.
* **com.faustic.cpu.wdc65x.Assembler**: the same assembler used for test generation. Here we use it to generate minimal system software on the fly as we start the emulator.
* **com.faustic.cpu.wdc65x.Executable**: a class to load and save executables. Here we use it to load the test programs.
* **com.faustic.memory.\***: abstract interface to read and write emulated memory, plus some concrete implementations.
* **com.faustic.cpu.wdc65x.Parser**: a class to parse 65C02 and 65C816 machine code.
* **com.faustic.cpu.wdc65x.Processor**: an interface to process 65C02 machine instructions (usually after having been parsed by a Parser object)
* **com.faustic.cpu.wdc65x.Disassembler**: a Processor implementation to print 65C02 instructions.
* **com.faustic.cpu.wdc65x.Cpu65c02**: a Processor implementation to execute 65C02 instructions
* **com.faustic.cpu.wdc65x.test.\***: classes to generate simple 65C02 programs to test the emulator.
