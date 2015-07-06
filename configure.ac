dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(ImageOutliner, 1.0)

AC_CHECK_LIB([boost_program_options], [main])
# libpng needs zlib
AC_CHECK_LIB([z], [main])
# libpng is needed so CImg can read pngs with 4 channels (RGBA)
# also, AM_CPPFLAGS=-Dcimg_use_png should be defined onto Makefile.am
AC_CHECK_LIB([png], [main])


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

