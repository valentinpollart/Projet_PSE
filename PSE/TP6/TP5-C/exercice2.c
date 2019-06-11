#include "pse.h"

int lgChaine;

void *maFonction(void *val) {
  char *chaine = (char *) val;
  printf ("chaine recue: %s\n", chaine);
  lgChaine = strlen(chaine);
  pthread_exit(&lgChaine);
}

int main(void) {
  pthread_t idThread;
  int *pLgChaine;

  int ret = pthread_create(&idThread, NULL, maFonction, "Bonjour a tous");
  if (ret != 0)
    erreur_IO ("pthread_create");  

  ret = pthread_join(idThread, (void **)&pLgChaine);
  if (ret != 0)
    erreur_IO ("pthread_join");  
  printf("lg chaine : %d\n", *pLgChaine);

  exit(EXIT_SUCCESS);
}
