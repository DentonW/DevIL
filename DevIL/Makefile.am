AUTOMAKE_OPTIONS = 1.4

SUBDIRS      = src-IL src-ILU src-ILUT include
DIST_SUBDIRS = src-IL src-ILU src-ILUT include
EXTRA_DIST = README.unix   README.win \
             Libraries.txt CREDITS \
             VERSION BUGS \
             MSVC++.txt

dist-hook:
	cp -Rf libtools      $(distdir)
	cp -Rf examples      $(distdir)
	cp -Rf docs          $(distdir)
	cp -Rf "cpp wrapper" $(distdir)
