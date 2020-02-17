cmake_minimum_required(VERSION 3.5)

project(OpensimQtGUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets WebEngine WebEngineWidgets  LinguistTools REQUIRED)

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

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

if(ANDROID)
  add_library(OpensimQtGUI SHARED
    main.cpp
    Modeling/onegeometrynode.h
    Modeling/onegeometrynode.cpp
    Modeling/onewrapobjectnode.h
    Modeling/onewrapobjectnode.cpp
    Modeling/oneframenode.h
    Modeling/oneframenode.cpp
    Modeling/modelnode.h
    Modeling/modelnode.cpp
    Modeling/navigatornode.h
    Modeling/navigatornode.cpp
    Modeling/navigatormodel.h
    Modeling/navigatormodel.cpp
    OpenSimWidgets/mainwindow.cpp
    OpenSimWidgets/mainwindow.h
    OpenSimWidgets/mainwindow.ui
    OpenSimWidgets/simulationtoolswidget.cpp
    OpenSimWidgets/simulationtoolswidget.h
    OpenSimWidgets/simulationtoolswidget.ui
    resource.qrc
    ${TS_FILES}
  )
else()
  add_executable(OpensimQtGUI
    main.cpp
    Modeling/oneprobnode.h
    Modeling/oneprobnode.cpp
    Modeling/probesnode.h
    Modeling/probesnode.cpp
    Modeling/controlnode.h
    Modeling/controlnode.cpp
    Modeling/controlersnode.h
    Modeling/controlersnode.cpp
    Modeling/onemarkernode.h
    Modeling/onemarkernode.cpp
    Modeling/markersnode.h
    Modeling/markersnode.cpp
    Modeling/oneotherforcenode.h
    Modeling/oneotherforcenode.cpp
    Modeling/otherforcesnode.h
    Modeling/otherforcesnode.cpp
    Modeling/onecontactforcenode.h
    Modeling/onecontactforcenode.cpp
    Modeling/contactforcesnode.h
    Modeling/contactforcesnode.cpp
    Modeling/oneactuatornode.h
    Modeling/oneactuatornode.cpp
    Modeling/actuatorsnode.h
    Modeling/actuatorsnode.cpp
    Modeling/musclegroupenode.h
    Modeling/musclegroupenode.cpp
    Modeling/onemusclenode.h
    Modeling/onemusclenode.cpp
    Modeling/musclesnode.h
    Modeling/musclesnode.cpp
    Modeling/allforcesnode.h
    Modeling/allforcesnode.cpp
    Modeling/onecontactgeometrynode.h
    Modeling/onecontactgeometrynode.cpp
    Modeling/contactgeometriesnode.h
    Modeling/contactgeometriesnode.cpp
    Modeling/oneconstraintnode.h
    Modeling/oneconstraintnode.cpp
    Modeling/constraintsnode.h
    Modeling/constraintsnode.cpp
    Modeling/bodiesnode.h
    Modeling/bodiesnode.cpp
    Modeling/onedofnode.h
    Modeling/onedofnode.cpp
    Modeling/onejointnode.h
    Modeling/onejointnode.cpp
    Modeling/jointsnode.h
    Modeling/jointsnode.cpp
    Modeling/groundnode.h
    Modeling/groundnode.cpp
    Modeling/onegeometrynode.h
    Modeling/onegeometrynode.cpp
    Modeling/onewrapobjectnode.h
    Modeling/onewrapobjectnode.cpp
    Modeling/onebodynode.h
    Modeling/onebodynode.cpp
    Modeling/oneframenode.h
    Modeling/oneframenode.cpp
    Modeling/modelnode.h
    Modeling/modelnode.cpp
    Modeling/navigatornode.h
    Modeling/navigatornode.cpp
    Modeling/navigatormodel.h
    Modeling/navigatormodel.cpp
    OpenSimWidgets/mainwindow.cpp
    OpenSimWidgets/mainwindow.h
    OpenSimWidgets/mainwindow.ui
    OpenSimWidgets/simulationtoolswidget.cpp
    OpenSimWidgets/simulationtoolswidget.h
    OpenSimWidgets/simulationtoolswidget.ui
    resource.qrc
    ${TS_FILES}
  )
endif()
target_link_libraries(OpensimQtGUI PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets  ${OpenSim_LIBRARIES} ${VTK_LIBRARIES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
