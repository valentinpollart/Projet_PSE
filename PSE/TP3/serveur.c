#include "pse.h"
#define BUF_SIZE 164

int main(int argc,char* argv[]){
  int inputFd,outputFd;
  char buffer[BUF_SIZE];
  int byteCounter;
  mkfifo("fifo",0422);
  inputFd = open("fifo", O_RDONLY, 0422);
  if (inputFd == -1) {
    erreur_IO("open input");
  }
  outputFd = open("journal.log", O_CREAT|O_WRONLY|O_APPEND, 0644);
  if (outputFd == -1) {
    erreur_IO("open output");
  }
  printf("Serveur initialisé\n");
  while(lireLigne(inputFd,buffer) != -1){
    if(strcmp(buffer,"fin") == 0){
      if (close(inputFd) == -1) {
        erreur_IO("close input");
      }
      if (close(outputFd) == -1) {
        erreur_IO("close output");
      }
      printf("Extinction du serveur");
      exit(EXIT_SUCCESS);
    }
    else if(strcmp(buffer,"init") == 0){
      if (close(outputFd) == -1) {
        erreur_IO("close output");
      }
      outputFd = open("journal.log", O_CREAT|O_WRONLY|O_TRUNC, 0644);
      printf("Journal réinitialisé\n");
    }
    else {
      byteCounter = ecrireLigne(outputFd,buffer);
      
      if (byteCounter == -1){
        erreur_IO("write output");
      }
      else {
        printf("%d octets écrits.\n",byteCounter);
        close(inputFd);
        inputFd = open("fifo", O_RDONLY, 0422);
      }
    }
  }
  erreur_IO("read input");
  if (close(inputFd) == -1) {
    erreur_IO("close input");
  }
  if (close(outputFd) == -1) {
    erreur_IO("close output");
  }
  exit(EXIT_FAILURE);
}
