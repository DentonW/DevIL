Build DevIL with CMake
----------------------


For Linux/Unix/OSX/mingw, using makefiles:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install

To generate visual studio project files (untested!):

    makedir build
    cd build
    cmake -G"Visual Studio 14 2015" ..

See here for list of generators:
http://www.cmake.org/cmake/help/v3.3/manual/cmake-generators.7.html



Debug/Release/etc:

    $ cmake -DCMAKE_BUILD_TYPE=Debug ..

