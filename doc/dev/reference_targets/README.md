# Fauces

## Reference targets

In the context of this project, a target is a set of choices for adapting our C++ implementation to a particular architecture and platform. We designate four targets as reference targets: one just virtual and the other three based on real CPUs:

* [Virtual target](virtual_target.md)
* 64-bit reference target
* 32-bit reference target
* 16-bit reference target

### Virtual target

The main goal of the virtual target is to provide a testing ground where we can try features as we add them to the compiler or standard library. Because our virtual CPU is very simple and easy to implement, we always have a virtual system ready, even if at the moment we are using a machine whose architecture we are not familiar with.

The idea is that we first implement and try things on the virtual target, and then, once they seem to work, we try them on real targets.

### Real reference targets

After we try something on the virtual target, the next step should normally be to try it on the real reference targets. By using several reference targets with different architectural characteristics, we intend to test portability before doing a lot of work on targets that behave too similarly.

At first we will designate three real reference targets. We identify them by number of bits of the void pointer.

