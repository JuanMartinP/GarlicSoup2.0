#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N_PROC 4

/* manejador_SIGUSR1: presenta un número aleatorio por pantalla. */
void manejador_SIGUSR1(int sig){
  printf("%d\n",rand());
  fflush(stdout);
}

/* manejador_SIGUSR2: presenta un número aleatorio por pantalla. */
void manejador_SIGUSR2(int sig){
  printf("Captured message\n");
  fflush(stdout);
}

int main{
  pid_t pid;

  if((pid = fork()) < 0){
    printf("ERROR en fork.\n");
    exit(EXIT_FAILURE);
  }else if(pid==0){       /*Proceso hijo gestor*/

    /* trozo del << Se arma la señal SIGUSR1. >> */

    for(i = 0;i <= N_PROC; i++){
      if((pid = fork()) < 0){
        printf("ERROR en fork.\n");
        exit(EXIT_FAILURE);
      }else if(pid==0){ /*Proceso re hijo*/
        printf("Im a re-son and im ready to launch, piumpium\n");
        /* Avisa a gestor con SIGUSR2 */
        kill(pid, 12)
      }
      /*manejador*/

      /* Tras recibir todos los SIGUSR2 avisa al padre con SIGUSR2 */
      kill(pid, 12);
    }
    exit(EXIT_SUCCESS);
  }else{

  }


}
