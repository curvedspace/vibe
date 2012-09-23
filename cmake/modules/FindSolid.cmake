# - Try to find Solid
# Once done this will define
#
#  Solid_FOUND - system has Solid
#  Sold_INCLUDE_DIRS - the Solid include directory
#  Solid_LIBRARIES - the Solid libraries

# Copyright (c) 2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_path(Solid_INCLUDE_DIRS solid/solidnamespace.h)
find_library(Solid_LIBRARIES solid)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Solid DEFAULT_MSG Solid_INCLUDE_DIRS Solid_LIBRARIES)

mark_as_advanced(Solid_INCLUDE_DIRS Solid_LIBRARIES)
