# Grpchello

Conan/CMake + grpc

## Build

```sh
cd build/
conan install .. -s build_type=Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug

cmake --build .

bin/grpchello
```


## Setup

```sh

pip install --user cmake conan # fastest way to install conan and cmake
export PATH=$HOME/.local/bin:$PATH # installed here (check ~/.profile)

conan new grpchello/0.1 -m cmake_exe

conan create . demo/testing -s build_type=Debug # recreates everything each time
```

## GRPC proto 

1. protoc needs  GRPC plugin  - conan has it as the cache
   1. cmake: use `find_program` to search for protoc and grpc plugin
   2. cmake: use `add_custom_command` to generenate protocolbuffer stabs


## Links

https://grpc.io/docs/languages/cpp/quickstart/
https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/CMakeLists.txt
https://github.com/PhantomGrazzler/example-grpc-conan-cmake/blob/master/src/client_hw/main.cpp