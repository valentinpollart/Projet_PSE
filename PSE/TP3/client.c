#include "pse.h"
#define BUF_SIZE 164

int main(int argc,char* argv[]){

  char buffer[BUF_SIZE];
  int error;
  sockaddr_in adresse;
  
  if (argc != 3) {
    perror ("Incorrect arguments. Command use : ./client serverIP serverPort" );
    exit ( EXIT_FAILURE );
  }
  
  int sock = socket (AF_INET, SOCK_STREAM, 0 ) ;
  if ( sock == -1 ) {
    perror ("Socket creation failure" );
    exit ( EXIT_FAILURE );
  }
	
  if ( bind ( sock, (struct sockaddr *) &adresse, sizeof ( adresse ) ) != 0 ) {
    perror ( "Bind failure" );
    exit ( EXIT_FAILURE );
  }
	
  if ( connect ( sock, (struct sockaddr *) &adrServ, sizeof ( adrServ ) ) == -1) {
    perror ( "accept" );
    exit ( EXIT_FAILURE );
  }
  
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
