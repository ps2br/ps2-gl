cmake_minimum_required(VERSION 3.16)

project(ps2gl C)

add_library(ps2gl
    STATIC
    src/matrix.c
    src/context.c
    src/clear.c
    src/viewport.c
    src/draw.c
    src/color.c
    src/vertex.c
    src/common.c
    src/texture.c
    src/render.c)

target_include_directories(ps2gl PRIVATE include/)

target_link_libraries(ps2gl PRIVATE
    m
    math3d
    gskit
    gskit_toolkit)

install(
    TARGETS ps2gl
    ARCHIVE DESTINATION lib)

install(
    DIRECTORY include/
    DESTINATION include)
