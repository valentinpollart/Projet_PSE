#include "pse.h"

#define DEFBOUCLES 1000
#define DEFTHREADS 2 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long globale = 0;

static void * maFonction (void *param) {
  int boucles = *((int *) param);
  int locale, j;
  for (j = 0; j < boucles; j++) {
    if (pthread_mutex_lock(&mutex) != 0)
      erreur_IO("lock mutex");
    locale = globale;
    locale++;
    globale= locale;
    if (pthread_mutex_unlock(&mutex) != 0)
      erreur_IO("unlock mutex");
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t *thr;
  int i;
  int boucles = DEFBOUCLES;
  int nbthreads = DEFTHREADS;
  long resultat;

  if (argc > 1) {
    boucles = atoi(argv[1]);
    if (argc > 2) {
      nbthreads = atoi(argv[2]);
      if (nbthreads < DEFTHREADS) nbthreads = DEFTHREADS;
    }
  }

  thr = (pthread_t *) malloc(nbthreads * sizeof(pthread_t));
  if (thr == NULL) {
    erreur("allocation %d pthread_t impossible\n", nbthreads);
  }

  resultat = boucles * nbthreads;

  printf("Donnees : %d boucles par thread (%d threads)\n",
	 boucles, nbthreads);
  printf("Resultat attendu : %ld\n", resultat);

  for (i=0; i<nbthreads; i++) {
    if ( pthread_create(&thr[i], NULL, maFonction, &boucles) != 0) {
      erreur_IO("pthread_create");
    }
  }
  
  for (i=0; i<nbthreads; i++) {
    if ( pthread_join(thr[i], NULL) != 0) {
      erreur_IO("pthread_join");
    }
  }

  /* les threads sont termines */
  printf("Resultat obtenu : %ld\n", globale);
  if (globale == resultat) {
    printf("Execution correcte !\n");
  }
  else {
    printf("Execution incorrecte !\n");
  }

  free(thr);
  pthread_exit(EXIT_SUCCESS);
}
