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
void Liberer(Move *mouvements);
void Init_Mouvements(Move **mouvements);
int Coo_to_Ind(char *code);



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
	Set_Pieces[28]=(Piece) {.Indice = 28,.Indice_Case = 60,.type = 4,.couleur = 0};
	Plateau[60]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[28]};
	Set_Pieces[4]=(Piece) {.Indice = 4,.Indice_Case = 4,.type = 5,.couleur = 1};
	Plateau[4]=(Case) {.libre = 0,.Occuppant = &Set_Pieces[4]};
	Set_Pieces[27]=(Piece) {.Indice = 27,.Indice_Case = 59,.type = 5,.couleur = 0};
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
    int Curseur;
    Liberer(mouvements[Indice]);
    switch(Set_Pieces[Indice].type)
    {
        case 0 :
         switch(Set_Pieces[Indice].couleur)
         {
             case 1 :
              if(Plateau[Set_Pieces[Indice].Indice_Case+8].libre == 1)
              {
                Constructeur = (Move*) malloc(sizeof(Move));
                *Constructeur = (Move) {.dst = Set_Pieces[Indice].Indice_Case+8,.Suivant = mouvements[Indice]};
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
    }
}


void Bouger_Piece(Case *Plateau,Piece *Set_Pieces,Move **mouvements)
{
    char origine[2],destination[2];
    int Ind_Origine,Ind_Destination,Ind_Piece1,Ind_Piece2;
    bool Test;
    Move *Curseur;
    printf("Veuillez selectionner la piece a deplacer : ");
    scanf("%s", origine);
    Ind_Origine = Coo_to_Ind(origine);
    if(Plateau[Ind_Origine].libre == 1)
    {
        printf("Code non valide.\n");
    }
    Ind_Piece1 = Plateau[Ind_Origine].Occuppant->Indice;
    Refresh_Move(Plateau,Set_Pieces,mouvements,Ind_Piece1);
    printf("Veuillez selectionner sa destination : ");
    scanf("%s", destination);
    Ind_Destination = Coo_to_Ind(destination);
    Curseur = mouvements[Ind_Piece1];
    Test = false;
    while(Curseur != NULL)
    {
        if(Ind_Destination == Curseur->dst) Test = true;
        Curseur = Curseur->Suivant;
    }
    if(Test)
    {
        Set_Pieces[Ind_Piece1].Indice_Case = Ind_Destination;
        Plateau[Ind_Origine] = (Case) {.libre = 1,.Occuppant = NULL};
        Plateau[Ind_Destination]= (Case) {.libre = 0,.Occuppant = &Set_Pieces[Ind_Piece1]};
    }
    else
    {
        printf("Code non valide.\n");
    }
}

void Liberer(Move *mouvements)
{
    Move *Curseur = mouvements;
    while(Curseur != NULL)
    {
        Curseur = Curseur->Suivant;
        free(mouvements);
        mouvements = Curseur;
    }
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
    for(int i = 0;i<=5;i++)
    {
        if(Indice == 49+i)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Gauche2(int Indice)
{
    for(int i = 0;i<=5;i++)
    {
        if(Indice == (i+1)*8+1)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Droit2(int Indice)
{
    for(int i = 0;i<=5;i++)
    {
        if(Indice == (i+1)*8+6)
        {
            return true;
        }
    }
    return false;
}

bool Test_Border_Bas2(int Indice)
{
    for(int i = 0;i<=5;i++)
    {
        if(Indice == 9+i)
        {
            return true;
        }
    }
    return false;
}
