#include "pse.h"

int main(void) {
  printf("F2: pid = %d, pid pere = %d\n", getpid(), getppid());
  sleep(10);
  exit(12);
}
