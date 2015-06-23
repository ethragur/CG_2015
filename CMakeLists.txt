cmake_minimum_required(VERSION 2.6)
project(cg_2015)

set(CMAKE_CXX_FLAGS "-g -I../ ${CMAKE_CXX_FLAGS} -std=c++0x")



#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()
#########################################################
# Include Files
#########################################################
add_executable(cg_2015 MerryGoRound.cpp InitClass.cpp GraphicsObject.cpp objloader.cpp tiny_obj_loader.cpp InitClass.cpp camera.cpp InputManager.cpp Billboard.cpp LightSource.cpp SOIL/SOIL.c SOIL/image_helper.c SOIL/image_DXT.c SOIL/stb_image_aug.c)

########################################################
# Linking & stuff
#########################################################


install(TARGETS cg_2015 RUNTIME DESTINATION bin)
target_link_libraries(cg_2015 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )