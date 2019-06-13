cmake_minimum_required(VERSION 3.13)
project(Projet_PSE C)

set(CMAKE_C_STANDARD 11)



set(HEADERS
        include/pse.h
        include/dataspec.h
        include/datathread.h
        include/erreur.h
        include/ligne.h
        include/msg.h
        include/msgbox.h
        include/resolv.h
)

set(SRCS
        modules/datathread.c
        modules/erreur.c
        modules/ligne.c
        modules/msgbox.c
        modules/msg.c
        modules/resolv.c
        )

include_directories(.)
include_directories(include)
include_directories(modules)
find_package(Threads)
add_executable(Test_Serveur ${HEADERS} ${SRCS} serveur.c game.c gameStructs.h)
add_executable(Test_Client ${HEADERS} ${SRCS} client.c)
target_link_libraries(Test_Serveur ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Test_Client ${CMAKE_THREAD_LIBS_INIT})





