This directory contains directories with the necesary source files for external libraries.
The full package this source was extracted from is also included as an archive in this dir.

See the archives for more information about the given library

I've tried to setup the DLL builds resonably with a .def amd .rs but not all libraries where available with
this to hand so I may have had to improvise.

This is intended to enable windows builds to "Just work, TM.".

Which is kinda what I think people are looking for from devil. Its been fine if you
want to use the .DLLs but if you would like to incorporate the source then getting the
graphic sublibraries to compile is a pain.

Non windows builds probably wont need this, since the whole dll/so thing is done diferently.
So I'm concentrating on windows builds, but these may eventually end up being of
use for other systems.

What this means is that things are hard configured to windows, untill I get around to
updating the cmake scripts to be a bit smarter. I'n theory the thing to do would be to check
if the library is available and then if it isnt use the one supplied here.

Check out the DevIL/cmake directory which contains some usefull batch files to auto build
project files from these sources. I'm looking at pushing everything into cmake for windows since
it solves the which version of VC to suport problem.
There are actually 3 live versions of VC right now, vc6 vc7(2002) and vc7(2003) and it is 
obvious from the new naming convention that microsoft have plans to break stuff once a year
from now on.


20040701 : Kriss


Well I havent put them in CVS but

I have the following files locally and figure they should go with any source
distribution of this dir.

? src-XTRA/jpegsr6.zip
? src-XTRA/lcms-1.13.zip
? src-XTRA/lm1006.zip
? src-XTRA/lpng125.zip
? src-XTRA/tiff-v3.6.1.zip
? src-XTRA/zlib-1.2.1.tar.gz
