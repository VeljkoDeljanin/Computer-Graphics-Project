cmake_minimum_required(VERSION 3.17)

add_library(stb_image src/stb_image.cpp include/stb_image.h)

target_include_directories(stb_image PUBLIC include)