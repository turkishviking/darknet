import platform
from conans import ConanFile, CMake

class DarknetcppConan(ConanFile):
    name = 'darknet-cpp'
    version = '0.1'
    license = '<Put the package license here>'
    url = '<Package recipe repository url here, for issues about the package>'
    description = '<Description of Darknetcpp here>'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {
        'shared': [True, False],
        'gpu': [True, False],
        'cudnn': [True, False],
    }
    default_options = {
        "shared": True,
        'gpu': True,
        'cudnn': True,
    }
    generators = 'cmake'
    exports_sources = 'CMakeLists.txt', 'include/*', 'src/*', 'test/*'

    def configure(self):
        if self.options.cudnn and not self.options.gpu:
            raise Exception("cuDNN support requires GPU support")

    def build(self):
        cmake = CMake(self)
        # When building for cygwin and  on cygwin we need to remove CMAKE_SYSTEM_NAME (not cross compiling)
        if platform.system().startswith('CYGWIN'):
            del cmake.definitions['CMAKE_SYSTEM_NAME']

        if self.options.gpu:
            cmake.definitions['USE_GPU'] = True

        if self.options.cudnn:
            cmake.definitions['USE_CUDNN'] = True
            
        if self.options.shared:
            cmake.definitions['BUILD_SHARED_LIBS'] = True

        # cmake.command_line is used to configure the project, you can
        # easily debug it using the following line:
        # print(*cmake.command_line.split(), sep="\n")
        
        # cmake.build_config is used to build (compile/link) the project

        # if you need verbose output from cmake, use this:
        # cmake.verbose = True

        cmake.configure()
        cmake.build()
        #cmake.test() //can't make test as builder has no cuda runtime binded from host
        #self.run("ctest . -VV")

    def package(self):
        self.copy('*.h', dst='include', src='include')
        self.copy('*hello.lib', dst='lib', keep_path=False)
        self.copy('*.dll', dst='bin', keep_path=False)
        self.copy('*.so', dst='lib', keep_path=False)
        self.copy('*.dylib', dst='lib', keep_path=False)
        self.copy('*.a', dst='lib', keep_path=False)
        self.copy('*.o', dst='lib', keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ['darknet-cpp']

