#include "pse.h"

#define CMD   "client"

int main(int argc, char *argv[]) {
  int sock, ret;
  struct sockaddr_in *adrServ;
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgEcr;
  char ip[15];
  char port[5];
  char pseudo[20];

  printf("Bienvenue sur notre jeu d'échecs ! Veuillez saisir l'adresse d'un serveur de jeu sous le format IP:port : ");
  scanf("%s:%s",ip,port)
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    erreur_IO("socket");
  adrServ = resolv(ip,port);
  if (adrServ == NULL)
    erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);

  ret = connect(sock, (struct sockaddr *)adrServ, sizeof(struct sockaddr_in));
  if (ret < 0)
    erreur_IO("connect");
  printf("Connecté au serveur !\n");
  lireLigne(sock,ligne);
  printf("%s",ligne);
  scanf("%s",pseudo);
  ecrireLigne(sock, pseudo);

  while (!fin) {
    printf("Liste des joueurs connectés");
    if (fgets(ligne, LIGNE_MAX, stdin) == NULL)
      erreur("saisie fin de fichier\n");



    lgEcr = ecrireLigne(sock, ligne);
    if (lgEcr == -1)
      erreur_IO("ecrire ligne");
  
    printf("Done.\n");
    printf("%s: %d bytes sent\n", CMD, lgEcr);

    if (strcmp(ligne, "fin\n") == 0 || strcmp(ligne, "fin_serveur\n") == 0)
      fin = VRAI;
  }

  if (close(sock) == -1)
    erreur_IO("close socket");

  exit(EXIT_SUCCESS);
}
