import os
import platform

from conans import ConanFile, CMake, tools


class DarknetcppTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        # When building for cygwin and  on cygwin we need to remove CMAKE_SYSTEM_NAME (not cross compiling)
        if platform.system().startswith("CYGWIN"):
            del cmake.definitions["CMAKE_SYSTEM_NAME"]

        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        cmake = CMake(self)
        # When building for cygwin and  on cygwin we need to remove CMAKE_SYSTEM_NAME (not cross compiling)
        if platform.system().startswith("CYGWIN"):
            del cmake.definitions["CMAKE_SYSTEM_NAME"]

        cmake.test()
