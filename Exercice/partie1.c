#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <stdio.h>

#define NB_CAMIONS 5
#define NB_VOITURES 5
#define NB_VEHICULES (NB_CAMIONS + NB_VOITURES)

void attendre(double max)
{
    struct timespec delai;
    delai.tv_sec = tirage_aleatoire(max);
    delai.tv_nsec = 0;
    nanosleep(&delai, NULL);
}
int tirage_aleatoire(double max)
{
    int j = (int)(max * rand() / (RAND_MAX + 1.0));
    if (j < 1)
        return 1;
    return j;
}
sem_t semaphore_compteur;
pthread_mutex_t camions_sc;

void acceder_a_pont(int tonnes)
{
    if (tonnes == 15)
    {
        pthread_mutex_lock(&camions_sc);
        while (tonnes > 0)
        {
            sem_wait(&semaphore_compteur);
            tonnes -= 5;
        }
        pthread_mutex_unlock(&camions_sc);
    }
}

void liberer_pont(int tonnes)
{
    while (tonnes > 0)
    {
        sem_post(&semaphore_compteur);
        tonnes -= 5;
    }
}
void *voiture(void *args)
{
    int pid = *((int *)args);
    attendre(5.0);
    acceder_a_pont(5);
    printf("Voiture %d traverse le pont \n", pid);
    attendre(5.0);
    printf("Voiture %d quitte le pont\n", pid);
    liberer_pont(5);
    pthread_exit(NULL);
}

void *camion(void *args)
{
    int pid = *((int *)args);
    attendre(5.0);
    acceder_a_pont(15);
    printf("Camion %d traverse le pont \n", pid);
    attendre(5.0);
    printf("Camion %d quitte le pont\n", pid);
    liberer_pont(15);
    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t id;
    sem_init(&semaphore_compteur, 0, 3);
    for (i = 0; i < NB_VEHICULES; i++)
    {
        int *j = (int *)malloc(sizeof(int));
        *j = i;
        if (i < NB_CAMIONS)
            pthread_create(&id, NULL, camion, j);
        else
            pthread_create(&id, NULL, voiture, j);
    }
    pthread_exit(NULL);
}