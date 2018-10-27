import platform
from conans import ConanFile, CMake

class DarknetcppConan(ConanFile):
    name = 'darknet-cpp'
    version = '0.1'
    license = '<Put the package license here>'
    url = '<Package recipe repository url here, for issues about the package>'
    description = '<Description of Darknetcpp here>'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False]}
    default_options = 'shared=True'
    generators = 'cmake'
    exports_sources = 'CMakeLists.txt', 'include/*', 'src/*', 'test/*'

    def build(self):
        cmake = CMake(self)
        # When building for cygwin and  on cygwin we need to remove CMAKE_SYSTEM_NAME (not cross compiling)
        if platform.system().startswith('CYGWIN'):
            del cmake.definitions['CMAKE_SYSTEM_NAME']

        # cmake.command_line is used to configure
        # cmake.build_config is used to build
        # check the content of these to see how cmake is called
        # cmake.verbose = True
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy('*.h', dst='include', src='hello')
        self.copy('*hello.lib', dst='lib', keep_path=False)
        self.copy('*.dll', dst='bin', keep_path=False)
        self.copy('*.so', dst='lib', keep_path=False)
        self.copy('*.dylib', dst='lib', keep_path=False)
        self.copy('*.a', dst='lib', keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ['darknet-cpp']

