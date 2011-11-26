add_definitions(-DVIBE_HARDWARE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTDBUS_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Check whether some processor features are supported.
check_cxx_source_compiles("	#ifdef __SUNPRO_CC
					#define __asm__ asm
				#endif
				int main() { __asm__(\"pxor %mm0, %mm0\") ; }" HAVE_X86_MMX)
check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                        #define __asm__ asm
                                #endif
				int main() { __asm__(\"xorps %xmm0, %xmm0\"); }" HAVE_X86_SSE)
check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                        #define __asm__ asm
                                #endif
				int main() { __asm__(\"xorpd %xmm0, %xmm0\"); }" HAVE_X86_SSE2)
check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                        #define __asm__ asm
                                #endif
				int main() { __asm__(\"femms\"); }" HAVE_X86_3DNOW)
check_cxx_source_compiles("     #ifdef __SUNPRO_CC
                                        #define __asm__ asm
                                #endif
				int main() { __asm__(\"mtspr 256, %0; vand %%v0, %%v0, %%v0\" : : \"r\"(-1) ); }" HAVE_PPC_ALTIVEC)

configure_file(config-processor.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-processor.h)

########### next target ###############

set(SOURCES
   namespace.cpp
   managerbase.cpp
   device.cpp
   devicemanager.cpp
   deviceinterface.cpp
   genericinterface.cpp
   processor.cpp
   block.cpp
   storagedrive.cpp
   opticaldrive.cpp
   storagevolume.cpp
   opticaldisc.cpp
   storageaccess.cpp
   camera.cpp
   portablemediaplayer.cpp
   networkinterface.cpp
   networkshare.cpp
   serialinterface.cpp
   acadapter.cpp
   battery.cpp
   button.cpp
   audiointerface.cpp
   dvbinterface.cpp
   predicate.cpp
   predicateparse.cpp
   predicate_lexer.c
   predicate_parser.c
   powermanagement.cpp
   networking.cpp
   video.cpp
   smartcardreader.cpp
   internetgateway.cpp

   ifaces/acadapter.cpp
   ifaces/audiointerface.cpp
   ifaces/battery.cpp
   ifaces/block.cpp
   ifaces/button.cpp
   ifaces/camera.cpp
   ifaces/opticaldrive.cpp
   ifaces/device.cpp
   ifaces/deviceinterface.cpp
   ifaces/devicemanager.cpp
   ifaces/dvbinterface.cpp
   ifaces/genericinterface.cpp
   ifaces/networkinterface.cpp
   ifaces/networkshare.cpp
   ifaces/serialinterface.cpp
   ifaces/opticaldisc.cpp
   ifaces/portablemediaplayer.cpp
   ifaces/processor.cpp
   ifaces/storagedrive.cpp
   ifaces/storagevolume.cpp
   ifaces/storageaccess.cpp
   ifaces/video.cpp
   ifaces/smartcardreader.cpp
   ifaces/internetgateway.cpp

   backends/shared/rootdevice.cpp
   backends/shared/cpufeatures.cpp
)

# Look for HUpnp
macro_optional_find_package(HUpnp)

if(HUPNP_FOUND AND ((HUPNP_VERSION_MAJOR EQUAL 0 AND HUPNP_VERSION_MINOR EQUAL 9) OR (HUPNP_VERSION_MAJOR EQUAL 1)))
    include_directories(${HUPNP_INCLUDE_DIR})
    message(STATUS "Building VibeHardware UPnP backend.")
    set(SOURCES ${SOURCES}
       backends/upnp/upnpdevice.cpp
       backends/upnp/upnpdevicemanager.cpp
       backends/upnp/upnpdeviceinterface.cpp
       backends/upnp/upnpmediaserver.cpp
       backends/upnp/upnpinternetgateway.cpp
       backends/upnp/upnpcontrolpoint.cpp
    )
else()
    set(HUPNP_FOUND false)
endif()
macro_log_feature(HUPNP_FOUND "HUPnP" "UPnP support for VibeHardware" "http://www.herqq.org"
    FALSE "" "Allows VibeHardware to provide information about UPnP devices on the network")

# Look for UDev
macro_optional_find_package(UDev)
macro_log_feature(UDEV_FOUND "UDev" "UDev support for VibeHardware" "http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev.html"
    FALSE "" "Allows VibeHardware to use UDev to provide information about devices on Linux")

# Configure header
configure_file(config-qubehardware.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-qubehardware.h)

# Include UDev support if possible
if(UDEV_FOUND)
    message(STATUS "Building VibeHardware UDev backend.")
    set(SOURCES ${SOURCES}
        backends/udev/udevdevice.cpp
        backends/udev/udevmanager.cpp
        backends/udev/udevdeviceinterface.cpp
        backends/udev/udevgenericinterface.cpp
        backends/udev/cpuinfo.cpp
        backends/udev/udevprocessor.cpp
        backends/udev/udevcamera.cpp
        backends/udev/udevvideo.cpp
        backends/udev/udevaudiointerface.cpp
        backends/udev/udevnetworkinterface.cpp
        backends/udev/udevserialinterface.cpp
        backends/udev/udevaudiointerface_p.cpp
        backends/udev/udevportablemediaplayer.cpp
        backends/udev/udevdvbinterface.cpp
        backends/udev/udevblock.cpp

        backends/shared/udevqtclient.cpp
        backends/shared/udevqtdevice.cpp
    )
endif(UDEV_FOUND)

# Include UPower support
message(STATUS "Building VibeHardware UPower backend.")
set(SOURCES ${SOURCES}
    backends/upower/upowermanager.cpp
    backends/upower/upowerdevice.cpp
    backends/upower/upowerbattery.cpp
    backends/upower/upoweracadapter.cpp
    backends/upower/upowerdeviceinterface.cpp
    backends/upower/upowergenericinterface.cpp
)

# Include UDisks support if possible
# FIXME: this should work on more Unix systems
if(CMAKE_SYSTEM_NAME MATCHES Linux)
    message(STATUS "Building VibeHardware UDisks backend.")
    set(SOURCES ${SOURCES}
        backends/udisks/udisksmanager.cpp
        backends/udisks/udisksdevice.cpp
        backends/udisks/udisksblock.cpp
        backends/udisks/udisksstoragevolume.cpp
        backends/udisks/udisksdeviceinterface.cpp
        backends/udisks/udisksopticaldisc.cpp
        backends/udisks/udisksopticaldrive.cpp
        backends/udisks/udisksstoragedrive.cpp
        backends/udisks/udisksstorageaccess.cpp
        backends/udisks/udisksgenericinterface.cpp
    )
endif(CMAKE_SYSTEM_NAME MATCHES Linux)

# Include fstab support
message(STATUS "Building VibeHardware fstab backend.")
set(SOURCES ${SOURCES}
    backends/fstab/fstabmanager.cpp
    backends/fstab/fstabdevice.cpp
    backends/fstab/fstabnetworkshare.cpp
    backends/fstab/fstabstorageaccess.cpp
    backends/fstab/fstabhandling.cpp
    backends/fstab/fstabwatcher.cpp
)

# Set DBus interfaces sources properties
set_source_files_properties(
    org.freedesktop.PowerManagement.xml
    org.freedesktop.PowerManagement.Inhibit.xml
    org.vision.Vibe.Hardware.PowerManagement.PolicyAgent.xml
    PROPERTIES NO_NAMESPACE TRUE
)

# DBus interfaces
qt4_add_dbus_interfaces(SOURCES org.freedesktop.PowerManagement.xml
                        org.freedesktop.PowerManagement.Inhibit.xml
                        org.vision.Vibe.Hardware.PowerManagement.PolicyAgent.xml)

qt4_add_dbus_interface(SOURCES org.vision.Vibe.Hardware.Networking.Client.xml
                       org_vision_qube_hardware_networking_client)

# Library target
automoc4(VibeHardare SOURCES)
add_library(VibeHardware SHARED ${SOURCES})

# Prepare for optional libraries dependencies
set(OPTIONAL_LIBS)

# HUPnP dependency
if(HUPNP_FOUND)
    set(OPTIONAL_LIBS ${OPTIONAL_LIBS} ${HUPNP_LIBS})
endif(HUPNP_FOUND)

# UDev dependency
if(UDEV_FOUND)
    set(OPTIONAL_LIBS ${OPTIONAL_LIBS} ${UDEV_LIBS})
endif(UDEV_FOUND)

# Link libraries
target_link_libraries(VibeHardware ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTGUI_LIBRARY} ${OPTIONAL_LIBS})
target_link_libraries(VibeHardware LINK_INTERFACE_LIBRARIES ${QT_CORE_LIBRARY})

# Library properties
set_target_properties(VibeHardware PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

#install(FILES org.kde.Solid.Networking.Client.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
#install(FILES org.kde.Solid.PowerManagement.PolicyAgent.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
#install(FILES org.freedesktop.PowerManagement.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})
#install(FILES org.freedesktop.PowerManagement.Inhibit.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR})

########### install files ###############

install(FILES
    namespace.h device.h
    devicenotifier.h deviceinterface.h genericinterface.h processor.h block.h
    storageaccess.h storagedrive.h opticaldrive.h storagevolume.h opticaldisc.h
    camera.h portablemediaplayer.h networkinterface.h acadapter.h battery.h
    button.h audiointerface.h dvbinterface.h predicate.h powermanagement.h
    networking.h video.h serialinterface.h smartcardreader.h internetgateway.h
    networkshare.h DESTINATION include/Vibe/Hardware COMPONENT Devel)
install(TARGETS VibeHardware
    LIBRARY DESTINATION lib COMPONENT Framework)

########### parser build ###############

set(lexer_FILE predicate_lexer)
set(parser_FILE predicate_parser)

find_package(Flex)
macro_log_feature(FLEX_FOUND
    "Flex"
    "Allows the VibeHardware predicate parser to be updated"
    "http://flex.sourceforge.net"
    FALSE
    ""
    "Required by the UpdateVibeHardwarePredicateParser target (mainly useful for developers)")

find_program(BISON_EXECUTABLE bison)
macro_log_feature(BISON_EXECUTABLE
    "Bison"
    "Allows the VibeHardware predicate parser to be updated"
    "http://www.gnu.org/software/bison"
    FALSE
    ""
    "Required by the UpdateVibeHardwarePredicateParser target (mainly useful for developers)")
mark_as_advanced(BISON_EXECUTABLE)      # don't show it in the simple view in cmake-gui/ccmake

if(FLEX_EXECUTABLE AND BISON_EXECUTABLE)
    add_custom_target(UpdateVibeHardwarePredicateParser
                      COMMAND ${FLEX_EXECUTABLE} -P VibeHardware -o${lexer_FILE}.c ${lexer_FILE}.l
                      COMMAND ${BISON_EXECUTABLE} -p VibeHardware -d -b ${parser_FILE} ${parser_FILE}.y
                      COMMAND ${CMAKE_COMMAND} -E copy ${parser_FILE}.tab.c ${CMAKE_CURRENT_SOURCE_DIR}/${parser_FILE}.c
                      COMMAND ${CMAKE_COMMAND} -E copy ${parser_FILE}.tab.h ${CMAKE_CURRENT_SOURCE_DIR}/${parser_FILE}.h
                      COMMAND ${CMAKE_COMMAND} -E remove ${parser_FILE}.tab.c ${parser_FILE}.tab.h
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else(FLEX_EXECUTABLE AND BISON_EXECUTABLE)
    add_custom_target(UpdateVibeHardwarePredicateParser
                      COMMAND echo "flex and/or bison not found, so target UpdateVibeHardwarePredicateParser inactive")
endif(FLEX_EXECUTABLE AND BISON_EXECUTABLE)
