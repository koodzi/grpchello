cmake_minimum_required(VERSION 3.15)
project(grpchello CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


# GRPC generate in cmake 
# 1 https://github.com/PhantomGrazzler/example-grpc-conan-cmake/blob/master/CMakeLists.txt
# 2 https://github.com/ZCube/conan-grpc/blob/master/grpc.cmake

find_program(_PROTOBUF_PROTOC NAMES protoc HINTS ${CONAN_BIN_DIRS_GRPC_RELEASE} ${CONAN_BIN_DIRS_GRPC_DEBUG} ${CONAN_BIN_DIRS_GRPC})
mark_as_advanced(_PROTOBUF_PROTOC)

find_program(_GRPC_CPP_PLUGIN NAMES grpc_cpp_plugin HINTS ${CONAN_BIN_DIRS_GRPC_RELEASE} ${CONAN_BIN_DIRS_GRPC_DEBUG} ${CONAN_BIN_DIRS_GRPC})
mark_as_advanced(_GRPC_CPP_PLUGIN)

set(GENERATED ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED})

set(HELLOWORLD_PROTOS
    ${GENERATED}/hello.pb.cc
    ${GENERATED}/hello.pb.h
    ${GENERATED}/hello.grpc.pb.cc
    ${GENERATED}/hello.grpc.pb.h
    )

add_custom_command(
    OUTPUT ${HELLOWORLD_PROTOS}
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out ${GENERATED}
         --cpp_out ${GENERATED}
         -I ${CMAKE_CURRENT_SOURCE_DIR}/src/proto
         --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN}
         hello.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/proto/hello.proto
    COMMENT "Invoking protoc to generate gRPC C++ files."
    )

add_executable(grpchello 
    src/main.cpp 
    src/proto/hello.proto
    ${GENERATED}/hello.pb.cc
    ${GENERATED}/hello.grpc.pb.cc
)
target_include_directories(grpchello 
    PUBLIC include 
    PRIVATE src/proto
    PRIVATE ${GENERATED}
)

target_link_libraries(grpchello 
    CONAN_PKG::poco 
    CONAN_PKG::grpc
)

install(TARGETS grpchello)
