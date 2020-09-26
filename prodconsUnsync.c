#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int writeable; /*true/false*/
  int sharedData;
  int cond;
} buffer;

buffer theBuffer; /* Cria um struct do tipo buffer */
pthread_mutex_t mutex;

int store(int item, buffer *pb) {
  pthread_mutex_lock(&mutex);  //Trava 

  pb->sharedData += item;      
  pb->writeable = !pb->writeable;

  pthread_mutex_unlock(&mutex);   //Destrava

  return 0;
}

int retrieve(buffer *pb) {
  pthread_mutex_lock(&mutex);

  int data = pb->sharedData;
  pb->sharedData -= data;       
  pb->writeable = !pb->writeable;
  //pthread_mutex_unlock(&mutex);//
  return data;
}

void delay(int secs) { 
  time_t beg = time(NULL), end = beg + secs;
  do {
    ;
  } while (time(NULL) < end);
}

void *producer(void *n) {
  int j=1, soma = 0;
  while (j<=10) {
    store(j, &theBuffer);
    printf("Produziu: %d --- Status do buffer: %d\n", j, theBuffer.sharedData);
    //printf("Status do buffer: %d\n",theBuffer.sharedData);//
    soma += j++;
    delay(rand() % 6);  /* up to 5 sec */
  }
  theBuffer.cond = 0;
  printf("Soma Produzidos: %d\n", soma);
  return n;
}

void *consumer(void *n) {
  int j=0, tot=0;
  while (theBuffer.cond) {
    if (theBuffer.sharedData > 0){
      j = retrieve(&theBuffer);
      tot += j;
      pthread_mutex_unlock(&mutex);
      printf("Consumiu: %d --- Status do buffer: %d\n", j, theBuffer.sharedData);
      //printf("Status do buffer: %d\n",theBuffer.sharedData);//
      delay(rand() % 6);  /* up to 5 sec */
    }
  }
  printf("Consumiu um total = %d\n", tot);
  return(n);
}

int main() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_t prodThread, consThread;

  theBuffer.writeable = 1;
  theBuffer.cond = 1;
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL)); /* initialise the rng */

  pthread_create(&prodThread, &attr, producer, NULL);
  pthread_create(&consThread, &attr, consumer, NULL);

  pthread_exit(NULL);
}
