cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(Mirage CXX)

set(ARCH, "x64")

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/magic_enum-master/include")
include_directories("${CMAKE_SOURCE_DIR}/include/boost_1_78_0")
include_directories("${CMAKE_SOURCE_DIR}/include/fmt/include")
include_directories("${CMAKE_SOURCE_DIR}/include/entt/src")
include_directories("${CMAKE_SOURCE_DIR}/include/imgui")
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include/SDL/include")

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

add_compile_options("-g")

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)



################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#add_compile_options (-fcolor-diagnostics)

# SDL
add_subdirectory("${CMAKE_SOURCE_DIR}/include/SDL")

################################################################################
# Sub-projects
################################################################################
add_subdirectory(server)
add_subdirectory(client)
