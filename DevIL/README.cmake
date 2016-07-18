Build DevIL with CMake
----------------------


For Linux/Unix/OSX/mingw, using makefiles:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install

To generate project files for visual studio/xcode/whatever, you
need to tell CMake which generator to use. For example:

    cmake -G"Visual Studio 14 2015" ..

See here for list of generators:
http://www.cmake.org/cmake/help/v3.3/manual/cmake-generators.7.html


Debug/Release/etc:

    $ cmake -DCMAKE_BUILD_TYPE=Debug ..


TODO:
- windows: document any linkage issues with Visual Studio - multithreaded,
  unicode, whatever.
- OSX: document any OSX-specificness - eg can we produce framework bundles
  easily?
- give some hints on installing prerequisites, eg:
    - an apt-get install line for ubuntu
    - brew packages to install for OSX
    - instructions for Msys2 on windows
    - steps for setting up libs for visual studio

