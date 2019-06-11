#include "pse.h"

int exitCode[10];

void * maFonction (void * val) {
  int * id = (int *) val;
  exitCode[*id] = 0;
  for (int i = 0; i <= *id; i++){
  	exitCode[*id] += i;
  }
  pthread_exit(exitCode+*id);
}

int main(void) {
  pthread_t idThread[10];
  int * codeRetour;
  int ID[10];
  for (int i = 0; i < 10; i++){
    ID[i]=i+1;
    int ret = pthread_create (idThread+i, NULL, maFonction, ID+i);
    if (ret != 0) {
      erreur_IO ("pthread_create");  
    }
    
  }
  for (int i = 0; i < 10; i++){
    if ( pthread_join ( idThread[i], (void **) &codeRetour) != 0) { perror("join"); exit(1); }
    printf ( "exitCode : %d\n", *codeRetour ) ;
  }
  exit(EXIT_SUCCESS);
}
