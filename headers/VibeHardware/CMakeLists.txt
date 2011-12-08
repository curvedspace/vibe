set(HEADERS
    VAcAdapter
    VAudioInterface
    VBattery
    VBlock
    VButton
    VCamera
    VDevice
    VDeviceInterface
    VDeviceNotifier
    VDvbInterface
    VGenericInterface
    VInternetGateway
    VNetworking
    VNetworkInterface
    VNetworkShare
    VOpticalDisc
    VOpticalDrive
    VPortableMediaPlayer
    VPowerManagement
    VPredicate
    VProcessor
    VHardware
    VSerialInterface
    VSmartCardReader
    VStorageAccess
    VStorageDrive
    VStorageVolume
    VVideo
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeHardware COMPONENT Devel)
