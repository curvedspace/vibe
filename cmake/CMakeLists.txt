set(cmakeFiles
    check_installed_exports_file.cmake
    FindPAM.cmake
    FindSharedMimeInfo.cmake
    HandleImportedTargetsInCMakeRequiredLibraries.cmake
    HawaiiInstallDirs.cmake
    MacroAddCompileFlags.cmake
    MacroAdditionalCleanFiles.cmake
    MacroAddLinkFlags.cmake
    MacroAppendIf.cmake
    MacroBoolTo01.cmake
    MacroEnsureOutOfSourceBuild.cmake
    MacroEnsureVersion.cmake
    MacroLibrary.cmake
    MacroLogFeature.cmake
    MacroOptionalAddSubdirectory.cmake
    MacroOptionalDependPackage.cmake
    MacroOptionalFindPackage.cmake
    MacroPushRequiredVars.cmake
    MacroWriteBasicCMakeVersionFile.cmake
)

install(FILES ${cmakeFiles} DESTINATION ${CMAKE_ROOT}/Modules)
