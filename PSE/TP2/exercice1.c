/* programme (c) M Kerrisk,
   adapté par P. Lalevée */

#include "pse.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int inputFd, outputFd;
  ssize_t nbRead;
  char buf[BUF_SIZE];
  int writtenCharCounter = 0;
  
  if (argc != 3) {
    erreur("usage: %s source dest\n", argv[0]);
  }
  inputFd = open(argv[1], O_RDONLY);
  if (inputFd == -1) {
    erreur_IO("open input");
  }
  outputFd = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0644);
  if (outputFd == -1) {
    erreur_IO("open output");
  }
  while ((nbRead = read(inputFd, buf, BUF_SIZE)) > 0) {
    writtenCharCounter+=nbRead;
    if (write(outputFd, buf, nbRead) != nbRead) {
      erreur_IO("write");
    }
  }
  if (nbRead == -1) {
    erreur_IO("read");
  }
  if (close(inputFd) == -1) {
    erreur_IO("close input");
  }
  if (close(outputFd) == -1) {
    erreur_IO("close output");
  }
  printf("Nombre d'octets lus : %d\n",writtenCharCounter);
  exit(EXIT_SUCCESS);
}
