cmake_minimum_required(VERSION 3.16)

project(ps2gl_sample C)

add_executable(ps2gl_sample
    src/main.c)

target_link_libraries(ps2gl_sample PRIVATE
    ps2gl
    packet
    draw
    graph
    math3d
    gskit_toolkit
    gskit
    dmakit
    -ldebug)