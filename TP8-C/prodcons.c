#include "pse.h"
#include "tampon.h"

#define NB_PROD     3
#define NB_CONSO    10 

void *threadProd(void *arg);
void *threadConso(void *arg);

Tampon tampon;

// semaphore pour synchroniser le producteur : il produit s'il y a une place
// libre dans le tampon
sem_t semProd;

// semaphore pour synchroniser le consommateur : il consomme s'il y a une
// valeur presente dans le tampon
sem_t semConso;

Tampon tampon;

int main(int argc, char *argv[]) {
  int i;
  int numProd[NB_PROD], numConso[NB_CONSO];
  pthread_t idProd[NB_PROD], idConso[NB_CONSO];

  tampon_init(&tampon);

  if (sem_init(&semProd, 0, P) != 0)    // P taille du tampon
    erreur_IO("init semaphore producteur");

  if (sem_init(&semConso, 0, 0) != 0)
    erreur_IO("init semaphore consommateur");

  for (i = 0; i < NB_PROD; i++) {
    numProd[i] = i;
    if (pthread_create(&idProd[i], NULL, threadProd, &numProd[i]) != 0)
      erreur_IO("creation prod");
  }

  for (i = 0; i < NB_CONSO; i++) {
    numConso[i] = i;
    if (pthread_create(&idConso[i], NULL, threadConso, &numConso[i]) != 0)
      erreur_IO("creation conso");
  }

  for (i = 0; i < NB_PROD; i++) {
    if (pthread_join(idProd[i], NULL) != 0)
      erreur_IO("join prod");
  }

  for (i = 0; i < NB_CONSO; i++) {
    if (pthread_join(idConso[i], NULL) != 0)
      erreur_IO("join conso");
  }

  exit(EXIT_SUCCESS);
}

void *threadProd(void *arg) {
  int numProd = * ((int *)arg);
  int val = 0;

  while (VRAI) {
    usleep(100000);

    if (sem_wait(&semProd) != 0)
      erreur_IO("wait semaphore producteur");

    tampon_deposer(&tampon, val);

    if (sem_post(&semConso) != 0)
      erreur_IO("post semaphore consommateur");

    printf("producteur %d valeur produite: %d\n", numProd, val);
    val++;
  }
  pthread_exit(NULL);
}

void *threadConso(void *arg) {
  int numConso = * ((int *)arg);
  int val;

  while (VRAI) {
    usleep(1000000);

    if (sem_wait(&semConso) != 0)
      erreur_IO("wait semaphore consommateur");

    tampon_prendre(&tampon, &val);

    if (sem_post(&semProd) != 0)
      erreur_IO("post semaphore producteur");

    printf("consommateur %d valeur consommee : %d\n", numConso, val);
  }
  pthread_exit(NULL);
}
