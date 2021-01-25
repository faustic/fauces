# Faustic Inferno's C++ implementation

## Reference targets

In the context of this project, a target is a set of choices for adapting our C++ implementation to a particular architecture and platform. We designate four targets as reference targets: one just virtual and the other three based on real CPUs:

* Virtual target
* 64-bit reference target
* 32-bit reference target
* 16-bit reference target

### Virtual target

The main goal of the virtual target is to provide a testing ground where we can try features as we add them to the compiler or standard library. Because our virtual CPU is very simple and easy to implement, we always have a virtual system ready, even if at the moment we are using a machine whose architecture we are not familiar with.

The idea is that we first implement and try things on the virtual target, and then, once they seem to work, we try them on real targets.

### Real reference targets

After we try something on the virtual target, the next step should normally be to try it on the real reference targets. By using several reference targets with different architectural characteristics, we intend to test portability before doing a lot of work on targets that behave too similarly.

At first we will designate three real reference targets. We identify them by number of bits of the void pointer.

### Reference target specification requirements.

A reference target may be specified more precisely or more loosely, but it should mention at least the following:

* Supported hardware.
* Supported system software.
* Memory usage conventions for code and data.
* Calling conventions.

The following will be assumed about choices that are not explicitly established:

* A `bool` value has 8 bits, `false` being represented by 0 and `true` by 1.
* A `char` value has 8 bits (C8).
* A `short` value has 16 bits (S16).
* An `int` value has 16 bits (I16).
* A `long` value has 32 bits (L32).
* A `long long` value has 64 bits (LL64).
* Integer types other than `bool` lack padding bits and trap representations.
* A `void *` value has 16 bits (P16).
* A `void (*)()` value has the same number of bits as a `void*` value, 16 bits if neither is specified (E16).
* The format used for floating point types will be the common one that will be defined for CPUs without floating point capabilities.
* Right-shifting negative integers will propagate the sign bit.

The default choice for integers and pointers can be abbreviated as:

C8-SIPE16-L32-LL64

Other choices can be expressed in an analogous way. Types that are the same size as the default can be omitted from the abbreviation. Particularly, E is usually omitted if it is the same size as P. For example,

C8-S16-IPE32-LLL64

can be further abbreviated as:

IP32-L64


