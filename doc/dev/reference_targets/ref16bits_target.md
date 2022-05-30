# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## 16-bit reference target

Our 16-bit reference target will be [`6502_prodos`](../targets/6502/6502_prodos.md), i.e. Apple ProDOS 8 on 6502 CPU. This is an 8-bit CPU with a 16-bit address space (when we talk of bits of a reference target we are talking about void pointer size).

### Choice explained

We use the following criteria to choose reference targets:

* Convenience: we like to target systems we have at hand. Currently, the chosen target is the 16-bit target that best meets this requirement.
* Comparability: we do not expect 16-bit targets to be widely supported by modern C++ implementations, so the criterion of comparability bears little weight in this case.

