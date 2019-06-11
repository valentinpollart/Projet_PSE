#include "pse.h"
#define BUF_SIZE 164

int main(int argc,char* argv[]){
  signal(SIGPIPE,SIG_IGN);
  char buffer[BUF_SIZE];
  int outputFd, byteCounter;
  outputFd = open("fifo", O_WRONLY, 0422);
  if (outputFd == -1) {
    erreur_IO("open output");
  }
  printf("Envoyez la ligne à écrire dans le journal : ");
  fgets(buffer,BUF_SIZE,stdin);
  while (strcmp(buffer,"fin") != 0){
    byteCounter = ecrireLigne(outputFd,buffer);
    if (byteCounter == -1){
      erreur_IO("write output");
    }
    printf("Envoyez la ligne à écrire dans le journal : ");
    fgets(buffer,BUF_SIZE,stdin);
  }
  if (close(outputFd) == -1) {
    erreur_IO("close output");
  }
  exit(EXIT_SUCCESS);
}
