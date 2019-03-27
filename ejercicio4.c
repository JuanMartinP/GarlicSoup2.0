/**
 * @file ejercicio4.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 27 Mar 2019
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N_PROC 4

/* manejador_SIGUSR1 */
void manejador_SIGUSR1(int sig){
}

/* manejador_SIGUSR2 */
void manejador_SIGUSR2(int sig){
}

int main (){
	pid_t pid, pidpadre, pidgestor;
	int i;
	struct sigaction act;

	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;

	/* Se arma la señal SIGUSR1 */
	act.sa_handler = manejador_SIGUSR1;
	if (sigaction(SIGUSR1, &act, NULL) < 0) {
			perror("sigaction");
			exit(EXIT_FAILURE);
	}

	/* Se arma la señal SIGUSR2 */
	act.sa_handler = manejador_SIGUSR2;
	if (sigaction(SIGUSR2, &act, NULL) < 0) {
			perror("sigaction");
			exit(EXIT_FAILURE);
	}

	pidpadre = getpid();
	if((pid = fork()) < 0){
		printf("ERROR en fork.\n");
		exit(EXIT_FAILURE);
	}
	/*Proceso hijo gestor*/
	else if (pid == 0) {
		pidgestor = getpid();
		for(i = 0; i < N_PROC; i++){
			if ((pid = fork()) < 0) {
				printf("ERROR en fork.\n");
				exit(EXIT_FAILURE);
			}
			/*Proceso nieto competidor*/
			else if (pid == 0) {
				printf("Soy el nieto %ld. Listo para la carrera! Voy a avisar al gestor con SIGUSR2\n", (long)getpid());
				/* Avisa a gestor con SIGUSR2 */
				kill(pidgestor, SIGUSR2);
				/* Cada participante debe recibir SIGUSR1 (salida) */
				pause();
				printf("Soy el nieto %ld y he recibido SIGUSR1, que es la senial de salida\n", (long)getpid());
				exit(EXIT_SUCCESS);
			}
			/* Hijo gestor espera a que llegue senial de cada rehijo */
			else {
				pause();
			}
		}

		/* Tras recibir todos los SIGUSR2 avisa al padre con SIGUSR2 */
		kill(pidpadre, SIGUSR2);
		printf("\nSoy el gestor y voy a avisar al padre con SIGUSR2 de que todos estan listos\n");

		/* Espera a la senial SIGUSR1 del padre (la salida) */
		pause();
		printf("Soy el hijo gestor y he recibido SIGUSR1, que es la senial de salida\n");

		/* Hijo espera a todos los rehijos y termina su ejecucion*/
		for(i = 0; i < N_PROC; i++){
			wait(NULL);
		}
		exit(EXIT_SUCCESS);
	}
	/* Proceso padre */
	else {
		/* Espera a SIGUSR2 del hijo gestor (todos los nietos estan listos) */
		pause();
		sleep(1);
		printf("\nSoy el padre y he recibido SIGUSR2 del gestor. Doy la salida con SIGUSR1\n\n");
		/*kill(0, ...) Envia a todos los procesos*/
		kill(0, SIGUSR1);
		wait(NULL);
	}

	exit(EXIT_SUCCESS);
}
