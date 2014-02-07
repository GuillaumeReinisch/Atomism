Atomism
=======

Atomism is library that allows the study of molecular motion within 
a statistical formalism to compute thermodynamic data and reaction rates. 
In particular it focuses on the case of large amplitude
motions that can be described using arbitrary potential energy surfaces 
or molecular coordinates. Atomism also implements common model such as rigid
 rotors, harmonic oscillator, small amplitude anharmonic oscillators, and 
one dimensional quantum motions. 
Atomism is designed for thread safety and high performances (CPU/GPU
parallelisation).


Dependencies
============

Requirements
------------

Atomism needs:
1. cmake
2. a C++11 compiler
3. a recent version of [boost] (http://www.boost.org/)
4. the [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) library

For parallel computations, the following packages are required:
1. [vexCL](https://github.com/ddemidov/vexcl) : This package will enable GPU offload capabilities


Building Atomism
================

Impact uses cmake build system

1. cmake CMakeLists.txt. Warning: The first time you run this command, do it twice.
(for more options about build options do 'cmake -L')
2. make (note parallel builds are supported)
3. make install



