import os
import platform
from conans import ConanFile, CMake

is_windows = platform.system() == 'Windows'

def shared():
    return is_windows


class Main(ConanFile):
    name = "main"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    default_options = {"boost:shared": shared(),
                       "gtest:shared": shared(),
                       "glfw:shared": shared(),
                       "glew:shared": shared(),
                       "glad:shared": shared()}

    def config_options(self):
        self.options["boost"].shared = shared()
        self.options["gtest"].shared = shared()
        self.options["glfw"].shared = shared()
        self.options["glew"].shared = shared()
        self.options["glew"].shared = shared()
        self.options["glad"].shared = shared()

    def requirements(self):
        self.requires("boost/1.79.0")
        self.requires("gtest/cci.20210126")
        self.add_opengl()

    def imports(self):
        parent = os.path.join(self.install_folder, os.pardir)    
        target_folder = os.path.join(parent, "bin", str(self.settings.build_type))

        for src_folder in { "bin" , "lib" }:
            self.copy("*.dll", dst=target_folder, src=src_folder)
            self.copy("*.so*", dst=target_folder, src=src_folder)

    def add_opengl(self):
        self.requires("glfw/3.3.7")
        self.requires("glad/0.1.35")
        self.requires("glew/2.2.0")
        self.requires("opengl/system")
        self.requires("glm/0.9.9.8")