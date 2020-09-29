#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
 
#define N 5 //Quantidade de filosofos 
#define ESQUERDA (i+N-1)%N //
#define DIREITA (i+1)%N
#define PENSANDO 0
#define FOME 1
#define COMENDO 2
 
int state[N], i;
 
sem_t mutex;   //semaforos
sem_t s[N];
 
void *filosofo(void *j){
   int i= *(int*) j;
   while(1){  
      sleep(2); 
      pegar_garfo(i); // pega os dois garfos ou bloqueia
      //sleep(2);
      colocar_garfo(i); //Devolve os garfos a mesa
   }
}
 
void pegar_garfo(int i){
   sem_wait(&mutex); //Começo da região critica
   state[i]=FOME;
   status_filosofo();
   test(i); 
   sem_post(&mutex); //fim da região critica
   sem_wait(&s[i]);  //Bloqueia se os garfos nçao foram pegos
}
 
void colocar_garfo(int i){
   sem_wait(&mutex); //Começo da região critica
   state[i]=PENSANDO;   //Depois de comer, vai para estado de pensando  
   status_filosofo();
   test(ESQUERDA);   //Verifica se o da esquerda pode comer
   test(DIREITA); //Verifica se o da direita pode comer
   sem_post(&mutex); //Fim da refião critica
}
 
void test(int i){
   if(state[i] == FOME && state[ESQUERDA] != COMENDO && state[DIREITA] != COMENDO){ //Verifica se o da direita e esquerda estão comendo
       state[i]=COMENDO;
       status_filosofo();
       sem_post(&s[i]);
   } 
}
 
void status_filosofo(){
   for(i=1; i<=N; i++){
       if(state[i-1] == PENSANDO){ printf("Filosofo %d pensando\n", i);}
       if(state[i-1] == FOME){ printf("Filosofo %d com fome\n", i);}
       if(state[i-1] == COMENDO){ printf("Filosofo %d comendo\n", i);}
   }
   printf("--------------------\n");
}
 
void main(){
 
   for(i=0;i<N; i++){ state[i]=0;}

   status_filosofo();
 
   int a;
   pthread_t thread[N];
   a = sem_init(&mutex,0,1);   //inicia os semaforos, os semaf... são um tipo especial ded inteiro
 
   for(i=0; i<N; i++){  //inicia os semaforos
      a = sem_init(&s[i],0,0);
   }
 
   for(i=0; i<N; i++){  //Criar as 5 threads
      a = pthread_create(&thread[i],NULL,filosofo,&i);
   }
 
	for(i=0; i<N; i++){  //Junta as threads
		a = pthread_join(thread[i], NULL); 
	}

}