# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## 32-bit reference target

Our 64-bit reference target will be [`i386_debian`](../targets/i386/i386_debian.md), i.e. Debian on i386 (IA-32).

### Choice explained

We use the following criteria to choose reference targets:

* Convenience: we like to target systems we have at hand. Currently, the chosen target is the 32-bit target that best meets this requirement.
* Comparability: whenever possible, we prefer that our reference targets are widely supported by modern C++ implementations other than ours, so we can easily test compatibility and relative performance. The chosen target is supported at least by clang and gcc.

