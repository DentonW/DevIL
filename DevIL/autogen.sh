#!/bin/sh
#
aclocal -I .
autoheader
automake --foreign --include-deps --add-missing --copy
autoconf

#./configure $*
#echo "Now you are ready to run ./configure"
