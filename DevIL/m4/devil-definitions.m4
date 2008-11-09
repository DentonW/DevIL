
AC_DEFUN([ADD_CFLAGS],
	 [LIBIL_CFLAGS="$LIBIL_CFLAGS $1"
          LIBILU_CFLAGS="$LIBILU_CFLAGS $1"
          LIBILUT_CFLAGS="$LIBILUT_CFLAGS $1"
          CFLAGS="$CFLAGS $1"])

dnl
dnl Check CPU Extensions
dnl
AC_DEFUN([TEST_EXT],
	 [AC_ARG_ENABLE([$6],
			[AC_HELP_STRING([--enable-$6],
                                        [Doesn't compile $1 support. Overrides general enable of ASM])],
			[use_$1=$enable_$6
                         test "x$enable_$6" = "xyes" && enable_asm="yes"],
			[use_$1=test])
         AC_MSG_CHECKING([$1])
         AS_IF([test "$use_$1" = "yes"],
               [AC_DEFINE([$4],
                          [],
                          [$1 extension found])
                AC_DEFINE([VECTORMEM],
                          [],
                          [Memory must be vector aligned])
                ADD_CFLAGS([$2])
                AC_MSG_RESULT([yes])
                SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
               [test "$use_$1" = "test" -a "x$enable_asm" = "xyes"],
               [CFLAGS_TMP=$CFLAGS
                CFLAGS=$CFLAGS" $2"
                AC_COMPILE_IFELSE([$5
                                   int main() 
                                   {$3;return 0;}],
                                  [AC_DEFINE([$4],
                                             [],
                                             [$1 extension found])
                                   AC_DEFINE([VECTORMEM],
                                             [],
                                             [Memory must be vector aligned])
                                   ADD_CFLAGS([$2])
                                   AC_MSG_RESULT([yes])
                                   SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
                                  [AC_MSG_RESULT([no])
                                   CFLAGS=$CFLAGS_TMP])],
               [AC_MSG_RESULT(disabled)]) ])

dnl
dnl Check CPU Assembly
dnl
dnl add test to have only an ASM for compilation
AC_DEFUN([TEST_ASM],
	 [AC_ARG_ENABLE([$4],
			[AC_HELP_STRING([--enable-$4],
					[Compile with $4 support])],
                        [use_$4=$enable_$4],[use_$4=test])
          AC_MSG_CHECKING([$4])
          AS_IF([test "$use_$4" = "yes"],
                [SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$2 "
		 AC_DEFINE([$3],
			   [],
			   [$2 assembly found]) 
		 AC_MSG_RESULT([yes])],
                [test "$use_$4" = "no"],
                [AC_MSG_RESULT(no)],
		[AS_CASE([$host_cpu], 
	                 [$1],
			 [SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$2 "
		          AC_DEFINE([$3],
                                    [],
                                    [$2 assembly found]) 
		          AC_MSG_RESULT([yes])],
	                 [AC_MSG_RESULT([no]) ]) ]) ])

dnl
dnl Setup Libraries
dnl SET_LIB([libname],[if-found],[if-not-found])
dnl
AC_DEFUN([SET_LIB],
	 [AC_ARG_WITH([$1],
		      [AC_HELP_STRING([--with-$1=[[yes/no]]],
				      [Do wou want to use lib$1?])],
	              [],
	              [enable_$1="yes"])
          AS_IF([test "$enable_$1" = "yes"],
	        [AC_CHECK_LIB([$1],
                              [main],
                              [LIBS="-l$1 $LIBS"
                               support_$1="yes"],
                              [support_$1="no"]) 
          AS_IF([test "x$support_$1" = "xyes" -a -n "$2"],
                [$2],
                [test "x$support_$1" = "xno" -a -n "$3"],
                [$3]) ]) ])

dnl
dnl Formats
dnl
AC_DEFUN([TEST_FORMAT],
	 [AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--disable-$1],
					[Doesn't compile $1 support])],
		        [use_$1="$enable_$1"],
			[use_$1="yes"])
          AS_IF([test "x$support_$1" = "xno"],
                [use_$1="no"])
          AS_IF([test "$no_$1" = "yes"],
                [AC_DEFINE([IL_NO_$2],
		           [],
		           [$2 support])],
                [test "$use_$1" = "yes"],
                [SUPPORTED_FORMATS=$SUPPORTED_FORMATS"$2 "],
                [AC_DEFINE([IL_NO_$2],
		           [],
		           [$2 support]) ]) ])

dnl
dnl ILUT APIs checking
dnl
AC_DEFUN([TEST_API],
	 [AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--disable-$1],
					[Doesn't compile $1 support])],
                        [use_$1=$enable_$1],
			[use_$1=yes]) ])


