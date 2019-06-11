#include "pse.h"

int main(void) {
  int p1, p2;
  char *arg1[] = {"F1", "bonjour", NULL};
  char *arg2[] = {"F2", NULL};
  int i, p, status;

  p1 = fork();
  if (p1 == 0) {
    execv("F1", arg1);
    erreur_IO("exec F1"); // on revient de execv si echec
  }
  else if (p1 > 0) {
    // pere apres creation du fils 1
    p2 = fork();
    if (p2 == 0) {
      execv("F2", arg2);
      erreur_IO("exec F2");
    }
    else if (p2 > 0) {
      // pere apres creation du fils 2
      printf("pere: pid = %d, pid fils1 = %d, pid fils2 = %d\n", getpid(),
                                                                    p1, p2);
      for (i = 0; i < 2; i++) {
        p = wait(&status);
        if (p < 0)
          erreur_IO("wait");
        else if (p == p1)
          printf("fin fils 1");
        else
          printf("fin fils 2");
        printf(", code exit = %d\n", WEXITSTATUS(status));
      }
    }
    else
      erreur_IO("creation fils 2");
  }
  else
    erreur_IO("creation fils 1");

  return 0;
}
