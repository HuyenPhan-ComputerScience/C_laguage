#include<stdio.h>
#include <pthread.h>
#include<stdlib.h>
#define MAX_SIZE 256
int fibs[MAX_SIZE];
void *run(void *param);
int main(int argc,char *argv[]){
    int i;
    pthread_t tid;
    pthread_attr_t attr;
    if(argc !=2){
        fprintf(stderr,"usage:a.out<integer value>\n");
        return -1;
    }
    if(atoi(argv[1])<0){
        fprintf(stderr,"Argument %d must be >=0\n",atoi(argv[1]));
    }
    //  get the default attributes
    pthread_attr_init(&attr);
    // creat thread
    pthread_create(&tid,0,run,argv[1]);
    // wait for thread to exit
    pthread_join(tid,NULL);
    //out put the Fibanacci numbers
    for(i=0;i<atoi(argv[1]);i++){
        printf("%d\n",fibs[i]);
    }
    return 0;
   



}
 void *run(void*param){
        int i;
        int upper=atoi(param);
        if(upper==0){
            pthread_exit(0);
        }
        else if(upper ==1){
            fibs[0]=0;
        }
        else if(upper==2){
            fibs[0]=0;
            fibs[1]=1;
        }
        else{
            fibs[0]=0;
            fibs[1]=1;
            for(i=2;i<upper;i++){
                fibs[i]=fibs[i-1]+fibs[i-2];
            }
        }
        pthread_exit (0);

    }
