#include "pse.h"
#include "tampon.h"

pthread_mutex_t mutex_iLibre = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_iPlein = PTHREAD_MUTEX_INITIALIZER;

void lock_mutex_tampon(pthread_mutex_t *pmutex);
void unlock_mutex_tampon(pthread_mutex_t *pmutex);

void tampon_init(Tampon *t) {
  int i;
  t->iLibre = 0;
  t->iPlein = 0;
  for (i=0; i<P; i++) t->buffer[i] = 0;
}

int tampon_deposer(Tampon *t, int valeur) {
  int indice;
  lock_mutex_tampon(&mutex_iLibre);
  indice = t->iLibre;
  t->buffer[t->iLibre] = valeur;
  t->iLibre = (t->iLibre + 1) % P;
  unlock_mutex_tampon(&mutex_iLibre);
  return indice;
}

int tampon_prendre(Tampon *t, int *valeur) {
  int indice;
  lock_mutex_tampon(&mutex_iPlein);
  indice = t->iPlein;
  *valeur = t->buffer[t->iPlein];
  t->iPlein = (t->iPlein + 1) % P;
  unlock_mutex_tampon(&mutex_iPlein);
  return indice;
}

void lock_mutex_tampon(pthread_mutex_t *pmutex) {
  if (pthread_mutex_lock(pmutex) != 0)
    erreur_IO("lock mutex tampon");
}

void unlock_mutex_tampon(pthread_mutex_t *pmutex) {
  if (pthread_mutex_unlock(pmutex) != 0)
    erreur_IO("unlock mutex tampon");
}
