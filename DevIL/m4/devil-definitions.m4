
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
			[AC_HELP_STRING([--enable-$6],[Doesn't compile $1 support])],
			[use_$1=$enable_$6],
			[use_$1=test])
         AC_MSG_CHECKING([$1])
         AS_IF([test "$use_$1" = "yes"],
               [AC_DEFINE([$4],,[$1 extension found])
                AC_DEFINE([VECTORMEM],[],[Memory must be vector aligned])
                ADD_CFLAGS([$2])
                AC_MSG_RESULT([yes])
                SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
               [test "$use_$1" = "test"],
               CFLAGS_TMP=$CFLAGS
               CFLAGS=$CFLAGS" $2"
               AC_COMPILE_IFELSE([
      $5
      int main() {$3;return 0;}],
      [AC_DEFINE([$4],[],[$1 extension found])
       AC_DEFINE([VECTORMEM],[],[Memory must be vector aligned])
       ADD_CFLAGS([$2])
       AC_MSG_RESULT(yes)
       SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
      [AC_MSG_RESULT(no)]
      [CFLAGS=$CFLAGS_TMP]),
AC_MSG_RESULT(disabled))])

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
		          AC_DEFINE([$3],,[$2 assembly found]) 
		          AC_MSG_RESULT([yes])],
	                 [AC_MSG_RESULT([no]) ]) ]) ])

dnl
dnl Setup Libraries
dnl
AC_DEFUN([SET_LIB],
	 [LIB$2=""
          AC_ARG_WITH([$1],
		      [AC_HELP_STRING([--with-$1=path],
				      [Specify the lib$1 installation path])],
	 [ADD_CFLAGS([-I$enable_$1dir/include])
	 LIB$2="-L$enable_$1dir/lib"
	 no_$1=no
	 use_$1=yes ])
         AS_IF([test ! "$no_$1" = "yes" && test "$use_$1" = "yes" && test -z "$3"],
	       [LIB$2="$LIB$2 -l$1"],
               [test ! "$no_$1" = "yes" && test "$use_$1" = "yes" && ! test -z "$3"],
	       [LIB$2="$3"],
	       [])
        AC_SUBST(LIB$2) ])

dnl
dnl Formats
dnl
AC_DEFUN([TEST_FORMAT],
	 [AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--disable-$1],
					[Doesn't compile $1 support])],
		        [use_$1="$enable_$1"],
			[use_$1="yes"])

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


