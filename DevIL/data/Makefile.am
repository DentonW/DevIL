pkgdir=$(libdir)/pkgconfig

pkg_DATA =

if BUILD_MONOLITHIC

if BUILD_IL
pkg_DATA += IL.pc
endif #BUILD_IL

if BUILD_ILU
pkg_DATA += ILU.pc
endif #BUILD_ILU

if BUILD_ILUT
pkg_DATA += ILUT.pc
endif #BUILD_ILUT

else #!BUILD_MONOLITHIC

pkg_DATA += DevIL.pc

endif #BUILD_MONOLITHIC
