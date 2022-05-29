set(PROJECT_NAME testing)
################################################################################
# Source groups
################################################################################

set(Header_Files	
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
	"main.cpp"
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
set(TARGET_NAME ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} SDL2-static boost_serialization SDL2main gtest gcov gmock)
cotire(${PROJECT_NAME})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE testing)

################################################################################
# Compile and link options
################################################################################
target_compile_options(${PROJECT_NAME} PRIVATE
	-std=c++2a
	-O0
	-Wall
	-Wpedantic
	-march=native	
	--coverage	
	${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
	${DEFAULT_CXX_EXCEPTION_HANDLING}
)
target_link_options(${PROJECT_NAME} PRIVATE	
)

