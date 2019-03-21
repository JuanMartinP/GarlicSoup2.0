#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NSONS 4
#define AVGSIESTA   30
#define SHORTSIESTA 5
int main(){
	int i;
	pid_t pid;

	for(i=0; i <= NSONS; i++) {

		if((pid = fork()) < 0) {
			printf("ERROR en fork.\n");
			exit(EXIT_FAILURE);
		} else if(pid==0) {
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
