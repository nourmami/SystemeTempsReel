#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>

#define NB_CAMIONS  5
#define NB_VOITURES 5
#define NB_VEHICULES (NB_CAMIONS + NB_VOITURES)

#define WAITING    1
#define NOTHING    2
#define CROSSING   3

//choisir aléatoirement un entier entre 1 et max-1 secondes pour attendre
int tirage_aleatoire(double max){
    int j = (int) (max*rand()/(RAND_MAX+1.0));
    if (j < 1)
        return 1;
    return j;
}

void attendre(double max){
    struct timespec delai;
    delai.tv_sec=tirage_aleatoire(max);
    delai.tv_nsec=0;
    nanosleep(&delai,NULL);

}


pthread_mutex_t sc;
sem_t sempriv[NB_VEHICULES];

int state[NB_VEHICULES]; //tableau qui nous montre les etats des vehicules (Waiting-Nothing-Crossing)

int nbCamionsBloquees=0; //compteur des camions bloquées

int sum_tonnes=0; // somme des tonnes sur le pont 


void acceder_au_pont(int tonnes,int pid)
{
    pthread_mutex_lock(&sc);
    // si le pont support le poids du vehicule, ce dernier traverse
    if(sum_tonnes+tonnes <=15 ){
        state[pid]=CROSSING;
        sum_tonnes=sum_tonnes+tonnes;
        sem_post(&sempriv[pid]);
    }
    // sinon il attend et si c'est un camion, on augmente le nombre des camions bloquées
    else {
        if(tonnes==15){
            nbCamionsBloquees++;
        }
        state[pid]=WAITING;
    }
    pthread_mutex_unlock(&sc);
    sem_wait(&sempriv[pid]);
}

void quitter_le_pont(int tonnes, int pid)
{
    int i=0;
    pthread_mutex_lock(&sc);
    state[pid]=NOTHING;
    sum_tonnes=sum_tonnes-tonnes;
    // on verifie les camions bloqués d'abord ( car on a donné la priorité aux camions)
    while((i<NB_CAMIONS)&& (sum_tonnes==0)){
        if(state[i]==WAITING){
            sem_post(&sempriv[i]);
            nbCamionsBloquees--;
            sum_tonnes=15;
        }
        i++;
    }

    for(i=NB_CAMIONS;i<NB_VEHICULES;i++){
        if ( sum_tonnes<15 && nbCamionsBloquees == 0 && state[i]== WAITING){
            sum_tonnes=sum_tonnes+5;
            sem_post(&sempriv[i]);
        }
    }
    pthread_mutex_unlock(&sc);
}

void *voiture(void *args)
{
    int pid = *((int *)args);
    attendre(5.0);
    acceder_au_pont(5,pid);
    printf("Voiture %d traverse le pont \n", pid);
    attendre(5.0);
    printf("Voiture %d quitte le pont\n", pid);
    quitter_le_pont(5,pid);
    pthread_exit(NULL);
}

void *camion(void *args)
{
    int pid = *((int *)args);
    attendre(5.0);
    acceder_au_pont(15,pid);
    printf("Camion %d traverse le pont \n", pid);
    attendre(5.0);
    printf("Camion %d quitte le pont\n", pid);
    quitter_le_pont(15,pid);
    pthread_exit(NULL);
}


int main(int argc, char* arg[])
{
    int i;
    pthread_t id;

    for(i=0;i<NB_VEHICULES;i++){
        state[i]==NOTHING;
        sem_init(&sempriv[i],0,0);
    }

    pthread_mutex_init(&sc,0);

    for(i=0;i<NB_VEHICULES;i++){
        int* j = (int*) malloc(sizeof(int));
        *j=i;
        if(i<NB_CAMIONS){
            pthread_create(&id,NULL,camion,j);
        } else {
            pthread_create(&id,NULL,voiture,j);
        }
    }
    
    pthread_exit(NULL);
}