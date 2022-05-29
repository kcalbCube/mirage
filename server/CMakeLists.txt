set(PROJECT_NAME server)
################################################################################
# Source groups
################################################################################

set(Header_Files
	"config.h"
	"server.h"
	"client.h"
	"game/client.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
	"main.cpp"
	"server.cpp"
	"client.cpp"
	"game/client.cpp"
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

target_link_libraries(${PROJECT_NAME} SDL2-static SDL2main boost_serialization)

cotire(${PROJECT_NAME})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE server)

################################################################################
# Compile and link options
################################################################################
target_compile_options(${PROJECT_NAME} PRIVATE
	-std=c++2a
	-O0
	-Wall
	-Wpedantic
	-march=native
	-DSERVERSIDE
	${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
	${DEFAULT_CXX_EXCEPTION_HANDLING}
)
target_link_options(${PROJECT_NAME} PRIVATE
)

