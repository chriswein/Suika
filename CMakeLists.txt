cmake_minimum_required(VERSION 3.10)

project(Suika)

find_package(raylib CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

include_directories(./include)
set(SOURCES main.cc box.cc helpers.cc suika.cc contactlistener.cc game.cc gui.cc ./include/raylib.h ./include/box2d.h)

set(CMAKE_CXX_STANDARD 14)
add_executable(Suika ${SOURCES})
#if(APPLE){
target_link_libraries(Suika PRIVATE "-framework Cocoa")
target_link_libraries(Suika PRIVATE "-framework CoreVideo")
target_link_libraries(Suika PRIVATE "-framework IOKIT")
target_link_libraries(Suika PRIVATE "-framework GLUT")
target_link_libraries(Suika PRIVATE "-framework OpenGL")
#}

target_link_libraries(Suika PRIVATE raylib)
target_link_libraries(Suika PRIVATE box2d::box2d)
target_include_directories(Suika PRIVATE ./include})
