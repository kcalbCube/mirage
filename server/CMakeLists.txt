set(PROJECT_NAME server)
################################################################################
# Source groups
################################################################################
set(Header_Files
	"config.h"
	"server.h"
	"client.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
	"main.cpp"
	"server.cpp"
	"client.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE server)

if("${ARCH}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${ARCH}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
target_compile_options(${PROJECT_NAME} PRIVATE
	-std=c++2b
	-flto
	-O0
	-Wall
	-march=native
	${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
	${DEFAULT_CXX_EXCEPTION_HANDLING}
)
target_link_options(${PROJECT_NAME} PRIVATE
	-flto
)

################################################################################
# Dependencies
################################################################################
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

