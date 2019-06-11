#include "pse.h"

#define    CMD      "serveur"

void *communicationThread(void *arg);
int remiseAZeroJournal(int fdJournal);
void createPool(int slaveCount);
DataThread *findFreeDataThread();
void closeServer();
void *closingThread();

int fdJournal;
short port;
sem_t MainSem;


int main(int argc, char *argv[]) {

  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute;
  
  DataThread *dataThread;
  
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
    
  sem_init(&MainSem,0,argv[2]);

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
		sem_post(&dataThread->spec.sem);
		
  }
  
}






void *communicationThread(void *arg) {
  DataThread *dataThread;
  char ligne[LIGNE_MAX];
  int lgLue;

  dataThread = (DataThread *)arg;
  char port_str[40];
  sprintf(port_str,"%d",port);
  
  char *args[3] = {"client","localhost",port_str};
	while (dataThread->spec.canal != -1){
		sem_wait(&dataThread->spec.sem);
		dataThread->spec.libre = FAUX;
  	while (!dataThread->spec.libre) {
    	lgLue = lireLigne(dataThread->spec.canal, ligne);
    	if (lgLue < 0)
    	  erreur_IO("lireLigne");
    	else if (lgLue == 0)
    	  erreur("arret client\n");
	
    	printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);
	
    	if (strcmp(ligne, "fin") == 0) {
    	    printf("serveur: fin client\n");
    	    dataThread->spec.libre = VRAI;
    	}
    	else if (strcmp(ligne, "init") == 0) {
    	  printf("serveur: remise a zero journal\n");
    	  fdJournal = remiseAZeroJournal(fdJournal);
    	}
    	else if (strcmp(ligne, "fin_serveur") == 0) {
    	  printf("serveur: extinction serveur\n");
    	  dataThread->spec.libre = VRAI;
    	  if (close(dataThread->spec.canal) == -1)
    			erreur_IO("fermeture canal");
    	  execve("client",args,NULL);
    	  perror ("execve");
				exit (EXIT_FAILURE);
    	}
    	else if (ecrireLigne(fdJournal, ligne) != -1) {
    	    printf("serveur: ligne de %d octets ecrite dans journal\n", lgLue);
    	}
    	else
    	  erreur_IO("ecriture journal");
  	} 	// fin while
		sem_post(&MainSem);
  
	}
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
    sem_init(&dataThread->spec.sem,0,0);
    
    
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
