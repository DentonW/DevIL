ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib docs test data

if BUILD_EXAMPLES
SUBDIRS += examples
endif #BUILD_EXAMPLES

#SUBDIRS = src-IL src-ILU src-ILUT include
#DIST_SUBDIRS = src-IL src-ILU src-ILUT include
EXTRA_DIST = README.unix README.win Libraries.txt CREDITS MSVC++.txt configure.bat README.macosx

# (uncoment this if you want to include examples to the distributed tarball)
# Actually it has to be commented now... It is currently very troublesome to include the projects in the distribution archive...
#SUBDIRS += projects

DISTCHECK_CONFIGURE_FLAGS = --enable-ILU --enable-ILUT --with-examples
