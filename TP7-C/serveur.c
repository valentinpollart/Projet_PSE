#include "pse.h"

#define CMD         "serveur"
#define NB_WORKERS  2

void creerCohorteWorkers(void);
int chercherWorkerLibre(void);
void *threadSessionClient(void *arg);
void sessionClient(int canal);
int remiseAZeroJournal(int fdJournal);

int fdJournal;
DataSpec dataThread[NB_WORKERS];
sem_t semWorkersLibres;

int main(int argc, char *argv[]) {
  short port;
  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  int numWorker;
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

  if (argc != 2)
    erreur("usage: %s port\n", argv[0]);

  port = (short)atoi(argv[1]);

  creerCohorteWorkers();
  ret = sem_init(&semWorkersLibres, 0, NB_WORKERS);
  if (ret == -1)
    erreur_IO("init sem workers libres");

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

    ret = sem_wait(&semWorkersLibres);
    if (ret == -1)
      erreur_IO("wait sem workers libres"); 

    numWorker = chercherWorkerLibre();
    printf("%s: worker %d libre\n", CMD, numWorker);

    dataThread[numWorker].canal = canal;
    ret = sem_post(&dataThread[numWorker].sem);
    if (ret == -1)
      erreur_IO("post sem thread"); 
  }

  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  if (close(fdJournal) == -1)
    erreur_IO("fermeture journal");

  exit(EXIT_SUCCESS);
}

void creerCohorteWorkers(void) {
  int i, ret;

  for (i = 0; i < NB_WORKERS; i++) {
    ret = sem_init(&dataThread[i].sem, 0, 0);
    if (ret == -1)
      erreur_IO("init sem thread");

    dataThread[i].canal = -1;

    ret = pthread_create(&dataThread[i].id, NULL, threadSessionClient,
                          &dataThread[i]);
    if (ret != 0)
      erreur_IO("pthread_create");
  }
}

// retourne le numero du worker libre ou -1 si pas de worker libre
int chercherWorkerLibre(void) {
  int i;
  
  i = 0;
  while (dataThread[i].canal != -1  && i < NB_WORKERS)
    i++;
  if (i < NB_WORKERS)
    return i;
  else
    return -1;
}

void *threadSessionClient(void *arg) {
  DataSpec *dataThread;
  int ret;

  dataThread = (DataSpec *)arg;

  while (VRAI) {
    ret = sem_wait(&dataThread->sem);
    if (ret == -1)
      erreur_IO("wait sem thread");

    sessionClient(dataThread->canal);

    dataThread->canal = -1;

    ret = sem_post(&semWorkersLibres);
    if (ret == -1)
      erreur_IO("post sem workers libres");
  }

  pthread_exit(NULL);
}

void sessionClient(int canal) {
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgLue;

  while (!fin) {
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lire ligne");

    printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);

    if (lgLue == 0) {
      printf("%s: interruption client\n", CMD);
      fin = VRAI;
    }
    else if (strcmp(ligne, "fin") == 0) {
      printf("%s: fin client\n", CMD);
      fin = VRAI;
    }
    else if (strcmp(ligne, "init") == 0) {
      printf("%s: remise a zero journal\n", CMD);
      fdJournal = remiseAZeroJournal(fdJournal);
    }
    else if (ecrireLigne(fdJournal, ligne) != -1) {
      printf("%s: ligne de %d octets ecrite dans journal\n", CMD, lgLue);
    }
    else
      erreur_IO("ecriture journal");
  } // fin while

  if (close(canal) == -1)
    erreur_IO("fermeture canal");
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
