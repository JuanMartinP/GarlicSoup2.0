#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_PROC 5

int main(){

    pid_t pid;
    pid_t ppid;
    FILE *salida;
    int idproc, i, ganador, temp;
    int contador[N_PROC];

    temp = 0;
    meta = 0;
    idproc = N_PROC++; /*Para que no este en el padre sin inicializar*/
    ppid = getpid();

    for(i=0; i <= N_PROC; i++)   /*Inicializo todo contador a 0 (?)*/
        contador[i] = 0;

    /*TODO manejadores*/

    for (i  = 0; i < N_PROC; i++) {

        pid = fork();

        if(pid < 0) {

            perror("fork");
            exit(EXIT_FAILURE);

        }/*PROCESO HIJO*/
        else if(pid == 0) {

            idproc = i;       /*NO ESTOY SEGURO DE QUE NO SOBREESCRIBA LA VARIABLE A CADA ITERACION DEL BUCLE*/

            /*Envio una señal al padre para avisar de que ya estoy listo*/
            kill(ppid, SIGUSR1);
            /*Hago pause() para que todos los procesos empiecen a la vez y sea una "carrera justa"*/
            pause();

            while(1){
                /*Abro fichero para escribir, modo "a" para anadir y no sobreescribir*/
                salida = fopen("salida.txt", "a");
                fprintf(salida, "%d", idproc);
                /*Se deberia salir de aqui con el sigterm que lanza el padre*/
            }


        }/*PROCESO PADRE*/
        else{
            /*Padre espera a que le llegue la señal a sus hijos*/
            pause();
        }

    }

    /*Segun el ejercicio 4(linea 68) aquí se ejecuta el padre tras recibir la señal de todos sus hijos*/
    salida = fopen("salida.txt", "r");

/*En este bucle el padre lee el fichero contando cada numero y guardandolo donde corresponde al llegar a 20 anota el ganador, hago los comentarios tan largos para que no se te pase ninguna explicacion <3*/
    while(ganador > 4){
        do{
            fscanf(salida, "%d", temp);
            for(i = 0; i < N_PROC; i++){
                if(temp == i)
                    contador[i]++;

                if(contador[i] >= 20)
                    ganador = i;
            }
        }while(temp != null)
        sleep(1);
    }

    for(i = 0; i < N_PROC; i++)
        wait();

    exit(EXIT_SUCCESS);
}
