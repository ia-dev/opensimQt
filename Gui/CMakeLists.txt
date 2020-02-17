cmake_minimum_required(VERSION 3.5)

project(OpensimQtGUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets WebEngine WebEngineWidgets  LinguistTools REQUIRED)

set(TS_FILES OpensimQtGUI_en_US.ts)

#adding opensim to the project
#after building and installing the project add the /bin
#to the system path
find_package(OpenSim REQUIRED)

  add_executable(OpensimQtGUI

    main.cpp
   
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneprobnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneprobnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/probesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/probesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/controlnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/controlnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/controlersnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/controlersnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onemarkernode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onemarkernode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/markersnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/markersnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneotherforcenode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneotherforcenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/otherforcesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/otherforcesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onecontactforcenode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onecontactforcenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/contactforcesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/contactforcesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneactuatornode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneactuatornode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/actuatorsnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/actuatorsnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/musclegroupenode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/musclegroupenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onemusclenode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onemusclenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/musclesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/musclesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/allforcesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/allforcesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onecontactgeometrynode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onecontactgeometrynode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/contactgeometriesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/contactgeometriesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneconstraintnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneconstraintnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/constraintsnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/constraintsnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/bodiesnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/bodiesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onedofnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onedofnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onejointnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onejointnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/jointsnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/jointsnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/groundnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/groundnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onegeometrynode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onegeometrynode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onewrapobjectnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onewrapobjectnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onebodynode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/onebodynode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneframenode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/oneframenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/modelnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/modelnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/navigatornode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/navigatornode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/navigatormodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Modeling/navigatormodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/simulationtoolswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/simulationtoolswidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSimWidgets/simulationtoolswidget.ui
    
    resource.qrc
    ${TS_FILES}
  )

target_link_libraries(OpensimQtGUI PRIVATE Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets  ${OpenSim_LIBRARIES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
