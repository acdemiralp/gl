from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name            = "gl"
    description     = "Conan package for gl."
    version         = "1.1.4"                
    url             = "https://github.com/acdemiralp/gl"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("catch2/2.2.0@bincrafters/stable"), 
                       ("glew/2.1.0_3@RWTH-VR/thirdparty"))
    options         = {"shared": [True, False]}
    default_options = "shared=True", "glew:shared=True"

    def imports(self):
       self.copy("*.dylib*", dst="", src="lib")
       self.copy("*.dll"   , dst="", src="bin")

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        fixed_options  = "-DCUDA_INTEROP_SUPPORT=OFF -DBUILD_TESTS=OFF"
        self.run("cmake %s-%s %s %s %s" % (self.name, self.version, cmake.command_line, shared_options, fixed_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.defines.append("%s_STATIC" % self.name.upper())
