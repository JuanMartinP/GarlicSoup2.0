/**
 * @file ejercicio8.c
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

#define N_READ 10
#define SEMR "/sem1"
#define SEMW "/sem2"
#define SEML "/sem3"
#define SECS 1

sem_t *sem_lectura = NULL, *sem_escritura = NULL, *sem_lectores = NULL;

/* manejador_SIGINT (para el padre) */
void manejador_SIGINT(int sig) {
  kill(0, SIGTERM);

  while (wait(NULL) > 0);

  /* Liberar todos los recursos */
  sem_close(sem_lectura);
  sem_close(sem_escritura);
  sem_close(sem_lectores);
  sem_unlink(SEMR);
  sem_unlink(SEMW);
  sem_unlink(SEML);

  exit(EXIT_SUCCESS);
}

/* manejador_SIGTERM (para el hijo) */
void manejador_SIGTERM(int sig) {
  sem_close(sem_lectura);
  sem_close(sem_escritura);
  sem_close(sem_lectores);
  exit(EXIT_SUCCESS);
}

/* manejador vacio para ignorar senial */
void manejador_vacio(int sig) {}

int main(void) {
  pid_t pid;
  int i, lectores;
  struct sigaction act;

  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;

  if ((sem_lectura = sem_open(SEMR, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  if ((sem_escritura = sem_open(SEMW, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  if ((sem_lectores = sem_open(SEML, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  /* Se arma la señal SIGTERM */
  act.sa_handler = manejador_SIGTERM;
  sigemptyset(&(act.sa_mask));
  act.sa_flags = SA_RESETHAND;
  if (sigaction(SIGTERM, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  /* Se arma la señal SIGINT */
  act.sa_handler = manejador_SIGINT;
  sigemptyset(&(act.sa_mask));
  act.sa_flags = SA_RESETHAND;
  if (sigaction(SIGINT, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < N_READ; i++) {
    pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    /* El hijo (lector) */
    /* Ignora SIGINT, sensible a SIGTERM */
    if (pid == 0) {

      /* Se "ignora" SIGINT con un manejador vacio */
      act.sa_handler = manejador_vacio;
      sigemptyset(&(act.sa_mask));
      act.sa_flags = SA_RESETHAND;
      if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
      }

      /* Bucle de lectura */
      while (1) {
        sem_wait(sem_lectura);

        /* lectores++ */
        sem_post(sem_lectores);

        sem_getvalue(sem_lectores, &lectores);
        if (lectores == 1)
          sem_wait(sem_escritura);
        sem_post(sem_lectura);

        /* Lectura */
        printf("R-INI %d\n", getpid());
        sleep(1);
        printf("R-FIN %d\n", getpid());

        sem_wait(sem_lectura);

        /* lectores-- */
        sem_wait(sem_lectores);

        sem_getvalue(sem_lectores, &lectores);
        if (lectores == 0)
          sem_post(sem_escritura);
        sem_post(sem_lectura);

        sleep(SECS);
      }
    }
    /* El padre (escritor) */
    /* Ignora SIGTERM, sensible a SIGINT */
    else {
      /* Cuando ya estan los hijos creados hace la escritura */
      if (i == N_READ - 1) {

        /* Se "ignora" SIGTERM con un manejador vacio */
        act.sa_handler = manejador_vacio;
        sigemptyset(&(act.sa_mask));
        act.sa_flags = SA_RESETHAND;
        if (sigaction(SIGTERM, &act, NULL) < 0) {
          perror("sigaction");
          exit(EXIT_FAILURE);
        }

        /* Bucle de escritura */
        while (1) {
          sem_wait(sem_escritura);

          /* Escritura */
          printf("W-INI %d\n", getpid());
          sleep(1);
          printf("W-FIN %d\n", getpid());

          sem_post(sem_escritura);

          sleep(SECS);
        }
      }
    }
  }
}
