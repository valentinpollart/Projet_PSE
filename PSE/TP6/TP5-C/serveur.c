#include "pse.h"

#define    CMD      "serveur"

void *fonctionThread(void *arg);
int remiseAZeroJournal(int fdJournal);

int fdJournal;

int main(int argc, char *argv[]) {
  short port;
  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  DataThread *dataThread;
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

  if (argc != 2)
    erreur("usage: %s port\n", argv[0]);

  port = (short)atoi(argv[1]);

  initDataThread();

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
  
  while (VRAI) {
    printf("%s: accepting a connection\n", CMD);
    canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    if (canal < 0)
      erreur_IO("accept");

    printf("%s: adr %s, port %hu\n", CMD,
	        stringIP(ntohl(adrClient.sin_addr.s_addr)),
	        ntohs(adrClient.sin_port));

    dataThread = ajouterDataThread();
    if (dataThread == NULL)
      erreur_IO("ajouterDataThread");
    dataThread->spec.canal = canal;

    ret = pthread_create(&dataThread->spec.id, NULL, fonctionThread,
                          dataThread);
    if (ret != 0)
      erreur_IO("pthread_create");

    joinDataThread();
  }

  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  if (close(fdJournal) == -1)
    erreur_IO("fermeture journal");

  exit(EXIT_SUCCESS);
}

void *fonctionThread(void *arg) {
  DataThread *dataThread;
  int canal;
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgLue;

  dataThread = (DataThread *)arg;
  canal = dataThread->spec.canal;

  while (!fin) {
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lireLigne");
    else if (lgLue == 0)
      erreur("arret client\n");

    printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);

    if (strcmp(ligne, "fin") == 0) {
        printf("serveur: fin client\n");
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

  dataThread->spec.libre = VRAI;

  if (close(canal) == -1)
    erreur_IO("fermeture canal");

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
