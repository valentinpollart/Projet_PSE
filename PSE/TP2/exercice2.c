#include "pse.h"


int main(int argc, char* argv[]){
  int inputFd;
  int fileNumber = 0;
  char
  if(argc != 4){
    erreur("Missing or invalid argument. Usage : ./exercice2 nameOfFileToStrip maxSizeOfStripFile beginningOfStripFileName");
  }
  char* buffer[(int)argv[2]]
  
  inputFd = open(argv[1],O_RDONLY,0600);
  if (inputFd == -1) {
    erreur_IO("open input");
  }
  do{
    
  
  } while(read(inputFd,buffer,(int)argv[2])=argv[2])
}
