#include "pse.h"

#define    CMD      "serveur"

int remiseAZeroJournal(int fdJournal);
void * clientCommunication(void * args);


int main(int argc, char *argv[]) {
  short port;
  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  int fdJournal;
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgLue;
  pthread_t *currentThreadId;
  DataThread *currentThread;
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

  if (argc != 2)
    erreur("usage: %s port\n", argv[0]);

  port = (short)atoi(argv[1]);

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
  
  printf("%s: accepting a connection\n", CMD);
  
  initDataThread();
  currentThread = listeDataThread;
  while (1) {
  
  	canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
  	ajouterDataThread();
  	current->DataSpec->canal = canal;
  	pthread_create(currentThreadId, NULL, clientCommunication, void)
  	current->DataSpec->id = *currentThreadId;
  	
  	
  	
  	if (canal < 0)
    	erreur_IO("accept");

  	printf("%s: adr %s, port %hu\n", CMD,
	        	stringIP(ntohl(adrClient.sin_addr.s_addr)),
	        	ntohs(adrClient.sin_port));

  
  
  
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lireLigne");
    else if (lgLue == 0)
      erreur("arret client\n");

    printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);

    if (strcmp(ligne, "fin") == 0) {
        printf("serveur: arret\n");
        fin = VRAI;
    }
    else if (strcmp(ligne, "init") == 0) {
      printf("serveur: remise a zero journal\n");
      fdJournal = remiseAZeroJournal(fdJournal);
    }
    else if (ecrireLigne(fdJournal, ligne) != -1) {
        printf("serveur: ligne de %d octets ecrite dans journal\n", lgLue);
    }
    else
      erreur_IO("ecriture journal");
  } // fin while

  if (close(canal) == -1)
    erreur_IO("fermeture fifo");

  if (close(fdJournal) == -1)
    erreur_IO("fermeture journal");

  exit(EXIT_SUCCESS);
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

void * clientCommunication(void * args){





}
