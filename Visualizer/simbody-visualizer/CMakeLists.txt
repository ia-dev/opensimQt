# Generate the visualizer application and deal with its dependencies
# on OpenGL and glut. On Windows we're going to install our own glut; for
# other platforms we depend on their already being a glut (or freeglut)
# available.

find_package(OpenGL)
    find_package(GLUT) # sets GLUT_LIBRARIES
        if(GLUT_FOUND AND GLUT_Xmu_LIBRARY AND GLUT_Xi_LIBRARY)
            set(SIMBODY_HAS_GLUT TRUE)
        else()
            set(SIMBODY_HAS_GLUT FALSE)
        endif()

if(OPENGL_FOUND AND SIMBODY_HAS_GLUT)

add_executable(${GUI_NAME} MACOSX_BUNDLE
    simbody-visualizer.cpp lodepng.cpp lodepng.h
    ${GLUT32_HEADERS}) # only on Windows

target_include_directories(${GUI_NAME} ${GLUT_INCLUDE_DIR})
set_target_properties(${GUI_NAME} PROPERTIES      PROJECT_LABEL "Code - ${GUI_NAME}"        DEBUG_OUTPUT_NAME ${GUI_NAME}$ {CMAKE_DEBUG_POSTFIX})

 if(BUILD_DYNAMIC_LIBRARIES)
    target_link_libraries(${GUI_NAME} ${TEST_SHARED_TARGET})
else()
    target_link_libraries(${GUI_NAME} ${TEST_STATIC_TARGET})
endif()
    
target_link_libraries(${GUI_NAME} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})


