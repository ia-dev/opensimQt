project( pendulumDF)

cmake_minimum_required(VERSION 3.2)

# Settings.
# ---------
set(OPENSIM_INSTALL_DIR "/home/ibr/myGitLab/VisSim/VisSim_OpenSim-dev/IAwork/install-opensim-core/lib/cmake/OpenSim")

set(TARGET pendulumDF CACHE TYPE STRING)
set(OPENSIM_INSTALL_DIR $ENV{OPENSIM_HOME}
        CACHE PATH "Top-level directory of OpenSim install")

# OpenSim uses C++11 language features.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and hook up to OpenSim.
# ----------------------------
find_package(OpenSim REQUIRED PATHS "${OPENSIM_INSTALL_DIR}")

# Configure this project.
# -----------------------
file(GLOB SOURCE_FILES *.h *.cpp)

add_executable(${TARGET} ${SOURCE_FILES})
target_link_libraries(${TARGET} ${OpenSim_LIBRARIES} )

add_subdirectory(BodyDragForce)
INCLUDE_DIRECTORIES(BodyDragForce)

