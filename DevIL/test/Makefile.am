check_PROGRAMS = suite
check_SCRIPTS = format_test/format_checks.sh

suite_SOURCES = format_test/suite.c
suite_LDADD = ../lib/libILU.la

TESTS_ENVIRONMENT = /bin/bash
TESTS = $(check_SCRIPTS)
