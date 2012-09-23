set(cmakeFiles
    CheckCXXSymbolExists.cmake
    check_installed_exports_file.cmake
    CheckPointerMember.cmake
    CheckPrototypeExists.cmake
    CheckStructMember.cmake
    FindLibLZMA.cmake
    FindPAM.cmake
    FindSharedMimeInfo.cmake
    HandleImportedTargetsInCMakeRequiredLibraries.cmake
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
    MauiInstallDirs.cmake
)

install(FILES ${cmakeFiles} DESTINATION ${CMAKE_ROOT}/Modules)
