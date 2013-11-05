noinst_PROGRAMS = mathgl_pp


# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/libmathglpp $(all_includes)

# the library search path.
mathgl_pp_LDFLAGS = $(all_libraries) $(LIBSDL_RPATH)
mathgl_pp_CXXFLAGS = $(LIBSDL_CFLAGS)
mathgl_pp_LDADD = $(top_builddir)/src/libmathglpp/libmathglpp.la $(GL_LIBS) \
	$(GLU_LIBS) $(LIBSDL_LIBS)

mathgl_pp_SOURCES = mathgl_pp.cpp


bin_SCRIPTS = mathglpp-config

SUBDIRS = libmathglpp unitests
