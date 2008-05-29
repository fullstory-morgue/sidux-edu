noinst_LTLIBRARIES = libappinstaller.la


libappinstaller_la_SOURCES = fancylistviewitem.cpp functions.cpp unique.cpp process.cpp
libappinstaller_la_LDFLAGS  = -L/usr/lib/debug  $(all_libraries)
libappinstaller_la_LIBADD = @LIBOBJS@ 


INCLUDES = -I$(top_builddir) -I$(top_srcdir) $(all_includes)
METASOURCES = AUTO

