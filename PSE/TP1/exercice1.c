#include "pse.h"

extern char ** environ;

int main ( int argc, char *argv[] ) {
  printf("argc = %d\n", argc);
  if ( argc == 1) {
    erreur("%s: mauvais argument\n", argv[0]);
  }
  for (int i = 1;i < argc; i++)
  {
  printf("argv %d : %s\n",i,argv[i]);
  }
  exit(EXIT_SUCCESS);
}
