from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class GlConan(ConanFile):                                    # Change.
    name            = "gl"                                   # Change.
    version         = "1.0.0"                                # Change.
    license         = "MIT"
    url             = "https://github.com/acdemiralp/gl"     # Change.
    description     = "Conan package for gl"                 # Change.
    requires        = "glew/2.1.0@RWTH-VR/thirdparty"        # Change.
    settings        = "os", "compiler", "build_type", "arch"
    options         = {"shared": [True, False]}              # Change (optional).
    default_options = "shared=True"                          # Change (optional).
    generators      = "cmake"

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)
        self.run ("cd %s-%s" % (self.name, self.version))

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        fixed_options  = "-DCUDA_INTEROP_SUPPORT=OFF -DBUILD_TESTS=OFF" # Change.
        self.run("cmake %s-%s %s %s %s" % (self.name, self.version, cmake.command_line, shared_options, fixed_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
        self.copy("*.a"  , dst="lib", keep_path=False)
        self.copy("*.so" , dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

    def package_info(self): # Change.
        self.cpp_info.libs = [self.name]
        glew_lib = ""
        # Linux.
        if self.settings.os == "Linux":
            glew_lib = "GLEW"
            if not self.options.shared:
                self.cpp_info.libs.append("GL")
        # MACOS.
        elif self.settings.os == "Macos":
            glew_lib = "GLEW"
            self.cpp_info.exelinkflags.append("-framework OpenGL")
        # Windows.
        else:
            glew_lib = "glew32"
            if not self.options.shared:
                self.cpp_info.defines.append("GL_STATIC"  )
                self.cpp_info.defines.append("GLEW_STATIC")

            if self.settings.compiler == "Visual Studio":
                if not self.options.shared:
                    glew_lib += "s"
                    self.cpp_info.libs.append("OpenGL32.lib")
                    if self.settings.compiler.runtime != "MT":
                        self.cpp_info.exelinkflags.append('-NODEFAULTLIB:LIBCMTD')
                        self.cpp_info.exelinkflags.append('-NODEFAULTLIB:LIBCMT' )
            else:
                self.cpp_info.libs.append("opengl32")

        if self.settings.build_type == "Debug":
            glew_lib += "d"
        self.cpp_info.libs.append(glew_lib)