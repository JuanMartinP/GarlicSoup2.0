/* ¿QUE SUCEDE CUANDO EL HIJO RECIBE LA SEÑAL DE ALARMA?
	- El hijo hace contador y no le interrumpen porque bloquea las señales
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N_ITER 5

int main(void) {
	pid_t pid;
	int counter;
	sigset_t setblocked, setbackup, setdesbloc;

	sigemptyset(&setblocked);

	pid = fork();
	if(pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	/* El hijo */
	if(pid == 0) {
		/* establece una alarma para ser recibida dentro de 40 segundos. */
		alarm(40);
		while(1) {
			/* bloquear las señales SIGUSR1, SIGUSR2 y SIGALRM */
			sigaddset(&setblocked, SIGUSR1);
			sigaddset(&setblocked, SIGUSR2);
			sigaddset(&setblocked, SIGALRM);
			if (sigprocmask(SIG_BLOCK, &setblocked, &setbackup) < 0) {
				perror("sigprocmask");
				exit(EXIT_FAILURE);
			}

			for(counter = 0; counter < N_ITER; counter++) {
				printf("%d\n", counter);
				sleep(1);
			}

			/* desbloquear señal SIGALRM y SIGUSR1 */
			sigaddset(&setdesbloc, SIGALRM);
			sigaddset(&setdesbloc, SIGUSR1);
			if (sigprocmask(SIG_UNBLOCK, &setdesbloc, &setbackup) < 0) {
				perror("sigprocmask");
				exit(EXIT_FAILURE);
			}

			sleep(3);
		}
	}
	/* Padre e hijo */
	while(wait(NULL) > 0);
}
