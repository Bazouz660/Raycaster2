# Build tools
Conan https://conan.io/downloads
G++ compiler (I use MinGW on Windows)
CMake

# How to build

Create a conan profile if not already created.
Here is the one i use on Windows:
```
[settings]
    arch=x86_64
    os=Windows
    compiler=gcc
    compiler.version=13.2
    compiler.libcxx=libstdc++11
    compiler.cppstd=23
    compiler.exception=seh
    compiler.threads=posix
    build_type=Release

[conf]
tools.build:compiler_executables={ "c" : "gcc", "cpp": "g++" }
```

At the root of the project directory, run
`conan install . --build missing -s build_type=Release -of build`
This will install all dependencies like SFML and build it from source.
Then run either the `build_windows.bat` script on windows, or the `build_linux.sh` script on linux.

When the build is done, the binary will be copied to the root directory of the project.