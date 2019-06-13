#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void Init_Board(Case *Plateau);
void Init_Set_Pieces(Case *Plateau,Piece *Set_Pieces);
void Afficher_Board(Case *Plateau);
void Refresh_Move(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int Indice);
bool Test_Border_Haut1(int Indice);
bool Test_Border_Bas1(int Indice);
bool Test_Border_Gauche1(int Indice);
bool Test_Border_Droit1(int Indice);
bool Test_Border_Haut2(int Indice);
bool Test_Border_Bas2(int Indice);
bool Test_Border_Gauche2(int Indice);
bool Test_Border_Droit2(int Indice);
void Bouger_Piece(Case *Plateau,Piece *Set_Pieces,Move **mouvements);
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





int main(void)
{
    int fin = 0;
	Case Plateau[64];
	Piece Set_Pieces[32];
	Move *mouvements[32];
	Init_Board(Plateau);
	Init_Set_Pieces(Plateau,Set_Pieces);
	Afficher_Board(Plateau);
	while(fin != 1)
	{
        Init_Mouvements(mouvements);
        Bouger_Piece(Plateau,Set_Pieces,mouvements);
        Afficher_Board(Plateau);
        printf("Voulez vous continuer ? 0 : OUI | 1 : NON\n");
        scanf("%d", &fin);
	}
	return 0;
}

void Init_Board(Case *Plateau)
{
	for(int i=0;i<64;i++)
	{
		Plateau[i] = (Case) {.libre = 1, .Occuppant = NULL};
	}
}

void Init_Set_Pieces(Case *Plateau, Piece *Set_Pieces)
{
	for(int i=0;i<=7;i++)
	{
		Set_Pieces[i+8]=(Piece) {.Indice = i+8,.Indice_Case = i+8,.type = 0,.couleur = 1};
		Plateau[i+8]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[i+8]};
		Set_Pieces[i+16]=(Piece) {.Indice = i+16,.Indice_Case = i+48,.type = 0,.couleur = 0};
		Plateau[i+48]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[i+16]};
	}
	Set_Pieces[1]=(Piece) {.Indice = 1,.Indice_Case = 1,.type = 1,.couleur = 1};
	Plateau[1]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[1]};
	Set_Pieces[6]=(Piece) {.Indice = 6,.Indice_Case = 6,.type = 1,.couleur = 1};
	Plateau[6]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[6]};
	Set_Pieces[25]=(Piece) {.Indice = 25,.Indice_Case = 57,.type = 1,.couleur = 0};
	Plateau[57]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[25]};
	Set_Pieces[30]=(Piece) {.Indice = 30,.Indice_Case = 62,.type = 1,.couleur = 0};
	Plateau[62]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[30]};
	Set_Pieces[2]=(Piece) {.Indice = 2,.Indice_Case = 2,.type = 2,.couleur = 1};
	Plateau[2]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[2]};
	Set_Pieces[5]=(Piece) {.Indice = 5,.Indice_Case = 5,.type = 2,.couleur = 1};
	Plateau[5]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[5]};
	Set_Pieces[26]=(Piece) {.Indice = 26,.Indice_Case = 58,.type = 2,.couleur = 0};
	Plateau[58]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[26]};
	Set_Pieces[29]=(Piece) {.Indice = 29,.Indice_Case = 61,.type = 2,.couleur = 0};
	Plateau[61]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[29]};
	Set_Pieces[0]=(Piece) {.Indice = 0,.Indice_Case = 0,.type = 3,.couleur = 1};
	Plateau[0]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[0]};
	Set_Pieces[7]=(Piece) {.Indice = 7,.Indice_Case = 7,.type = 3,.couleur = 1};
	Plateau[7]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[7]};
	Set_Pieces[24]=(Piece) {.Indice = 24,.Indice_Case = 56,.type = 3,.couleur = 0};
	Plateau[56]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[24]};
	Set_Pieces[31]=(Piece) {.Indice = 31,.Indice_Case = 63,.type = 3,.couleur = 0};
	Plateau[63]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[31]};
	Set_Pieces[3]=(Piece) {.Indice = 3,.Indice_Case = 3,.type = 4,.couleur = 1};
	Plateau[3]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[3]};
	Set_Pieces[28]=(Piece) {.Indice = 28,.Indice_Case = 60,.type = 5,.couleur = 0};
	Plateau[60]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[28]};
	Set_Pieces[4]=(Piece) {.Indice = 4,.Indice_Case = 4,.type = 5,.couleur = 1};
	Plateau[4]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[4]};
	Set_Pieces[27]=(Piece) {.Indice = 27,.Indice_Case = 59,.type = 4,.couleur = 0};
    Plateau[59]= (Case) {.libre = 0,.Occuppant = &Set_Pieces[27]};
}

void Afficher_Board(Case *Plateau)
{
	char Type, lettres[]={'a','b','c','d','e','f','g','h'};
	printf(" ");
	for(int i = 0;i<=7;i++) printf(" %c",lettres[i]);
	printf("\n");
	for(int i = 1;i <= 8;i++)
	{
	    printf(" ");
        for(int k = 0;k<=7;k++) printf("--");
        printf("-\n%d",9-i);
		for (int j = 0;j <= 7;j++)
		{
		    if(Plateau[63-8*i+j+1].Occuppant == NULL) Type = ' ';
		    else
            {
                switch(Plateau[63-8*i+j+1].Occuppant->type)
                {
                    case 0 :
                     Type = 'P';
                    break;
                    case 1 :
                     Type = 'C';
                    break;
                    case 2 :
                     Type = 'F';
                    break;
                    case 3 :
                     Type = 'T';
                    break;
                    case 4 :
                     Type = 'Q';
                    break;
                    case 5 :
                     Type = 'R';
                    break;
                }
            }
            printf("|");
            printf("%c",Type);
		}
        printf("|\n");
	}
	printf(" ");
    for( int k = 0;k<=7;k++) printf("--");
    printf("-\n");
}

void Refresh_Move(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int Indice)
{
    Move *Constructeur;
    Liberer(mouvements, Indice);
    switch(Set_Pieces[Indice].type)
    {
        case 0 :
         switch(Set_Pieces[Indice].couleur)
         {
             case 1 :
              if(Plateau[Set_Pieces[Indice].Indice_Case+8].libre == 1)
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                Constructeur->dst = Set_Pieces[Indice].Indice_Case+8;
                Constructeur->Suivant = mouvements[Indice];
                mouvements[Indice] = Constructeur;
              }
              if(Set_Pieces[Indice].Indice_Case <= 15 && Set_Pieces[Indice].Indice_Case >= 8 && Plateau[Set_Pieces[Indice].Indice_Case+16].libre == 1)
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+16,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if( Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case) && Plateau[Set_Pieces[Indice].Indice_Case+9].libre == 0)
              {
                if(Plateau[Set_Pieces[Indice].Indice_Case+9].Occuppant->couleur == 0)
                {
                    Constructeur = (Move*) malloc(sizeof(Move));
                    *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+9,.Suivant = mouvements[Indice]};
                    mouvements[Indice] = Constructeur;
                }
              }
              else if( Test_Border_Droit1(Set_Pieces[Indice].Indice_Case) && Plateau[Set_Pieces[Indice].Indice_Case+7].libre == 0)
              {
                if(Plateau[Set_Pieces[Indice].Indice_Case+7].Occuppant->couleur == 0)
                {
                    Constructeur = (Move*) malloc(sizeof(Move));
                    *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+7,.Suivant = mouvements[Indice]};
                    mouvements[Indice] = Constructeur;
                }
              }
              else
              {
                  if(Plateau[Set_Pieces[Indice].Indice_Case+7].libre == 0)
                  {
                    if(Plateau[Set_Pieces[Indice].Indice_Case+7].Occuppant->couleur == 0)
                    {
                        Constructeur = (Move*) malloc(sizeof(Move));
                        *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+7,.Suivant = mouvements[Indice]};
                        mouvements[Indice] = Constructeur;
                    }
                  }
                  if(Plateau[Set_Pieces[Indice].Indice_Case+9].libre == 0)
                  {
                    if(Plateau[Set_Pieces[Indice].Indice_Case+9].Occuppant->couleur == 0)
                    {
                        Constructeur = (Move*) malloc(sizeof(Move));
                        *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+9,.Suivant = mouvements[Indice]};
                        mouvements[Indice] = Constructeur;
                    }
                  }
              }
            break;
            case 0 :
             if(Plateau[Set_Pieces[Indice].Indice_Case-8].libre == 1)
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-8,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(Set_Pieces[Indice].Indice_Case <= 55 && Set_Pieces[Indice].Indice_Case >= 48 && Plateau[Set_Pieces[Indice].Indice_Case-16].libre == 1)
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-16,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if( Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case) && Plateau[Set_Pieces[Indice].Indice_Case-7].libre == 0)
              {
                if(Plateau[Set_Pieces[Indice].Indice_Case-7].Occuppant->couleur == 1)
                {
                    Constructeur = (Move*) malloc(sizeof(Move));
                    *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-7,.Suivant = mouvements[Indice]};
                    mouvements[Indice] = Constructeur;
                }
              }
              else if( Test_Border_Droit1(Set_Pieces[Indice].Indice_Case) && Plateau[Set_Pieces[Indice].Indice_Case-9].libre == 0)
              {
                if(Plateau[Set_Pieces[Indice].Indice_Case-9].Occuppant->couleur == 1)
                {
                    Constructeur = (Move*) malloc(sizeof(Move));
                    *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-9,.Suivant = mouvements[Indice]};
                    mouvements[Indice] = Constructeur;
                }
              }
              else
              {
                  if(Plateau[Set_Pieces[Indice].Indice_Case-9].libre == 0)
                  {
                    if(Plateau[Set_Pieces[Indice].Indice_Case-9].Occuppant->couleur == 1)
                    {
                        Constructeur = (Move*) malloc(sizeof(Move));
                        *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-9,.Suivant = mouvements[Indice]};
                        mouvements[Indice] = Constructeur;
                    }
                  }
                  if(Plateau[Set_Pieces[Indice].Indice_Case-7].libre == 0)
                  {
                    if(Plateau[Set_Pieces[Indice].Indice_Case-7].Occuppant->couleur == 1)
                    {
                        Constructeur = (Move*) malloc(sizeof(Move));
                        *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-7,.Suivant = mouvements[Indice]};
                        mouvements[Indice] = Constructeur;
                    }
                  }
              }
             break;
         }
         break;
         case 1 :
              if(!Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+15,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Droit1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+17,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Droit1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-15,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-17,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Droit2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Droit1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-6,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Droit2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Droit1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+10,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Gauche2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Bas1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-10,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
              if(!Test_Border_Gauche2(Set_Pieces[Indice].Indice_Case) && !Test_Border_Haut1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case))
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+6,.Suivant = mouvements[Indice]};
                mouvements[Indice] = Constructeur;
              }
            break;
         case 2 :
            Ajouter_Diagonale_HDroite(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_HGauche(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_BGauche(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_BDroite(Plateau, mouvements, Set_Pieces, Indice);


         break;
         case 3 :
            Ajout_Ligne_Bas(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Haut(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Gauche(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Droit(Plateau,mouvements, Set_Pieces, Indice);
         break;
         case 4 :
            Ajouter_Diagonale_BDroite(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_BGauche(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_HDroite(Plateau, mouvements, Set_Pieces, Indice);
            Ajouter_Diagonale_HGauche(Plateau, mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Bas(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Haut(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Gauche(Plateau,mouvements, Set_Pieces, Indice);
            Ajout_Ligne_Droit(Plateau,mouvements, Set_Pieces, Indice);
         break;
         case 5 :
            if(!Test_Border_Haut1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+8,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Droit1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+1,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+1,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Bas1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-8,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Bas1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-9,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Bas1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Droit1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case-7,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Haut1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Gauche1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+7,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
            if(!Test_Border_Haut1(Set_Pieces[Indice].Indice_Case) && !Test_Border_Droit1(Set_Pieces[Indice].Indice_Case))
            {
            Constructeur = (Move*) malloc(sizeof(Move));
            *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+9,.Suivant = mouvements[Indice]};
            mouvements[Indice] = Constructeur;
            }
        break;
    }
}


void Bouger_Piece(Case *Plateau,Piece *Set_Pieces,Move **mouvements)
{
    char origine[2],destination[2];
    int Ind_Origine,Ind_Destination,Ind_Piece1,Ind_Piece2=0,libre_temp = 0,couleur_temp;
    Piece *Occupant_Temp;
    bool Test;
    printf("Veuillez selectionner la piece a deplacer : ");
    scanf("%s", origine);
    Ind_Origine = Coo_to_Ind(origine);
    if(Plateau[Ind_Origine].libre == 1)
    {
        printf("Code non valide.\n");
    }
    Ind_Piece1 = Plateau[Ind_Origine].Occuppant->Indice;
    Ind_Piece2 = Ind_Piece1;
    Refresh_Move(Plateau,Set_Pieces,mouvements,Ind_Piece1);
    Afficher_Move_Valide(Plateau,Set_Pieces,mouvements,Ind_Piece1);
    printf("Veuillez selectionner sa destination : ");
    scanf("%s", destination);
    Ind_Destination = Coo_to_Ind(destination);
    Test = Test_Move_Valide(Plateau,Set_Pieces,mouvements,Ind_Origine,Ind_Destination,Ind_Piece1);
    if(Test)
    {
        Set_Pieces[Ind_Piece1].Indice_Case = Ind_Destination;
        Plateau[Ind_Origine] = (Case) {.libre = 1,.Occuppant = NULL};
        Plateau[Ind_Destination]= (Case) {.libre = 0,.Occuppant = &Set_Pieces[Ind_Piece1]};
        switch(Set_Pieces[Ind_Piece1].couleur)
        {
        case 0:
        if(Test_Echec(Plateau,Set_Pieces,mouvements,1))
        {
            if(Test_Checkmate(Plateau,Set_Pieces,mouvements,1))
            {
                printf("%d en echec et mat",1);
            }
            else{printf("Poire\n");}
        }
        break;
        case 1 :
        if(Test_Echec(Plateau,Set_Pieces,mouvements,0))
           {

               if(Test_Checkmate(Plateau,Set_Pieces,mouvements,0))
               {
                   printf("%d En echec et mat.\n",0);
               }
           }
        break;
        }
    }
    else
    {
        printf("Code non valide.\n");
    }
}

void Liberer(Move **mouvements,int indice)
{
    mouvements[indice] = NULL;
}

void Init_Mouvements(Move **mouvements)
{
    for(int i = 0;i<32;i++)
    {
        mouvements[i]=NULL;
    }
}

int Coo_to_Ind(char *code)
{
        return (int) (code[0]-97+(code[1]-49)*8);
}

bool Test_Border_Haut1(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == 63-i)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Gauche1(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == i*8)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Droit1(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == 8*i+7)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Bas1(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == i)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Haut2(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == 48+i)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Gauche2(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == i*8+1)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Droit2(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == i*8+6)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Bas2(int Indice)
{
    for(int i = 0;i<=7;i++)
    {
        if(Indice == 8+i)
        {
            return true;
        }
    }
    return false;
}


void Ajout_Ligne_Gauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Gauche1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i--;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajout_Ligne_Haut(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Haut1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i+=8;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajout_Ligne_Bas(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Bas1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i-=8;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajout_Ligne_Droit(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Droit1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i++;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajouter_Diagonale_HGauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Gauche1(i) && !Test_Border_Haut1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i+=7;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajouter_Diagonale_HDroite(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Droit1(i) && !Test_Border_Haut1(i) && ((Plateau[i].libre != 0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i+=9;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajouter_Diagonale_BGauche(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Gauche1(i) && !Test_Border_Bas1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i-=9;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

void Ajouter_Diagonale_BDroite(Case *Plateau, Move **mouvements,Piece *Set_Pieces,int Indice_Piece)
{
    Move *Constructeur;
    int i = Set_Pieces[Indice_Piece].Indice_Case;
    while(!Test_Border_Droit1(i) && !Test_Border_Bas1(i) && ((Plateau[i].libre!=0 || i == Set_Pieces[Indice_Piece].Indice_Case)))
    {
        i-=7;
        Constructeur = (Move*) malloc(sizeof(Move));
        *Constructeur = (Move) {.dst = i,.Suivant = mouvements[Indice_Piece]};
        mouvements[Indice_Piece] = Constructeur;
    }
}

bool Test_Border1(int Indice)
{
    if (Test_Border_Haut1(Indice) || Test_Border_Bas1(Indice) || Test_Border_Gauche1(Indice) || Test_Border_Droit1(Indice)) return true;
    return false;
}

bool Test_Border2(int Indice)
{
    if (Test_Border_Haut2(Indice) || Test_Border_Bas2(Indice) || Test_Border_Gauche2(Indice) || Test_Border_Droit2(Indice)) return true;
    return false;
}

bool Test_Move_Liste(int Indice,int Ind_Destination,Move **mouvements)
{
    bool Test = false;
    Move *Curseur;
    if(mouvements[Indice] != NULL)
    {
    Curseur = mouvements[Indice];
    while(Curseur != NULL && Test == false)
    {
        if(Ind_Destination == Curseur->dst) Test = true;
        Curseur = Curseur->Suivant;
    }
    return Test;
    }
    return false;
}

bool Test_Echec(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int couleur)
{
    bool Test=false;
    Move *Curseur;

    if(couleur == 0)
    {
        for(int i=0;i<=15;i++)
        {
            Refresh_Move(Plateau,Set_Pieces,mouvements,i);
        }
        for(int i = 0;i<=15;i++)
        {
            Curseur = mouvements[i];
            while(Curseur!=NULL)
            {
                if(Set_Pieces[28].Indice_Case == Curseur->dst )
                {
                    Test = true;
                }
                Curseur = Curseur->Suivant;
            }
        }
        return Test;
    }
    else if(couleur == 1)
    {
        for(int i=16;i<=31;i++)
        {
            Refresh_Move(Plateau,Set_Pieces,mouvements,i);
        }
        for(int i = 16;i<=31;i++)
        {
            Curseur = mouvements[i];
            while(Curseur!=NULL)
            {
                if(Set_Pieces[4].Indice_Case == Curseur->dst)
                {
                    Test = true;
                }
                Curseur = Curseur->Suivant;
            }
        }
        return Test;
    }
    return Test;
}

void Afficher_Move(Move **mouvements, int Indice)
{
    Move *Curseur = mouvements[Indice];
    while(Curseur != NULL)
    {
        printf("%d\n",Curseur->dst);
        Curseur = Curseur->Suivant;
    }
}

bool Test_Move_Valide(Case *Plateau,Piece *Set_Pieces, Move **mouvements,int Ind_Origine,int Ind_Destination,int Ind_Piece)
{
    Piece *Occupant_Temp;
    int libre_temp;
    bool Test = Test_Move_Liste(Ind_Piece, Ind_Destination, mouvements);
    if(Test)
    {
        if(Plateau[Ind_Destination].libre == 1 || (Plateau[Ind_Destination].libre == 0 && Plateau[Ind_Destination].Occuppant->couleur != Set_Pieces[Ind_Piece].couleur))
        {
        Occupant_Temp = Plateau[Ind_Destination].Occuppant;
        libre_temp = Plateau[Ind_Destination].libre;
        Set_Pieces[Ind_Piece].Indice_Case = Ind_Destination;
        Plateau[Ind_Origine] = (Case) {.libre = 1,.Occuppant = NULL};
        Plateau[Ind_Destination]= (Case) {.libre = 0,.Occuppant = &Set_Pieces[Ind_Piece]};
        if(Test_Echec(Plateau,Set_Pieces,mouvements,Set_Pieces[Ind_Piece].couleur))
        {
                    Test = false;
        }
            Set_Pieces[Ind_Piece].Indice_Case = Ind_Origine;
            Plateau[Ind_Origine] = (Case) {.libre = 0,.Occuppant = &Set_Pieces[Ind_Piece]};
            Plateau[Ind_Destination]= (Case) {.libre = libre_temp,.Occuppant = Occupant_Temp};
        }
        else
        {
            Test = false;
        }
    }
    return Test;
}

bool Test_Checkmate(Case *Plateau,Piece *Set_Pieces,Move **mouvements,int couleur)
{
    bool Test=true;
    Move *Curseur;
    if(couleur == 0)
    {
        for(int i=16;i<=31;i++)
        {
            Refresh_Move(Plateau,Set_Pieces,mouvements,i);
        }
        for(int i = 16;i<=31;i++)
        {
            Curseur = mouvements[i];
            while(Curseur!=NULL)
            {
                if(Test_Move_Valide(Plateau,Set_Pieces,mouvements,Set_Pieces[i].Indice_Case,Curseur->dst,i))
                {
                    Test=false;
                }
                Curseur = Curseur->Suivant;
            }
        }
        return Test;
    }
    else if(couleur == 1)
    {
        for(int i=0;i<=15;i++)
        {
            Refresh_Move(Plateau,Set_Pieces,mouvements,i);
        }
        for(int i = 0;i<=15;i++)
        {
            Curseur = mouvements[i];
            while(Curseur!=NULL)
            {
                if(Test_Move_Valide(Plateau,Set_Pieces,mouvements,Set_Pieces[i].Indice_Case,Curseur->dst,i))
                {
                    Test=false;
                }
                Curseur = Curseur->Suivant;
            }
        }
    }
    return Test;
}

void Afficher_Move_Valide(Case* Plateau,Piece *Set_Pieces,Move ** mouvements,int Indice)
{
    Move* Curseur=mouvements[Indice];
    while(Curseur!=NULL)
    {
        if(Test_Move_Valide(Plateau,Set_Pieces,mouvements,Set_Pieces[Indice].Indice_Case,Curseur->dst,Indice))
        {
            printf("%d\n",Curseur->dst);
        }
        Curseur=Curseur->Suivant;
    }
}
