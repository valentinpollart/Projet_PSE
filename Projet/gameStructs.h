//
// Created by magister on 13/06/19.
//

#ifndef PROJET_PSE_GAMESTRUCTS_H
#define PROJET_PSE_GAMESTRUCTS_H

#include "pse.h"

typedef struct _player{
    int id;
    char pseudo[20];
    sem_t input;
    sem_t challenged;
    DataThread* handler;
    struct _player *next;
    char kbInput[20];
    struct _player *challenger;
    sem_t gameDone;


}player;

typedef struct{
    pthread_t id;
    player *players[2];
}game;




#endif //PROJET_PSE_GAMESTRUCTS_H
