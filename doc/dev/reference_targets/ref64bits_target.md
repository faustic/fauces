# Fauces

*Copyright © Faustic Inferno SL*

*Licensed under the MIT License*

## 64-bit reference target

Our 64-bit reference target will be [`aarch64_mac`](../targets/aarch64/aarch64_mac.md), i.e. macOS on 64-bit ARM.

### Choice explained

We use the following criteria to choose reference targets:

* Convenience: we like to target systems that we have at hand. Currently, the chosen target is the 64-bit target that best meets this requirement.
* Comparability: whenever possible, we prefer that our reference targets are widely supported by modern C++ implementations other than ours, so we can easily test compatibility and relative performance. The chosen target is supported at least by clang, so it fulfills this requirement. However, gcc support for ARM macOS is still experimental, so the following targets would satisfy it better:
    * Debian on 64-bit x86 or 64-bit ARM.
    * Windows on 64-bit x86.

Weighing both criteria, we ended deciding that comparing with clang will be enough for now, so convenience wins. This may change if circumstances evolve in a way that makes the chosen target less convenient.

