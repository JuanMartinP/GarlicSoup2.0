/**
 * @file ejercicio6b.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 27 Mar 2019
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N_ITER 5

/* manejador_SIGTERM */
void manejador_SIGTERM(int sig) {
  printf("Soy %d y he recibido la señal SIGTERM\n", getpid());
  exit(EXIT_SUCCESS);
}

int main(void) {
  pid_t pid;
  int counter;
  struct sigaction act;

  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;

  /* Se arma la señal SIGTERM */
  act.sa_handler = manejador_SIGTERM;
  if (sigaction(SIGTERM, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  /* El hijo */
  if (pid == 0) {
    while (1) {
      for (counter = 0; counter < N_ITER; counter++) {
        printf("%d\n", counter);
        sleep(1);
      }
      sleep(3);
    }
  }
  /* El padre */
  else {
    /* Envia SIGTERM al hijo tras 40 segundos */
    sleep(40);
    kill(pid, SIGTERM);
    wait(NULL);
  }

  exit(EXIT_SUCCESS);
}
