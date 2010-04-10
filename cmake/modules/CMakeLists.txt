project(QubeCmakeModules)

set(cmakeFiles
    CheckCXXSourceCompiles.cmake
    CheckCXXSourceRuns.cmake
    CheckCXXSymbolExists.cmake
    CheckPointerMember.cmake
    CheckPrototypeExists.cmake
    CheckStructMember.cmake
    FindACL.cmake
    FindAlsa.cmake
    FindAvahi.cmake
    FindBlitz.cmake
    FindBlueZ.cmake
    FindFontconfig.cmake
    FindFreetype.cmake
    FindLCMS.cmake
    FindLibraryWithDebug.cmake
    FindLinuxWirelesstools.cmake
    FindNetworkManager.cmake
    FindPkgConfig.cmake
    FindPopplerQt4.cmake
    FindPulseAudio.cmake
    FindQCA2.cmake
    FindQImageBlitz.cmake
    FindQt4.cmake
    FindSamba.cmake
    FindSane.cmake
    FindSharedMimeInfo.cmake
    FindUSB.cmake
    MacroAddCompileFlags.cmake
    MacroAddLinkFlags.cmake
    MacroEnsureOutOfSourceBuild.cmake
    MacroEnsureVersion.cmake
    MacroLibrary.cmake
    Qt4ConfigDependentSettings.cmake
    Qt4Macros.cmake
)

install(FILES ${cmakeFiles}
    DESTINATION ${QUBE_DEVELOPMENT_CMAKE_DIR}/Modules)
