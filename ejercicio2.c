/**
 * @file ejercicio2.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 27 Mar 2019
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NSONS 4
#define AVGSIESTA 30
#define SHORTSIESTA 5

int main() {
  int i;
  pid_t pid;

  for (i = 0; i <= NSONS; i++) {

    if ((pid = fork()) < 0) {
      printf("ERROR en fork.\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      printf("Soy el proceso hijo %d\n", getpid());
      sleep(AVGSIESTA);
      /* Mensaje que nunca se imprime porque el padre se despierta antes de
      que se despierte el hijo y lo mata mientras este esta en su siesta */
      printf("Soy el proceso hijo %d y ya me toca terminar.\n", getpid());
      exit(EXIT_SUCCESS);
    } else {
      /* Proceso padre */
      sleep(SHORTSIESTA);
      /* Cuando se despierta, mata al hijo */
      kill(pid, SIGTERM);
      wait(NULL);
    }
  }
  exit(EXIT_SUCCESS);
}
