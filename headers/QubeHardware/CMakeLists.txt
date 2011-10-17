set(HEADERS
    AcAdapter
    AudioInterface
    Battery
    Block
    Button
    Camera
    Device
    DeviceInterface
    DeviceNotifier
    DvbInterface
    GenericInterface
    InternetGateway
    Networking
    NetworkInterface
    NetworkShare
    OpticalDisc
    OpticalDrive
    PortableMediaPlayer
    PowerManagement
    Predicate
    Processor
    QubeHardwareNamespace
    SerialInterface
    SmartCardReader
    StorageAccess
    StorageDrive
    StorageVolume
    Video
)

install(FILES ${HEADERS}
    DESTINATION include/Qube/QubeHardware COMPONENT Devel)
