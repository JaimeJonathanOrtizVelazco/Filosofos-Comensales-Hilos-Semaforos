#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t filosofo[5];
sem_t mutex_jarra;
void *FILOSOFO(int i);
int main(){
    pthread_t th[5];
    sem_init(&mutex_jarra,0,1);
    for (int i = 0; i < 5; i++) {
        sem_init(&filosofo[i],0,1);
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&th[i],NULL,FILOSOFO,i);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(th[i],NULL);
    }
    sem_destroy(&mutex_jarra);
    return 0;
}
void *FILOSOFO(int i){
    int valuer;
    int valuel;
    while (1){
        if(i==0){
            sem_getvalue(&filosofo[4],&valuel);
            sem_getvalue(&filosofo[1],&valuer);
        }else if(i==4){
            sem_getvalue(&filosofo[3],&valuel);
            sem_getvalue(&filosofo[0],&valuer);
        }else{
            sem_getvalue(&filosofo[i-1],&valuel);
            sem_getvalue(&filosofo[i+1],&valuer);
        }
        if(valuel==1 && valuer==1){
            sem_wait(&filosofo[i]);
            printf("El filosofo %lu esta comiendo\n",pthread_self());
            sleep(2);
            printf("El filosofo %lu dejo de comer\n",pthread_self());
            sem_post(&filosofo[i]);
            sleep(2);
        }else{
            int agua=1;
            printf("El filosofo %lu esta esperando comer\n",pthread_self());
            sleep(2);
            sem_getvalue(&mutex_jarra,&agua);
            if(agua==1){
                sem_wait(&mutex_jarra);
                printf("El filosofo %lu esta tomando agua\n",pthread_self());
                sleep(4);
                sem_post(&mutex_jarra);
            }
        }
    }
}