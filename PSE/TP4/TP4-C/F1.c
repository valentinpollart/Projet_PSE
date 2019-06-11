#include "pse.h"

int main(int argc, char *argv[]) {
  printf("F1: ");

  if (argc >= 2)
    printf("argument = %s", argv[1]);
  else
    printf("il manque un argument");

  printf(", pid = %d, pid pere = %d\n", getpid(), getppid());

  sleep(10);
  exit(11);
}
