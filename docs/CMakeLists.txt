# Find the qdoc executable
find_program(QDOC_EXECUTABLE qdoc)
if(NOT QDOC_EXECUTABLE)
    message(FATAL_ERROR "qdoc was not found")
endif()

set(_modules
    Core
    Gui
    Widgets
)

# Source code locations and qdoc includes
set(_locations "")
set(_includes "")
foreach(_module ${_modules})
    string(TOLOWER ${_module} _module_name_lower)

    string(TOUPPER ${_module} _module_name)
    string(REGEX REPLACE "-" "_" _module_name ${_module_name})

    get_property(_module_value TARGET Vibe${_module} PROPERTY SOURCES)
    if(_module_value)
        #set(_locations "${_locations} VIBE_${_module_name}_SOURCES=${_module_value}")
        #set(_includes "${_includes} VIBE_${_module_name}_QDOCCONF=${CMAKE_CURRENT_SOURCE_DIR}/config/modules/vibe${_module_name_lower}.qdocconf")
#        list(APPEND _locations "VIBE_${_module_name}_SOURCES=${_module_value}")
        list(APPEND _includes "VIBE_${_module_name}_QDOCCONF=${CMAKE_CURRENT_SOURCE_DIR}/config/modules/vibe${_module_name_lower}.qdocconf")
    else()
        #set(_includes "${_includes} VIBE_${_module_name}_QDOCCONF=${CMAKE_CURRENT_SOURCE_DIR}/config/modules/missing.qdocconf")
        list(APPEND _includes "VIBE_${_module_name}_QDOCCONF=${CMAKE_CURRENT_SOURCE_DIR}/config/modules/missing.qdocconf")
    endif()
endforeach()

# QDoc command
set(QDOC ${_locations} ${_includes} SOURCE_TREE=${CMAKE_SOURCE_DIR} MODULE_SOURCE_TREE=${CMAKE_CURRENT_SOURCE_DIR} MODULE_BUILD_TREE=${CMAKE_CURRENT_BINARY_DIR} ${QDOC_EXECUTABLE})

# Commands
add_custom_target(BuildOnlineDocs COMMAND ${QDOC} ${CMAKE_CURRENT_SOURCE_DIR}/config/vibe-build-online.qdocconf)
