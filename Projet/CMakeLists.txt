cmake_minimum_required(VERSION 3.13)
project(Projet_PSE C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(./include, ./modules)

add_executable(Projet_PSE main.c)
add_executable(Test_Serveur serveur.c include/pse.h)
