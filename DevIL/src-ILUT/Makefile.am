SUBDIRS = include src
DIST_SUBDIRS = include src

dist-hook:
	cp -Rf "msvc9"      $(distdir)
	find $(distdir) -name CVS -type d | xargs rm -rf
