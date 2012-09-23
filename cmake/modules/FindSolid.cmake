# - Try to find Solid
# Once done this will define
#
#  SOLID_FOUND - system has Solid
#  SOLID_INCLUDE_DIR - the Solid include directory
#  SOLID_LIBS - the Solid libraries

# Copyright (c) 2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_path(SOLID_INCLUDE_DIR solid/solidnamespace.h)
find_library(SOLID_LIBS solid)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Solid DEFAULT_MSG SOLID_INCLUDE_DIR SOLID_LIBS)

mark_as_advanced(SOLID_INCLUDE_DIR SOLID_LIBS)
