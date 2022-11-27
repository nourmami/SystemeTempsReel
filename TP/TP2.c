#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

//creation des semaphores pour chaque segment du pont
pthread_mutex_t AB;
pthread_mutex_t BC;
pthread_mutex_t CD;
pthread_mutex_t BF;
pthread_mutex_t CE;


void attendre(int seconds)
{
        //wait for 6 seconds 
        int milli_seconds = 1000 * seconds;
        clock_t start_time = clock();
        while (clock() < start_time + milli_seconds);
}

      
      

//le metro de A a B 
void* Metro_de_A_vers_D (void* arg) {


    pthread_mutex_lock(&AB);
    printf("Le métro %d de la ligne 1 est en train de traverser le segment AB\n",(int)pthread_self());
    attendre(6);
    printf("Le segment AB est libre\n");
    pthread_mutex_unlock(&AB);

    pthread_mutex_lock(&BC);
    printf("Le métro %d de la ligne 1 est en train de traverser le segment BC\n",(int)pthread_self());
    attendre(6);
    printf("Le segment BC est libre\n");
    pthread_mutex_unlock(&BC);

    pthread_mutex_lock(&CD);
    printf("Le métro %d de la ligne 1 est en train de traverser le segment CD\n",(int)pthread_self());
    attendre(6);
    printf("Le segment CD est libre\n");
    pthread_mutex_unlock(&CD);
    printf("Le métro %d de la ligne 1 est arrivé à D!\n",(int)pthread_self());

    pthread_exit(NULL);
}

//le metro de E a B
void* Metro_de_E_vers_B (void* arg) {

    

    pthread_mutex_lock(&CE);
    printf("Le métro %d de la ligne 3 est en train de traverser le segment EC\n",(int)pthread_self());
    attendre(6);
    printf("Le segment CE est libre\n");
    pthread_mutex_unlock(&CE);

    pthread_mutex_lock(&BC);
    printf("Le métro %d de la ligne 3 est en train de traverser le segment CB\n",(int)pthread_self());
    attendre(6);
    printf("Le segment BC est libre\n");
    pthread_mutex_unlock(&BC);
    printf("Le métro %d de la ligne 3 est arrivé à B !\n",(int)pthread_self());

    pthread_exit(NULL);
}

//le metro de F a A
void* Metro_de_F_vers_A (void* arg) {

    
    pthread_mutex_lock(&BF);
    printf("Le métro %d de la ligne 2 est en train de traverser le segment FB\n",(int)pthread_self());
    attendre(6);
    printf("Le segment BF est libre\n");
    pthread_mutex_unlock(&BF);

    pthread_mutex_lock(&AB);
    printf("Le métro %d de la ligne 2 est en train de traverser le segment BA\n",(int)pthread_self());
    attendre(6);
    printf("Le segment AB est libre\n");
    pthread_mutex_unlock(&AB);
    printf("Le métro %d de la ligne 2 est arrivé à A !\n",(int)pthread_self());

    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t id;

    pthread_mutex_init(&AB,0);
	pthread_mutex_init(&BC,0);
	pthread_mutex_init(&CD,0);
    pthread_mutex_init(&BF,0);
	pthread_mutex_init(&CE,0);

    for(i=0; i<5; i++)
    {
        int* j=(int*)malloc(sizeof(int));
        *j=i;
        pthread_create(&id,NULL,Metro_de_A_vers_D,j);
        pthread_create(&id,NULL,Metro_de_E_vers_B,j);
        pthread_create(&id,NULL,Metro_de_F_vers_A,j);
    }

    pthread_exit(NULL);
}