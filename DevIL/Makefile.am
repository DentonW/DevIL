AUTOMAKE_OPTIONS = 1.4 cygnus

SUBDIRS      = src-IL src-ILU src-ILUT include
DIST_SUBDIRS = src-IL src-ILU src-ILUT include
EXTRA_DIST = README.unix   README.win \
             Libraries.txt CREDITS \
             VERSION       BUGS \
             MSVC++.txt    configure.bat

dist-hook:
	cp -Rf "libtools"      $(distdir)
	cp -Rf "examples"      $(distdir)
	cp -Rf "test"          $(distdir)
	cp -Rf "projects"      $(distdir)
	find $(distdir) -name *.o -type f -exec echo \"{}\" \; | xargs rm -f
	find $(distdir) -name obj -type d -exec echo \"{}\" \; | xargs rm -rf
	find $(distdir) -name Debug -type d -exec echo \"{}\" \; | xargs rm -rf
	find $(distdir) -name Release -type d -exec echo \"{}\" \; | xargs rm -rf
	find $(distdir) -name CVS -type d -exec echo \"{}\" \; | xargs rm -rf
