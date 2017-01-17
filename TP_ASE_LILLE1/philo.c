#include<stdio.h>
#include <semaphore.h>
#include <stdlib.h>

#define NbPhilo 4

pthread_mutex_t fourchettes[NbPhilo];
sem_t places;

typedef enum{Pense, Mange, Attend} T_etat;
T_etat Etat_Philo[NbPhilo];


int gauche(int i){
  return((i+1)% NbPhilo);
}

int droit(int i){
  return i;
}

void manger(){}

void penser(){}


void obtenir_fourchettes(int i) {
sem_wait(&places);
pthread_mutex_lock(&fourchettes[gauche(i)]);
pthread_mutex_lock(&fourchettes[droite(i)]);
}

void relacher_fourchettes(int i) {
pthread_mutex_unlock(&fourchettes[gauche(i)]);
pthread_mutex_unlock(&fourchettes[droite(i)]);
sem_post(&places);
}

int main() {
  int i;

  for (i=0; i < NbPhilo; i++) {
   pthread_mutex_init(&fourchettes[i], NULL);
  }
  sem_init(&places, 0, NbPhilo-1);

  while (1) {
    penser();
    obtenir_fourchettes();
    manger();
    relacher_fourchettes();
  }
}
