/**
 * @file ejercicio9.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 27 Mar 2019
 */

#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N_PROC 5
#define SEM "/recurso"
#define RANDMAX 100000
#define WIN 1234

/* manejador_SIGUSR1 */
void manejador_SIGUSR1(int sig) {}

int main() {
  pid_t pid;
  FILE *salida = NULL;
  sem_t *sem = NULL;
  int i, ganador = N_PROC, temp = 0, flag = 0, r;
  int contador[N_PROC] = {0};
  struct sigaction act;

  /* Opening the semaphore */
  if ((sem = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
    perror("sem_open");
    return -1;
  }

  /* Para que se vacie el archivo */
  salida = fopen("salida.txt", "w");
  fclose(salida);

  /* Se arma la señal SIGUSR1 */
  act.sa_handler = manejador_SIGUSR1;
  if (sigaction(SIGUSR1, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("PREPARADOS?\n");
  sleep(1);
  printf("LISTOS?\n\n");
  sleep(1);

  for (i = 0; i < N_PROC; i++) {
    pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    /*PROCESO HIJO. Sensible a SIGTERM y SIGUSR1*/
    else if (pid == 0) {
      srand(getpid());
      r = rand();
      printf("Proceso %d: Listo para la carrera\n", i);
      /* Espera al SIGUSR1 (salida del padre) para una carrera + o - justa */
      pause();
      /* Dentro de un bucle, cada participante escribe en un fichero su id y
       * duerme un tiempo aleatorio de máximo una décima. */
      while (1) {
        sem_wait(sem);
        /* Abro fichero para escribir ("a": anadir sin sobreescribir) */
        salida = fopen("salida.txt", "a");
        fprintf(salida, "%d ", i);
        fclose(salida);
        sem_post(sem);
        /* DUERME MAXIMO UNA DECIMA: RANDOMIZAR */
        usleep((r % RANDMAX) + 1);
      }
      sem_close(sem);
      exit(EXIT_SUCCESS);
    }
  }

  /* Padre da la salida (SIGUSR1) */
  sleep(2);
  printf("\nYA!\n\n");
  kill(0, SIGUSR1);

  /*El padre lee el fichero contando cada numero y guardandolo donde
   * corresponde. Al llegar a 20 anota el ganador*/
  while (ganador >= N_PROC) {
    sleep(1);
    sem_wait(sem);

    /* Padre abre fichero para lectura */
    salida = fopen("salida.txt", "r");

    while (fscanf(salida, "%d", &temp) && flag != WIN && temp != EOF) {
      contador[temp]++;
      for (i = 0; i < N_PROC; i++) {
        if (contador[i] >= 20) {
          ganador = i;
          flag = WIN;
          signal(SIGTERM, SIG_IGN);
          kill(0, SIGTERM); /* we done, kill the competitors */
        }
      }
    }
    sem_post(sem);
    fclose(salida);
  }
  sem_close(sem);
  sem_unlink(SEM);
  for (i = 0; i < N_PROC; i++) {
    printf("Proceso %d: %d escrituras\n", i, contador[i]);
  }
  printf("\nHa GANADO el %d!!\n", ganador);
  /* El padre espera a que todos sus hijos finalicen */
  for (i = 0; i < N_PROC; i++)
    wait(NULL);

  exit(EXIT_SUCCESS);
}
