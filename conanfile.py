from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class GrpcHelloConan(ConanFile):
    name = "grpchello"
    version = "0.1"

    # Optional metadata
    license = ""
    author = "kood@o2.pl"
    url = ""
    description = "Just a compilation of example programs, just to check how much does it cost to setup project"
    topics = ("grpc", "conan", "cmake")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"


    requires = (
        "poco/1.9.4", # idk
        "grpc/1.48.0" 
    )

    generators = "cmake"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

