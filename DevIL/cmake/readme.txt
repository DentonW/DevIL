You dont have to use cmake here, obviously,

check out www.cmake.org for documentation and download

If you run the cmake_all.bat (windows) you should with a bit of luck
create some project files for use in MSVC of various flavours. This
will depend upon you having the right flavour of MSVC installed and
cmake.exe in the path.

Note that we need two cmake configured outputs, one for dlls and one
for static linking. I'm gonna take a good look at the cmake stuff
and see if that can be merged (currently that is what the cmake boys
recomend). It doesnt make much diference using cmake then make but
looks bad when using MSVC.

The devc(mingwin) version doesnt build yet and my 2003 install has
died so I'm only sure the vc6 one works. But thats a start. I will
try and improve on this. Certanly it makes more sense than trying
to maintain MSVC compatability across a number of versions.


20040701 : Kriss
