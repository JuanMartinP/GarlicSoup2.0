#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NSONS 4
#define AVGSIESTA 30000000
#define SSIESTA 5000000
int main(){
  int i;
  pid_t pid;

  for(i=0; i <= NSONS; i++){

    if((pid = fork()) < 0){
      printf("ERROR en fork.\n");
      exit(EXIT_FAILURE);
    }else if(pid==0){
      printf("Soy el proceso hijo %d\n", getpid());
      usleep(AVGSIESTA);
      printf("Soy el proceso hijo %d y ya me toca terminar.\n", getpid());
      exit(EXIT_SUCCESS);
    }else{
      usleep(SSIESTA);
      kill(pid, 15);
      wait(NULL);
    }
  }

  exit(EXIT_SUCCESS);
}
