
#include "game.h"


#define    CMD      "serveur"





int fdJournal;
short port;
sem_t MainSem;
int availablePlayers;



player* playerList;


void *communicationThread(void *arg);
void *inputThread(void *arg);
int remiseAZeroJournal(int fdJournal);
void createPool(int slaveCount);
DataThread *findFreeDataThread();
player* bindPlayerToThread(DataThread *dataThread);
void unbindPlayer(player* Player);
void showPlayerList(player* player);

int main(int argc, char *argv[]) {

  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute;
  availablePlayers = 0;
  
  DataThread *dataThread;
  playerList = NULL;
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

  if (argc != 3)
    erreur("usage: %s port workerCount\n", argv[0]);

  port = (short)atoi(argv[1]);
	createPool((short)atoi(argv[2]));

  printf("%s: creating a socket\n", CMD);
  ecoute = socket (AF_INET, SOCK_STREAM, 0);
  if (ecoute < 0)
    erreur_IO("socket");
  
  adrEcoute.sin_family = AF_INET;
  adrEcoute.sin_addr.s_addr = INADDR_ANY;
  adrEcoute.sin_port = htons(port);
  printf("%s: binding to INADDR_ANY address on port %d\n", CMD, port);
  ret = bind (ecoute,  (struct sockaddr *)&adrEcoute, sizeof(adrEcoute));
  if (ret < 0)
    erreur_IO("bind");

  printf("%s: listening to socket\n", CMD);
  ret = listen (ecoute, 5);
  if (ret < 0)
    erreur_IO("listen");
    
  sem_init(&MainSem,0,(unsigned int)atoi(argv[2]));

  while (VRAI) {
  	struct sockaddr_in adrClient;
  	unsigned int lgAdrClient;
    printf("%s: accepting a connection\n", CMD);
    sem_wait(&MainSem);
    canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    printf("Client accepted. \n");
//    if (adrClient.sin_addr.s_addr == inet_addr("127.0.0.1")){
//
//    	dataThread = listeDataThread;
//    	while(dataThread != NULL){
//    		printf("Closing threads\n");
//    		dataThread->spec.libre = VRAI;
//    		dataThread->spec.canal = -1;
//    		pthread_join(dataThread->spec.id,NULL);
//    		dataThread = dataThread->next;
//
//    	}
//    	if (close(ecoute) == -1)
//    		erreur_IO("fermeture ecoute");
//
//			if (close(fdJournal) == -1)
//    		erreur_IO("fermeture journal");
//
//  		exit(EXIT_SUCCESS);
//
//    }
    if (canal < 0)
      erreur_IO("accept");

    printf("%s: adr %s, port %hu\n", CMD,
	        stringIP(ntohl(adrClient.sin_addr.s_addr)),
	        ntohs(adrClient.sin_port));

    dataThread = findFreeDataThread();
    
    dataThread->spec.canal = canal;
		dataThread->spec.libre = FAUX;
		sem_post(&dataThread->spec.waking_sem);
		
  }
  
}






void *communicationThread(void *arg) {
    DataThread *dataThread;
    char ligne[LIGNE_MAX];
    int lgLue;
    int challenged;
    bool play;
    dataThread = (DataThread *)arg;
    char port_str[40];
    sprintf(port_str,"%d",port);

    char *args[3] = {"client","localhost",port_str};
    while (dataThread->spec.canal != -1){
        sem_wait(&dataThread->spec.waking_sem);
        dataThread->spec.libre = FAUX;
        player* Player = bindPlayerToThread(dataThread);
        sprintf(ligne,"Veuillez saisir votre pseudo (20 caractères max) : ");
        ecrireLigne(dataThread->spec.canal,ligne);
        sprintf(ligne,"waiting input");
        ecrireLigne(dataThread->spec.canal,ligne);
        lireLigne(dataThread->spec.canal, Player->pseudo);
        while (!dataThread->spec.libre) {
            //DataThread *currentThread = listeDataThread;
            player *currentPlayer = playerList;

            showPlayerList(Player);
            pthread_t *IOThread;
            IOThread = (pthread_t*)malloc(sizeof(pthread_t));
            pthread_create(IOThread,NULL,inputThread,Player);
            play = false;
            while(!play){
                sem_getvalue(&Player->input,&lgLue);
                if(lgLue){
                    sem_wait(&Player->input);
                    lgLue = 0;
                    if (strcmp(&Player->kbInput[0],"q") == 0){
                        sprintf(ligne,"client exit");
                        ecrireLigne(dataThread->spec.canal, ligne);
                        unbindPlayer(Player);
                        dataThread->spec.libre = VRAI;
                        play = true;
                    }
                    else if (strcmp(&Player->kbInput[0],"r") == 0){
                        showPlayerList(Player);
                    }
                    else {
                        int challengedPlayerId = atoi(&Player->kbInput[0]);
                        if (challengedPlayerId < 0 || challengedPlayerId > availablePlayers) {
                            sprintf(ligne,"Entrée incorrecte, veuillez réessayer.");
                            ecrireLigne(dataThread->spec.canal, ligne);
                        }
                        else{
                            currentPlayer = playerList;
                            while(currentPlayer->id != challengedPlayerId){
                                currentPlayer = currentPlayer->next;
                            }
                            currentPlayer->challenger = Player;
                            sem_post(&currentPlayer->challenged);
                            sem_wait(&Player->challenged);
                            if(Player->challenger == NULL){
                                pthread_create(IOThread,NULL,inputThread,Player);
                            }
                            else{
                                printf("Début de la partie");
                                game *Game = (game*)malloc(sizeof(game));
                                Game->players[0] = Player;
                                Game->players[1] = currentPlayer;
                                pthread_create(&Game->id,NULL,start,Game);
                            }

                        }
                    }
                }
                sem_getvalue(&Player->challenged,&challenged);
                if(challenged){
                    sem_wait(&Player->challenged);
                    challenged = 0;
                    sprintf(ligne,"%s",Player->challenger->pseudo);
                    ecrireLigne(dataThread->spec.canal, ligne);
                    sprintf(ligne,"waiting input");
                    ecrireLigne(dataThread->spec.canal,ligne);
                    lireLigne(dataThread->spec.canal, ligne);
                    while(strcmp(ligne,"y") && strcmp(ligne,"n")){
                        sprintf(ligne,"Entrée incorrecte\n");
                        ecrireLigne(dataThread->spec.canal, ligne);
                        sprintf(ligne,"%s vous défie ! Accepter ? (y/n)\n",Player->challenger->pseudo);
                        ecrireLigne(dataThread->spec.canal, ligne);
                        sprintf(ligne,"waiting input");
                        ecrireLigne(dataThread->spec.canal,ligne);
                        lireLigne(dataThread->spec.canal, ligne);
                    }
                    sem_post(&Player->challenger->challenged);
                    if(!strcmp(ligne,"y")){
                        play=true;
                        Player->challenger->challenger = Player;
                    }
                    else{
                        Player->challenger->challenger = NULL;
                        showPlayerList(Player);
                    }
                }



            }
            if (dataThread->spec.libre){
                break;
            }
            sem_wait(&Player->gameDone);

        }
        sem_post(&MainSem);
    }
    printf("Jeu lancé");
    pthread_exit(NULL);
}

void *inputThread(void *arg){
    player *Player = (player*)arg;
    do {
        lireLigne(Player->handler->spec.canal, Player->kbInput);
        sem_post(&Player->input);
    }while(strcmp(Player->kbInput,"q") !=0);
    pthread_exit(NULL);
}




// le fichier est ferme et rouvert vide, le nouveau descripteur est retourne
int remiseAZeroJournal(int fdJournal) {
  int fdNouv;

  if (close(fdJournal) == -1)
    erreur_IO("raz journal - fermeture");

  fdNouv = open("journal.log", O_WRONLY|O_TRUNC|O_APPEND, 0644);
  if (fdNouv == -1)
    erreur_IO("raz journal - ouverture");

  return fdNouv;
}





void createPool(int slaveCount){
  initDataThread();
  DataThread *dataThread;
  for(int i = 0; i <slaveCount; i++){
      dataThread = ajouterDataThread();
      if (dataThread == NULL)
          erreur_IO("ajouterDataThread");
      int ret = pthread_create(&dataThread->spec.id, NULL, communicationThread,
                          dataThread);
      if (ret != 0)
      erreur_IO("pthread_create");
      dataThread->spec.libre = VRAI;
      dataThread->spec.canal = 0;
      sem_init(&dataThread->spec.waking_sem,0,0);
	}
}




DataThread *findFreeDataThread(){
	DataThread *currentThread = listeDataThread;
	while(currentThread != NULL){
		if(currentThread->spec.libre){
			return currentThread;
		}
		currentThread = currentThread->next;
	}
	return NULL;
}


player* bindPlayerToThread(DataThread *dataThread){
    player* currentPlayer = playerList;
    if (playerList == NULL){
        playerList = (player*) malloc(sizeof(player));
        playerList->handler = dataThread;
        playerList->id = 1;
        playerList->next = NULL;
        sem_init(&playerList->challenged,0,0);
        sem_init(&playerList->input,0,0);
        availablePlayers++;
        return playerList;
    }
    else{
        while(currentPlayer->next != NULL){
            currentPlayer = currentPlayer->next;
        }
        currentPlayer->next = (player*) malloc(sizeof(player));
        currentPlayer->next->handler = dataThread;
        currentPlayer->next->id = currentPlayer->id + 1;
        currentPlayer->next->next = NULL;
        sem_init(&currentPlayer->next->challenged,0,0);
        sem_init(&currentPlayer->next->input,0,0);
        availablePlayers++;
        return currentPlayer->next;
    }
}

void unbindPlayer(player* Player){
    player* currentPlayer = playerList;
    availablePlayers--;
    if(currentPlayer == Player){
        playerList = Player->next;
        free(Player);
    }
    else{
        while(currentPlayer->next != Player){
            currentPlayer = currentPlayer->next;
        }
        currentPlayer->next = Player->next;
        free(Player);
    }
}

void showPlayerList(player* Player){
    player* currentPlayer = playerList;
    char ligne[LIGNE_MAX];
    sprintf(ligne,"Numéro du joueur\t\tPseudo\t\tStatut");
    ecrireLigne(Player->handler->spec.canal,ligne);
    while (currentPlayer != NULL) {
        sprintf(ligne, "%d\t\t\t\t\t\t%s",currentPlayer->id, currentPlayer->pseudo);
        ecrireLigne(Player->handler->spec.canal, ligne);
        currentPlayer = currentPlayer->next;
    }
    sprintf(ligne,"Pour défier un joueur, tappez son numéro. Pour rafraichir la liste, tappez r. Pour quitter le serveur, tappez q\n");
    ecrireLigne(Player->handler->spec.canal, ligne);
    sprintf(ligne,"waiting input");
    ecrireLigne(Player->handler->spec.canal, ligne);

}