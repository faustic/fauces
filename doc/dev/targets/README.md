# Faustic Inferno's C++ implementation

## Targets

In the context of this project, a target is a set of choices for adapting our C++ implementation to a particular architecture and platform.

We usually name a target by architecture and platform. Even if we only define one target for a particular architecture and platform, most often we also add other qualifiers to the target name, such as an *integer size string*.

### Integer size string

An integer size string is an abbreviated way to express the size of pointer and integer types. Rather than specifying it precisely we give a couple of examples of our usage.

The default choice for integers and pointers can be abbreviated as:

C8-SIPE16-L32-LL64

Which means:

    CHAR_BIT == 8                     /* C8 */
    sizeof(short)*CHAR_BIT == 16      /* S16 */
    sizeof(int)*CHAR_BIT == 16        /* I16 */
    sizeof(void*)*CHAR_BIT == 16      /* P16 */
    sizeof(void(*)())*CHAR_BIT == 16  /* E16 */
    sizeof(long)*CHAR_BIT == 32       /* L32 */
    sizeof(long long)*CHAR_BIT == 64  /* LL64 */

We often abbreviate the default as just I16.

Other choices can be expressed in an analogous way. Types that are the same size as the default can be omitted from the abbreviation. Particularly, E is usually omitted if it is the same size as P. For example,

C8-S16-IPE32-LLL64

can be further abbreviated as:

IP32-L64

### Target specification requirements.

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




