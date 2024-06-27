cmake_minimum_required(VERSION 3.25.1)

project(opencv_gtk_test)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_search_module(EPOXY REQUIRED epoxy)
# pkg_check_modules(GTK4 REQUIRED)

# GTK3+ and OpenGL Project
add_executable(gtk_gl_window_sample src/gtk_gl_window_sample.cpp)
target_include_directories(gtk_gl_window_sample PRIVATE ${GTK3_INCLUDE_DIRS} ${EPOXY_INCLUDE_DIRS})
target_link_libraries(gtk_gl_window_sample ${GTK3_LIBRARIES} ${EPOXY_LIBRARIES} GL GLU)
target_link_directories(gtk_gl_window_sample PRIVATE ${GTK3_LIBRARY_DIRS} ${EPOXY_LIBRARY_DIRS})

add_executable(gtk_gl_window_simple src/gtk_gl_window_simple.cpp)
target_include_directories(gtk_gl_window_simple PRIVATE ${GTK3_INCLUDE_DIRS} ${EPOXY_INCLUDE_DIRS})
target_link_libraries(gtk_gl_window_simple ${GTK3_LIBRARIES} ${EPOXY_LIBRARIES} GL GLU)
target_link_directories(gtk_gl_window_simple PRIVATE ${GTK3_LIBRARY_DIRS} ${EPOXY_LIBRARY_DIRS})

# OpenGL + GTK3 + rotation example in OpenCV
add_definitions(-DSAMPLE_IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/lena.jpg")
add_executable(gtk3_opengl_image_rot src/gtk3_opengl_image_rot.cpp)
target_include_directories(gtk3_opengl_image_rot PRIVATE ${GTK3_INCLUDE_DIRS} ${EPOXY_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(gtk3_opengl_image_rot ${GTK3_LIBRARIES} ${EPOXY_LIBRARIES} GL GLU ${OpenCV_LIBRARIES})

# Testing OpenCV GUI built with OpenCV supported by GTK3+ and OpenGL
add_executable(opencv_window_sample src/opencv_window_sample.cpp)
target_include_directories(opencv_window_sample PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_window_sample ${OpenCV_LIBRARIES} GL GLU)
