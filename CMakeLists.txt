cmake_minimum_required(VERSION 3.25.1)

project(opencv_gtk_test)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_search_module(EPOXY REQUIRED epoxy)
#pkg_check_modules(GTK4 REQUIRED)

include_directories(${GTK3_INCLUDE_DIRS} ${EPOXY_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${EPOXY_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} ${EPOXY_CFLAGS_OTHER})

add_executable(gtk_gl_window_sample src/gtk_gl_window_sample.cpp)

target_link_libraries(gtk_gl_window_sample ${GTK3_LIBRARIES} ${EPOXY_LIBRARIES} GL GLU)
