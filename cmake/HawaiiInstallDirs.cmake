# - Define standard installation directories for Hawaii applications
#
# Inclusion of this module defines the following variables:
#  CMAKE_INSTALL_<dir>      - destination for files of a given type
#  CMAKE_INSTALL_FULL_<dir> - corresponding absolute path
# A lot of directories are defined by GNUInstallDirs.cmake we only
# define additional directories for Hawaii applications:
#  PLUGINSDIR       - object code plugin libraries (plugins)
#  QMLDIR           - QML modules (qml)
#  APPLICATIONSDIR  - applications' desktop files (DATAROOTDIR/applications)
#  SETTINGSDIR      - settings (DATAROOTDIR/settings)
#
# Each CMAKE_INSTALL_<dir> value may be passed to the DESTINATION options of
# install() commands for the corresponding file type.  If the includer does
# not define a value the above-shown default will be used and the value will
# appear in the cache for editing by the user.
# Each CMAKE_INSTALL_FULL_<dir> value contains an absolute path constructed
# from the corresponding destination by prepending (if necessary) the value
# of CMAKE_INSTALL_PREFIX.

#=============================================================================
# Copyright 2011 Nikita Krupen'ko <krnekit@gmail.com>
# Copyright 2011 Kitware, Inc.
# Copyright 2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License").
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products 
#    derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

include(GNUInstallDirs)

#
# Installation directories
#

if(NOT DEFINED CMAKE_INSTALL_PLUGINSDIR)
  set(CMAKE_INSTALL_PLUGINSDIR "plugins" CACHE PATH "object code plugins libraries (plugins)")
endif()

if(NOT DEFINED CMAKE_INSTALL_QMLDIR)
  set(CMAKE_INSTALL_QMLDIR "qml" CACHE PATH "QML modules (qml)")
endif()

#-----------------------------------------------------------------------------
# Values whose defaults are relative to DATAROOTDIR.  Store empty values in
# the cache and store the defaults in local variables if the cache values are
# not set explicitly.  This auto-updates the defaults as DATAROOTDIR changes.

if(NOT CMAKE_INSTALL_APPLICATIONSDIR)
  set(CMAKE_INSTALL_APPLICATIONSDIR "" CACHE PATH "applications' desktop files (DATAROOTDIR/applications)")
  set(CMAKE_INSTALL_APPLICATIONSDIR "${CMAKE_INSTALL_DATAROOTDIR}/applications")
endif()

if(NOT CMAKE_INSTALL_SETTINGSDIR)
  set(CMAKE_INSTALL_SETTINGSDIR "" CACHE PATH "settings (DATAROOTDIR/settings)")
  set(CMAKE_INSTALL_SETTINGSDIR "${CMAKE_INSTALL_DATAROOTDIR}/settings")
endif()

#-----------------------------------------------------------------------------

mark_as_advanced(
  CMAKE_INSTALL_APPLICATIONSDIR
  CMAKE_INSTALL_PLUGINSDIR
  CMAKE_INSTALL_QMLDIR
  CMAKE_INSTALL_SETTINGSDIR
  )

# Result directories
#
foreach(dir
    APPLICATIONSDIR
    PLUGINSDIR
    QMLDIR
    SETTINGSDIR
    )
  if(NOT IS_ABSOLUTE ${CMAKE_INSTALL_${dir}})
    set(CMAKE_INSTALL_FULL_${dir} "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_${dir}}")
  else()
    set(CMAKE_INSTALL_FULL_${dir} "${CMAKE_INSTALL_${dir}}")
  endif()
endforeach()
