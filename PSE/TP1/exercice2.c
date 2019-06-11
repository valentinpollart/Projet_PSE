#include "pse.h"

int main ( int argc, char *argv[]) {
  /* requete DNS */
  int code;
  char *addr;
  addr = (char *)malloc(INET_ADDRSTRLEN);
  struct addrinfo *infos, hints;
  struct sockaddr_in *adresse;

  memset ( &hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ( argc != 3 ) {
    printf("Mauvais argument\n");
  }

  code = getaddrinfo ( argv[1], argv[2], &hints, &infos );
  if ( code != 0) {
    fprintf ( stderr, "Erreur: %s\n", gai_strerror(code) );
    exit ( EXIT_FAILURE );
  }
  adresse = (struct sockaddr_in *) infos->ai_addr;

  /* adresse->sin_addr.s_addr doit contenir 0xC131AEC2
                                         ie 193.49.174.194 */
  /* adresse->sin_port doit contenir 80 */

  /* Exercice 2 */
  printf("Adresse IP (Network Order) 	: 0x%X:%hd ~ %hd.%hd.%hd.%hd:%hd\n",adresse->sin_addr.s_addr,adresse->sin_port >> 8,(adresse->sin_addr.s_addr >> 24) & 255,(adresse->sin_addr.s_addr >> 16) & 255,(adresse->sin_addr.s_addr >> 8) & 255,adresse->sin_addr.s_addr & 255,adresse->sin_port >> 8);

  printf("Adresse IP (Host Order) 	: 0x%X:%hd ~ %hd.%hd.%hd.%hd:%hd\n",ntohl(adresse->sin_addr.s_addr),adresse->sin_port >> 8,(ntohl(adresse->sin_addr.s_addr) >> 24) & 255,(ntohl(adresse->sin_addr.s_addr) >> 16) & 255,(ntohl(adresse->sin_addr.s_addr) >> 8) & 255,ntohl(adresse->sin_addr.s_addr) & 255,adresse->sin_port >> 8);

  /* Exercice 3 */

  printf("Adresse IP (Network Order) 	: 0x%X:%hd ~ %s\n",adresse->sin_addr.s_addr,adresse->sin_port >> 8,inet_ntop(hints.ai_family,&adresse->sin_addr,addr,sizeof(addr)));


  
  freeaddrinfo ( infos );
  exit(EXIT_SUCCESS);
}
