CPPRay
====

a Minimalistic pathtracer written in C++, inspiration taken from projects like smallpt.

![alt tag](https://raw.githubusercontent.com/Harha/CPPRay/master/cppray.png)

Performance differences between different compilers:
====
![alt tag](https://raw.githubusercontent.com/Harha/CPPRay/master/linuxbench.png)
Linux build: GNU GCC Compiler, All possible optimizations, -O3, x64

![alt tag](https://raw.githubusercontent.com/Harha/CPPRay/master/winbenchtdm.png)
Windows build: TDM GCC Compiler, All possible optimizations, -O3, x64

![alt tag](https://raw.githubusercontent.com/Harha/CPPRay/master/winbenchvc++.png)
Windows build: Visual C++ Compiler, All possible optimizations, -O3, x64

Configuration used while testing:

WIDTH 128

HEIGHT 128

SCALE 2

RECURSION_MAX 5

MAXDISTANCE 128

SUPERSAMPLINGLEVEL 0
