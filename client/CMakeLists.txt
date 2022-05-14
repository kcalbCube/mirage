set(PROJECT_NAME client)
################################################################################
# Source groups
################################################################################
set(Header_Files
	"client.h"
	"render/window.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
	"main.cpp"
	"client.cpp"
	"render/window.cpp"
	"render/imgui_impl.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES} )

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")

# Libraries
target_link_libraries(${PROJECT_NAME} SDL2-static boost_serialization)
cotire(${PROJECT_NAME})

################################################################################
# Compile and link options
################################################################################
target_compile_options(${PROJECT_NAME} PRIVATE
	-std=c++2a
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
set(ROOT_NAMESPACE client)
