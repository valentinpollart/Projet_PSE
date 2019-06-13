#ifndef DATASPEC_INCLUDE_H
#define DATASPEC_INCLUDE_H

/* module datathread : donnees specifiques */

/* donnees specifiques */
typedef struct DataSpec_t {
  pthread_t id;               /* identifiant du thread */
  int libre;                  /* indicateur de terminaison */
/* ajouter donnees specifiques après cette ligne */
  int tid;                    /* identifiant logique */
  int canal;                  /* canal de communication */
  sem_t waking_sem;                  /* semaphore de reveil */
  sem_t input_sem;            /* sémaphore input console */
	char playerName[20];
} DataSpec;

#endif
