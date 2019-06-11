#include "pse.h"

int main(void) {
  char nomFic[50];
  int p;
  char *arg[3];
  int nb1, nb2;
  int ret;
  char reponse;

  printf("nom de fichier : ");
  scanf("%s", nomFic);
  p = fork();
  if (p == 0) {
    arg[0] = "gedit";
    arg[1] = nomFic;
    arg[2] = NULL;
    execv("/usr/bin/gedit", arg);
    erreur_IO("execv");
  }
  else if (p > 0) {
    printf("entrer deux nombres entiers : ");
    scanf("%d%d", &nb1, &nb2);
    printf("produit : %d\n", nb1 * nb2);

    ret = waitpid(p, NULL, WNOHANG);
    if ( ret < 0)
      erreur_IO("waitpid");

    else if (ret == 0) {
      printf("edition non terminee, voulez-vous attendre (o ou n) ? ");
      scanf(" %c", &reponse);
      if (reponse == 'o') {
        if (wait(NULL) < 0)
          erreur_IO("wait");
      }
    }
  }
  else
    erreur_IO("fork");

  return 0;
}
