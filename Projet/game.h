//
// Created by magister on 13/06/19.
//

#ifndef PROJET_PSE_GAME_H
#define PROJET_PSE_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gameStructs.h"

typedef struct _Move{
    int dst;
    struct _Move *Suivant;
}Move;

typedef struct{
    int Indice;
    int Indice_Case;
    int type;
    int couleur;
}Piece;

typedef struct{
    int libre;
    Piece *Occuppant;
}Case;


void *start(void *args);
void Init_Board(Case *Plateau);
void Init_Set_Pieces(Case *Plateau,Piece *Set_Pieces);
void Afficher_Board(Case *Plateau,game *Game);
void Refresh_Move(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int Indice);
bool Test_Border_Haut1(int Indice);
bool Test_Border_Bas1(int Indice);
bool Test_Border_Gauche1(int Indice);
bool Test_Border_Droit1(int Indice);
bool Test_Border_Haut2(int Indice);
bool Test_Border_Bas2(int Indice);
bool Test_Border_Gauche2(int Indice);
bool Test_Border_Droit2(int Indice);
int Bouger_Piece(Case *Plateau,Piece *Set_Pieces,Move **mouvements,game *Game, int playing);
void Liberer(Move **mouvements,int indice);
void Init_Mouvements(Move **mouvements);
bool Test_Echec(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int couleur);
bool Test_Move_Liste(int Indice,int Ind_Destination,Move **mouvements);
int Coo_to_Ind(char *code);
bool Test_Border1(int Indice);
bool Test_Border2(int Indice);
void Ajout_Ligne_Gauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajout_Ligne_Droit(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajout_Ligne_Bas(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajout_Ligne_Haut(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajouter_Diagonale_HGauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajouter_Diagonale_BGauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajouter_Diagonale_HDroite(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Ajouter_Diagonale_BDroite(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece);
void Afficher_Move(Move **mouvements, int Indice);
bool Test_Move_Valide(Case *Plateau,Piece *Set_Pieces, Move **mouvements,int Ind_Origine,int Ind_Destination,int Indice_Piece);
bool Test_Checkmate(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int couleur);
void Afficher_Move_Valide(Case* Plateau,Piece *Set_Pieces,Move ** mouvements,int Indice);

#endif //PROJET_PSE_GAME_H
