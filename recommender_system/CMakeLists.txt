add_compile_options(-std=c++11)

cmake_minimum_required(VERSION 2.8.12.2)

project(recommender_system)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(recommender_system ${SOURCES})
