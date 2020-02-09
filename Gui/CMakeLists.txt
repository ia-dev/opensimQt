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

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES OpensimQtGUI_en_US.ts)

#adding opensim to the project
#after building and installing the project add the /bin
#to the system path
find_package(OpenSim REQUIRED)

if(ANDROID)
  add_library(OpensimQtGUI SHARED
    main.cpp
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
target_link_libraries(OpensimQtGUI PRIVATE Qt5::Widgets ${OpenSim_LIBRARIES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
