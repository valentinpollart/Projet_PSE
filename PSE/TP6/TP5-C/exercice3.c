#include "pse.h"

#define NB_THREADS  10

void *fonctionThread(void *arg);

int somme[NB_THREADS];

void *fonctionThread(void *arg) {
  int *pNum = (int*)arg;
  int num = *pNum;
  printf("thread %d\n", num);
  somme[num] = (num * (num + 1)) / 2;
  pthread_exit(&somme[num]);
}

int main(void) {
  int numThread[NB_THREADS];
  pthread_t idThread[NB_THREADS];
  int i, ret;
  int *pSomme;

  for (i = 0; i < NB_THREADS; i++) {
    numThread[i] = i;
    ret = pthread_create(&idThread[i], NULL, fonctionThread, &numThread[i]);
    if (ret != 0)
      erreur_IO("pthread_create");
  }

  for (i = 0; i < NB_THREADS; i++) {
    ret = pthread_join(idThread[i], (void **)&pSomme);
    if (ret != 0)
      erreur_IO("pthread_join");
    printf("thread %d somme=%d\n", i, *pSomme);
  }

  exit(EXIT_SUCCESS);
}
