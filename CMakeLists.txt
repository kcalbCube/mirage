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

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/magic_enum/include")
include_directories("${CMAKE_SOURCE_DIR}/include/fmt/include")
include_directories("${CMAKE_SOURCE_DIR}/include/entt/src")
include_directories("${CMAKE_SOURCE_DIR}/include/SDL/include")
include_directories("${CMAKE_SOURCE_DIR}/include/imgui")
include_directories("${CMAKE_SOURCE_DIR}/include/boost")
include_directories("${CMAKE_SOURCE_DIR}/include/googletest/googletest/include")
include_directories("${CMAKE_SOURCE_DIR}/include/googletest/googlemock/include")

include(CMake/Utils.cmake)
include(CMake/cotire.cmake)

add_compile_options("-g")

include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# dependencies
add_subdirectory("${CMAKE_SOURCE_DIR}/include/SDL")
add_subdirectory("${CMAKE_SOURCE_DIR}/include/googletest")
#cotire(SDL2)
add_subdirectory("${CMAKE_SOURCE_DIR}/include/boost" EXCLUDE_FROM_ALL)

################################################################################
# Sub-projects
################################################################################
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)
