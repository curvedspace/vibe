add_definitions(-DVIBE_BUILDING -Wno-multichar)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/accounts
    ${CMAKE_CURRENT_SOURCE_DIR}/archive
    ${CMAKE_CURRENT_SOURCE_DIR}/compression
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/standards
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vglobal.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/vglobal.h
    IMMEDIATE @ONLY)

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

set(SOURCES
    vapplicationinfo.cpp
    vcommandoptions.cpp
    vstringhandler.cpp

    accounts/vaccountsmanager.cpp
    accounts/vuseraccount.cpp

    archive/varchive.cpp
    archive/varchivehandler.cpp
    archive/varchivehandlerplugin.cpp
    archive/vlimitediodevice.cpp

    compression/vabstractcompressionfilter.cpp
    compression/vcompressionfilter.cpp
    compression/vgzipcompressionfilter.cpp

    io/vfilestring.cpp
    io/vfilesystemwatcher.cpp
    io/vsavefile.cpp

    settings/settingsschema.cpp
    settings/settingsschemaloader.cpp
    settings/vsettings.cpp

    standards/vdesktopfile.cpp
    standards/vstandarddirectories.cpp

    ${VibeCore_OPTIONAL_SRCS}
)

set(HEADERS
    vapplicationinfo.h
    vcommandoptions.h
    vstringhandler.h
    ${CMAKE_CURRENT_BINARY_DIR}/vglobal.h

    accounts/vaccountsmanager.h
    accounts/vuseraccount.h

    archive/varchive.h
    archive/varchivehandler.h
    archive/varchivehandlerplugin.h

    compression/vabstractcompressionfilter.h
    compression/vcompressionfilter.h

    io/vfilestring.h
    io/vfilesystemwatcher.h
    io/vsavefile.h

    settings/vsettings.h

    standards/vdesktopfile.h
    standards/vstandarddirectories.h
)

add_library(VibeCore SHARED ${SOURCES})
target_link_libraries(VibeCore ${Qt5Core_LIBRARIES} ${Qt5Xml_LIBRARIES} ${VibeCore_OPTIONAL_LIBS})
set_target_properties(VibeCore PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeCore COMPONENT Devel)
install(TARGETS VibeCore LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
