cmake_minimum_required(VERSION 3.10)

project(opensimQt LANGUAGES CXX)

set(TARGET opensimQt CACHE TYPE STRING)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(TS_FILES ${TARGET}.ts)

if (WIN32)
    message(STATUS ">>> ..............  WINDOWS .............")
elseif(UNIX AND NOT APPLE)
    message(STATUS ">>> ...............  UNIX .............")
elseif (UNIX AND APPLE)
     message(STATUS ">>>...............  MAC .............")
else()
     message(STATUS ">>> ............. UNKNOWN SYSTEM .............")
endif ()

#---------------------------------------------------------
#              Qt5
#---------------------------------------------------------
#set(Qt5_DIR "/Users/ritesh/Qt/5.15.0/clang_64/lib/cmake/Qt5")
find_package(Qt5 COMPONENTS Widgets WebEngine WebEngineWidgets OpenGL LinguistTools REQUIRED)

#---------------------------------------------------------
#              OpenSim
#---------------------------------------------------------
#adding opensim to the project
#after building and installing the project add the /bin to the system path
#set(OpenSim_DIR "/Users/ritesh/projects/idhamari/VisSimKoblenz/opensim_install/cmake")
#set(OpenSim_DIR "/home/ibr/myGitLab/opensim-dev/IA/install-opensim-core/lib/cmake/OpenSim")
find_package(OpenSim REQUIRED)

#---------------------------------------------------------
#              VTK 9
#---------------------------------------------------------
#adding the vtk modules
#set(VTK_DIR "/Users/ritesh/projects/idhamari/VisSimKoblenz/vtk_build/lib/cmake/vtk-9.0")
find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersCore
  vtkInfovisCore
  vtkInteractionStyle
  vtkViewsQt
  vtkIOImage
  vtkIOGeometry
  vtkCommonColor
  vtkCommonDataModel
  vtkCommonTransforms
  vtkFiltersSources
  vtkFiltersTexture
  vtkRenderingAnnotation
  vtkRenderingCore
  vtkRenderingFreeType
  vtkIOXML
  vtkCommonComputationalGeometry
  vtkCommonDataModel
  vtkRenderingOpenGL2
  QUIET
)
#include(${VTK_USE_FILE})
#copy the files used by vtk
file(GLOB VtkImages
    ${CMAKE_SOURCE_DIR}/Data/Images/vtk_images/*.png
    ${CMAKE_SOURCE_DIR}/Data/Images/vtk_images/skybox1/*.png
    )
file(COPY ${VtkImages}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/vtk_images)

file(GLOB templateModels
     ${CMAKE_SOURCE_DIR}/Data/templates/*.osim
     )
file(COPY ${templateModels}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/templates)

#---------------------------------------------------------
#              PYTHONQT
#---------------------------------------------------------
# TODO add option to select python version
# TODO add option to locate PYTHONQT
#set (PyVer 38)
set (PyVer "" CACHE STRING "python version")
find_package(PythonLibs REQUIRED)
if(${PYTHONLIBS_FOUND})
    #PythonQt Package
     message(WARNING "Python ${PYTHONLIBS_VERSION_STRING} was found")
     message(WARNING "Python ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR} was found")

     #set(PYTHONQT_SRC_PATH "F:/GitClones/pythonqt" CACHE STRING "")
     #set(PYTHONQT_PATH "F:/GitClones/build-PythonQt-Desktop_Qt_5_14_1_MSVC2017_64bit-Release" CACHE STRING "")
     #set(PYTHONQT_SRC_PATH "/home/ibr/opensim-dev/pyhtonQT-3.2" CACHE STRING "")
     #set(PYTHONQT_PATH "/home/ibr/opensim-dev/pyhtonQT-3.2-build" CACHE STRING "")
     set(PYTHONQT_SRC_PATH "" CACHE STRING "path tp pythonqt source ")
     set(PYTHONQT_PATH ""     CACHE STRING  "path to pythonqt build")
     find_library(PYTHONQT PythonQt-Qt5-Python${PyVer} PATHS ${PYTHONQT_PATH}/lib NO_DEFAULT_PATH)
     find_library(PYTHONQTALL PythonQt_QtAll-Qt5-Python${PyVer}  PATHS ${PYTHONQT_PATH}/lib NO_DEFAULT_PATH)
     include_directories(${PYTHONQT_SRC_PATH}/src)
     include_directories(${PYTHONQT_SRC_PATH}/extensions/PythonQt_QtAll)
 else()
    message(WARNING "Python was not found today, make sure you have the same python version for PythonQt")
    message(STATUS  "Skipping PythonQt")
endif()


#---------------------------------------------------------
#              Git downloads
#---------------------------------------------------------
include(cmake/GitUtils.cmake)
option(Download_Geometry "Download geometry, models, and other files." False)
if (Download_Geometry)
   message(STATUS "Downloading geometry, models and other files to vsWorkSpace")
   git_clone(
         PROJECT_NAME opensim-models
         GIT_URL https://github.com/opensim-org/opensim-models.git
         DIRECTORY ./vsWorkSpace
    )
else()
   message(WARNING "Geometry files are not downloaded!")
endif()

#---------------------------------------------------------
#              Add source files
#---------------------------------------------------------
file(GLOB visualizer_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsVisualizer/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsVisualizer/*h
    )
file(GLOB modeling_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsModeling/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsModeling/*h
    )
file(GLOB tools_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsTools/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsTools/*h
    )
file(GLOB opensim_widgets_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vsWidgets/*.ui
    )
file(GLOB opensim_python_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/vsPython/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vsPython/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vsPython/*.ui
    )

#---------------------------------------------------------
#              Building and linking
#---------------------------------------------------------
add_executable(${TARGET}
    main.cpp
    ${opensim_widgets_sources}
    ${modeling_sources}
    ${visualizer_sources}
    ${tools_sources}
    ${opensim_python_sources}
    resource.qrc
    ${TS_FILES}
)

if (WIN32)
    if(${PYTHONLIBS_FOUND})
       message(STATUS "linking opensimQt OpenSim VTK PythonQt ...................... ")
       target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL 
                          ${OpenSim_LIBRARIES} ${VTK_LIBRARIES}  ${PYTHONQT} ${PYTHONQTALL} ${PYTHON_LIBRARIES})
       #includes for PythonQt
       target_include_directories(${TARGET} PRIVATE ${PYTHON_INCLUDE_PATH} ${PYTHONQT_SRC_PATH}/src ${PYTHONQT_SRC_PATH}/extensions/PythonQt_QtAll)
   else()
       message(STATUS "linking opensimQt OpenSim VTK ...................... ")
       target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES} )
   endif()

elseif(UNIX AND NOT APPLE)
    if(${PYTHONLIBS_FOUND})
        message(STATUS "linking opensimQt OpenSim VTK PythonQt ...................... ")
        target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL 
                             ${OpenSim_LIBRARIES} ${VTK_LIBRARIES}  ${PYTHONQT} ${PYTHONQTALL} ${PYTHON_LIBRARIES})
        #includes for PythonQt
        target_include_directories(${TARGET} PRIVATE ${PYTHON_INCLUDE_PATH} ${PYTHONQT_SRC_PATH}/src ${PYTHONQT_SRC_PATH}/extensions/PythonQt_QtAll)
    else()
        message(STATUS "linking opensimQt OpenSim VTK ...................... ")
        target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES} )
    endif()
elseif (UNIX AND APPLE)
    if(${PYTHONLIBS_FOUND})
        message(STATUS "linking opensimQt OpenSim VTK PythonQt ...................... ")
       target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES}  ${PYTHONQT} ${PYTHONQTALL} Python::Module)
       #includes for PythonQt
       target_include_directories(${TARGET} PRIVATE ${PYTHON_INCLUDE_PATH} ${PYTHONQT_SRC_PATH}/src ${PYTHONQT_SRC_PATH}/extensions/PythonQt_QtAll)
   else()
       message(STATUS "linking opensimQt OpenSim VTK ...................... ")
       target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets Qt5::OpenGL  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES} )
   endif()

endif ()

vtk_module_autoinit(  TARGETS ${TARGET}   MODULES ${VTK_LIBRARIES} )

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})




