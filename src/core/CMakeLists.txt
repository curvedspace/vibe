include_directories(
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/accounts
    ${CMAKE_CURRENT_SOURCE_DIR}/archive
    ${CMAKE_CURRENT_SOURCE_DIR}/compression
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/jobs
    ${CMAKE_CURRENT_SOURCE_DIR}/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/standards
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Create the cmakedirs.h header file
configure_file(cmakedirs.h.in ${CMAKE_CURRENT_BINARY_DIR}/cmakedirs.h)

# Configure checks for compression/
include(compression/ConfigureChecks.cmake)
configure_file(
    compression/config-compression.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/compression/config-compression.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/compression)

# Compile bzip2 support if available
if(BZIP2_FOUND)
   include_directories(${BZIP2_INCLUDE_DIR})
   set(VibeCore_OPTIONAL_SRCS ${VibeCore_OPTIONAL_SRCS} compression/vbzip2compressionfilter.cpp)
   set(VibeCore_OPTIONAL_LIBS ${VibeCore_OPTIONAL_LIBS} ${BZIP2_LIBRARIES})
endif(BZIP2_FOUND)

# Compile lzma support if available
if(LIBLZMA_FOUND)
   include_directories(${LIBLZMA_INCLUDE_DIRS})
   set(VibeCore_OPTIONAL_SRCS ${VibeCore_OPTIONAL_SRCS} compression/vxzcompressionfilter.cpp)
   set(VibeCore_OPTIONAL_LIBS ${VibeCore_OPTIONAL_LIBS} ${LIBLZMA_LIBRARIES})
endif(LIBLZMA_FOUND)

# Sources
set(SOURCES
    vapplicationinfo.cpp
    vcommandoptions.cpp
    vstringhandler.cpp
    vdesktopfile.cpp

    accounts/vaccountsmanager.cpp
    accounts/vuseraccount.cpp

    archive/varchive.cpp
    archive/varchivehandler.cpp
    archive/varchivehandlerplugin.cpp
    archive/vlimitediodevice.cpp

    bookmarks/vbookmark.cpp
    bookmarks/vbookmarkmanager.cpp
    bookmarks/vbookmarkmanageradaptor.cpp

    compression/vabstractcompressionfilter.cpp
    compression/vcompressionfilter.cpp
    compression/vgzipcompressionfilter.cpp

    io/vfilestring.cpp
    io/vfilesystemwatcher.cpp
    io/vsavefile.cpp

    jobs/vcompositejob.cpp
    jobs/vjob.cpp
    jobs/vjobtrackerinterface.cpp
    jobs/vjobuidelegate.cpp

    settings/vsettings.cpp
    settings/vsettingsschema.cpp

    ${VibeCore_OPTIONAL_SRCS}
)

# Headers
set(HEADERS
    vapplicationinfo.h
    vcommandoptions.h
    vstringhandler.h
    vsharedpointer.h
    vdesktopfile.h

    accounts/vaccountsmanager.h
    accounts/vuseraccount.h

    archive/varchive.h
    archive/varchivehandler.h
    archive/varchivehandlerplugin.h

    bookmarks/vbookmark.h
    bookmarks/vbookmarkmanager.h

    compression/vabstractcompressionfilter.h
    compression/vcompressionfilter.h

    io/vfilestring.h
    io/vfilesystemwatcher.h
    io/vsavefile.h

    jobs/vcompositejob.h
    jobs/vjob.h
    jobs/vjobtrackerinterface.h
    jobs/vjobuidelegate.h

    settings/vsettings.h
)

# AccountsService DBus adaptors
qt5_add_dbus_interfaces(SOURCES
    ${CMAKE_SOURCE_DIR}/data/interfaces/org.freedesktop.Accounts.xml
    ${CMAKE_SOURCE_DIR}/data/interfaces/org.freedesktop.Accounts.User.xml
)

# Add library
add_compiler_export_flags()
add_library(VibeCore SHARED ${SOURCES})
generate_export_header(VibeCore
    BASE_NAME vibecore
    DEPRECATED_MACRO_NAME VIBE_DEPRECATED
)
qt5_use_modules(VibeCore Core DBus Xml)
target_link_libraries(VibeCore ${VibeCore_OPTIONAL_LIBS})
set_target_properties(VibeCore PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

# Install files
set(HEADERS ${HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/vibecore_export.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore)
install(TARGETS VibeCore LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
