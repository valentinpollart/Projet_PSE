#include "pse.h"

int main (void) {
  pid_t pid;
  int sonCounter;
  int sonCreated = 0;
  int status = 0;
  printf("Combien voulez de fils directs ?\n");
  scanf("%d",&sonCounter);
  while(pid != 0 && sonCreated != sonCounter){
  	sonCreated ++;
  	pid = fork();
  }
  if (pid == 0){
  	 sleep(2);
  	 printf("Mon PID : %d.PID de mon père :%d\n",getpid(),getppid());
  	 return 0;
  }
  while( wait(&status) > 0);
  printf("Mon PID : %d.\n",getpid());
  return 0;
}

