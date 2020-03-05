cmake_minimum_required(VERSION 3.5)

project(OpensimQtGUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets WebEngine WebEngineWidgets OpenGL  LinguistTools REQUIRED)


set(TS_FILES OpensimQtGUI_en_US.ts)

#adding opensim to the project
#after building and installing the project add the /bin
#to the system path
find_package(OpenSim REQUIRED)

#adding the vtk modules
find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersCore
  vtkInfovisCore
  vtkInteractionStyle
  vtkViewsQt
  vtkIOImage
  vtkIOGeometry
)
include(${VTK_USE_FILE})

include(cmake/GitUtils.cmake)

git_clone(
    PROJECT_NAME opensim-models
    GIT_URL https://github.com/opensim-org/opensim-models.git
    DIRECTORY ./vsWorkSpace
    )

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()
file(GLOB visualizer_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsVisualizer/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsVisualizer/*h
    )
file(GLOB modeling_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsModeling/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsModeling/*h
    )
file(GLOB opensim_widgets_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.ui
    )


add_executable(OpensimQtGUI
    main.cpp
    ${opensim_widgets_sources}
    ${modeling_sources}
    ${visualizer_sources}
    resource.qrc
    ${TS_FILES}
)

target_link_libraries(OpensimQtGUI PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES})


qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
