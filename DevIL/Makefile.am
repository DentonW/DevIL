ACLOCAL_AMFLAGS = -I m4

SUBDIRS      = src-IL src-ILU src-ILUT include
DIST_SUBDIRS = src-IL src-ILU src-ILUT include
EXTRA_DIST = README.unix   README.win    \
             Libraries.txt CREDITS       \
             MSVC++.txt    configure.bat \
             README.macosx

dist-hook:
	cp -Rf "cpp wrapper"   $(distdir)
	cp -Rf "examples"      $(distdir)
	cp -Rf "test"          $(distdir)
	cp -Rf "projects"      $(distdir)
	cp -Rf "libtools"	   $(distdir)
	find $(distdir) -name obj -type d -exec echo \"{}\" \; | xargs rm -rf
	find $(distdir) -name Debug -type d -exec echo \"{}\" \; | xargs rm -rf
	find $(distdir) -name Release -type d -exec echo \"{}\" \; | xargs rm -rf

