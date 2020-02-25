Smuggler
========

## Index

 1. Summary:
    * Resume of the app purpose.

 2. Priorities:
    * List of the priorities that will be followed during the development
       of this library. We hope this way users will have early knowledge
       of when certain features will be available.

 4. Usage guide:
    * Basic introduction on how to use the project.

 3. Acknowledgements:
    * Thanks to the community.

## Summary

A minimal utility to rewrite header files from TCP/UPD packages, to easely perform [ip | mac] spoofing.

## Priorities

 1. Make the utility functional.

 2. Ensure it's performant.

 3. Cover community requirements.

## Usage

### Compile

To make use of the library it's required have the next utilities:

    - CMake, version >= 3
    - Compiler supporting at least C++17.
    - Conan, version >= 1.10
    - Doxygen

To consume the library in a project, it's only required to add the
Bintray conan repository to your remote list, and add the project
as a dependency:

    - conan remote add Philabs https://api.bintray.com/conan/javjarfer/Philabs

To compile the library itself it's only required to:

    - git clone https://github.com/JavierJF/smuggler
    - cd smuggler
    - mkdir build
    - cd build

In case of Windows-Debug:

    - conan install .. -s build_type=Debug  --build=missing -s compiler.libcxx=libstdc++11
    - cmake -G "Ninja" -DCMAKE_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH=install -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl ..
    - cmake --build . --config Debug

### Generate the docs:

Dependencies to construct the documentation with Sphinx + Breathe the docs:

    - Install python3
    - pip install sphinx sphinx_adc_theme sphinx_rtd_theme recommonmark breathe

Generate the docs:

    - cd build
    - cmake --build . --target doc; ..\doc\sphinx\make.bat html

### Launch the tests

After making the build, execute the binary:

    - build/bin/Tests

## Thanks

Thanks to all Open Source / Free Software communities, for his effort and constance.
Without then, it wouldn't be possible to access to the amount of information this
project requires. Also thanks to the C++ community, for his desires to share their
knowledge.
