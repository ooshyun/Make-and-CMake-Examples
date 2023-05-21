# Example of Make, Makefile and CMakeLists.txt 

This file contains an example of Make, Makefile and CMakeLists.txt, and the original purpose is evaluating the vanilia and custom matrix muliplication.

It can be used makefile or CMake in vscode

```bash
make
```

The another makefile example is https://github.com/ooshyun/makefile_sample


## Examples
### 1. Use Configuration for CMake
- Go to [files](https://github.com/ooshyun/Example-Make/tree/master/examples/1)!
- This example is for using configuration in CMakeLists.txt
- It includes bash file for building library and program, and running

```bash
.
├── Assignment
│   ├── Assignment
│   │   ├── Config.cmake
│   │   └── main.cpp
│   ├── AssignmentDLL
│   │   ├── CAssigment.cpp
│   │   ├── Config.cmake
│   │   └── lib
│   │       ├── libsndfile.dylib
│   │       └── libsndfile_x86_64.dylib
│   ├── Assignment_Console
│   │   ├── Config.cmake
│   │   └── main.cpp
│   └── CMakelists.txt
├── CMakelists.txt
├── build
├── build.sh
└── run.sh
```


Reference
----------
- Makefile/CMakeLists.txt: https://github.com/doches/progressbar
- CMSIS-DSP: https://github.com/ARM-software/CMSIS-DSP
- [https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html](https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html)
- [https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c)