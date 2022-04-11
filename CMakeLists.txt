cmake_minimum_required(VERSION 3.0.2)

project(CGFrameWork)

INCLUDE_DIRECTORIES (
#   D:/Develope/Libs/Glut/include
)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

find_package(OpenGL)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET( CGFrameWork_SRCS
   main.cpp
   RenderWidget.cpp
   RenderWidget.h
   PPUViewer.cpp
   PPUViewer.h
)

LINK_LIBRARIES(
#   D:/Develope/Libs/Glut/Lib/glut32.lib
#   ${OPENGL_glu_LIBRARY}
#   ${OPENGL_LIBRARY}
)


add_executable(CGFrameWork ${CGFrameWork_SRCS})
target_link_libraries (CGFrameWork Qt5::Widgets)

#qt5_use_modules(CGFrameWork Widgets OpenGL)
#target_link_libraries(CGFrameWork ${QT_LIBRARIES} ${OPENGL_LIBRARIES})