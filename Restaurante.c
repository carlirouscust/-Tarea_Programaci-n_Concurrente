#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_CUBIERTOS 3
#define MAX_CUCHARAS 4
#define MAX_CUCHARITAS 3
#define MAX_CUCHILLOS 4

int cubiertos = MAX_CUBIERTOS;
int cucharas = MAX_CUCHARAS;
int cucharitas = MAX_CUCHARITAS;
int cuchillos = MAX_CUCHILLOS;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void imprimirDisponibilidad() {
    printf("Disponibilidad: Cubiertos: %d, Cucharas: %d, Cucharitas: %d, Cuchillos: %d\n",
           cubiertos, cucharas, cucharitas, cuchillos);
}

void *pedirComida(void *arg) {
    char *persona = (char *)arg;
    char plato[20];
    int entrada = 0, platoFuerte = 0, postre = 0;

   
    if (strcmp(persona, "Persona 1") == 0) {
        entrada = 1; platoFuerte = 1; postre = 1; 
    } else if (strcmp(persona, "Persona 2") == 0) {
        entrada = 1; platoFuerte = 1; postre = 1; 
    } else if (strcmp(persona, "Persona 3") == 0) {
        platoFuerte = 1; postre = 1; 
    } else if (strcmp(persona, "Persona 4") == 0) {
        platoFuerte = 1; postre = 1; 
    } else if (strcmp(persona, "Persona 5") == 0) {
        platoFuerte = 1; 
    }

    printf("%s esta pidiendo: Entrada: %d, Plato Fuerte: %d, Postre: %d\n",
           persona, entrada, platoFuerte, postre);

    pthread_mutex_lock(&mutex);

    
    if (entrada) {
        while (cubiertos == 0 || cuchillos == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        cubiertos--;
        cuchillos--;
    }
    if (platoFuerte) {
        while (cucharas == 0 || cubiertos == 0 || cuchillos == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        cucharas--;
        cubiertos--;
        cuchillos--;
    }
    if (postre) {
        while (cucharitas == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        cucharitas--;
    }

    printf("%s ha recibido su pedido\n", persona);
    imprimirDisponibilidad(); 

    pthread_mutex_unlock(&mutex);

    sleep(2); 

    pthread_mutex_lock(&mutex);
    if (entrada) {
        cubiertos++;
        cuchillos++;
    }
    if (platoFuerte) {
        cucharas++;
        cubiertos++;
        cuchillos++;
    }
    if (postre) {
        cucharitas++;
    }
    pthread_cond_broadcast(&cond);
    printf("%s ha terminado de comer\n", persona);
    imprimirDisponibilidad(); 
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t personas[5];
    char *nombres[5] = {"Persona 1", "Persona 2", "Persona 3", "Persona 4", "Persona 5"};

    for (int i = 0; i < 5; i++) {
        pthread_create(&personas[i], NULL, pedirComida, nombres[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(personas[i], NULL);
    }

    return 0;
}
