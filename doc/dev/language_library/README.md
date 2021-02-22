# Fauces

## Language library

Some of the operations appearing in a source file may be translated by the compiler as function calls made by the built program at runtime. In this project, we will call a library containing such functions a *language library*. In other places it is often called a *runtime library*, but this term can cause confusion because it is also sometimes used to refer to the C++ standard library.

Here we document the default language library. Some targets may use a different language library or only use a few parts of the default language library. For example, a target that lacks a floating point unit will probably use the language library to implement all floating point operations, while targets with a floating point unit may not use the language library at all or use it just for the operations not directly supported by the FPU.