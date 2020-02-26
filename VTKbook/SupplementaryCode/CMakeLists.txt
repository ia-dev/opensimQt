cmake_minimum_required(VERSION 3.4)

#-----------------------------------------------------------------------------
# Set policies as needed.
foreach(p
#    CMP0020 # CMake 2.8.11
#    CMP0022 # CMake 2.8.12
#    CMP0025 # CMake 3.0
#    CMP0043 # CMake 3.0
#    CMP0053 # CMake 3.1
    CMP0074 # CMake 3.12
    )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

PROJECT(SupplementaryCode)

#-----------------------------------------------------------------------------
# We will enforce an out of source build.
STRING(COMPARE EQUAL "${${PROJECT_NAME}_SOURCE_DIR}"
  "${${PROJECT_NAME}_BINARY_DIR}" INSOURCE)
if(INSOURCE)
  MESSAGE(FATAL_ERROR "${PROJECT_NAME} requires an out of source build. Please create a separate binary directory and run CMake there.")
endif(INSOURCE)

#-----------------------------------------------------------------------------
# Output directories.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
  "Single output directory for building all executables.")

#-----------------------------------------------------------------------------
# Directory where the CMakeFiles are
set ( CMAKE_FILE_DIR
  ${PROJECT_SOURCE_DIR}/CMake
)
#-----------------------------------------------------------------------------
# Set the compiler warning level and other compiler settings.
include(${CMAKE_FILE_DIR}/DetermineCompilerFlags.cmake)
include(${CMAKE_FILE_DIR}/SetCompilerFeatures.cmake)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#-----------------------------------------------------------------------------
# Set the C++ Standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Enable this if you need -std=c++14 instead of -std=g++14 for gcc compilers.
# This must be set before the target-definition (add_library or add_executable).
#set(CMAKE_CXX_EXTENSIONS OFF)


#-----------------------------------------------------------------------------
# Set executable flags.
SET(EXECUTABLE_FLAG WIN32 MACOSX_BUNDLE )

# Subdirectories.
add_subdirectory(Chapter03)
add_subdirectory(Chapter07)
