#include "pse.h"

int main(void) {
  int p1, p2;

  p1 = fork();
  if (p1 == 0) {
    printf("fils 1\n");
    sleep(10);
  }
  else if (p1 > 0) {
    // pere apres creation du fils 1
    p2 = fork();
    if (p2 == 0) {
      printf("fils 2\n");
      sleep(10);
    }
    else if (p2 > 0) {
      // pere apres creation du fils 2
      sleep(10);
    }
    else
      erreur_IO("creation fils 2");
  }
  else
    erreur_IO("creation fils 1");

  return 0;
}
