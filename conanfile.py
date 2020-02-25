from conans import ConanFile, CMake, tools
import os

class ResourcerConan(ConanFile):
    name = "resourcer"
    version = "0.1"
    license = "GNU GPL v3"
    url = "https://gitlab.com/philabs/resourcer"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "tests": [True, False]}
    generators = "cmake", "ycm"
    requires = (
        "boost_asio/1.69.0@bincrafters/stable",
        "boost_beast/1.69.0@bincrafters/stable",
        "boost_lexical_cast/1.69.0@bincrafters/stable",
        "gtest/1.8.0@bincrafters/stable",
        "rapidjson/1.1.0@bincrafters/stable",
        "OpenSSL/1.0.2q@conan/stable",
        "UErrno/0.1@javjarfer/testing",
        "StdWrapper/0.1@javjarfer/testing",
        "Phi/0.1@javjarfer/testing"
    )
    default_options = (
        "shared=False",
        "tests=False",
        #  GTest options
        "gtest:shared=False"
    )

    def source(self):
        self.run("git clone https://gitlab.com/philabs/resourcer")
        self.run("cd resourcer && git checkout develop")

    def build(self):
        cmake = CMake(self, parallel=True)
        if self.options.tests:
            cmake.definitions["CMAKE_BUILD_TESTS"] = "ON"
            cmake.definitions["BUILD_SHARED_LIBS"] = "ON"
        cmake.configure(source_folder="resourcer")
        cmake.build(target="install")

    def imports(self):
        self.copy("*.dll", "bin")

    def package(self):
        self.copy("*.h", dst="include", src="resourcer/include")
        self.copy("*Service.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ResourcerServ"]
