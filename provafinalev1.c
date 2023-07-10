#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512

typedef struct{
    int parcoAuto [MAX];
    int distanza;
}stazione;


void creaStazione(int dist){
    printf("%d\n", dist);
}

void demolisciStazione(int dist){
    printf("%d\n", dist);
}

void aggiungiAuto(int dist, int autonomia){
    printf("%d %d\n", dist ,autonomia);
}

void rottamaAuto(int dist, int autonomia){
    printf("%d %d\n", dist ,autonomia);
}

void pianificaPercorso(int start, int finish){

}

void execute(char * curr , int lineSize){
    char * cmd = strtok(curr," ");

    if (strcmp(cmd,"aggiungi-stazione")==0){
        char * dist = strtok(NULL," ");
        creaStazione(atoi(dist));
        char * car = strtok(NULL," ");
        while(car != NULL){
            aggiungiAuto(atoi(dist), atoi(car));
            car = strtok(NULL," ");
        }
        return;
    }
    else if (strcmp(cmd,"demolisci-stazione")==0){
        char * dist = strtok(NULL," ");
        demolisciStazione(atoi(dist));
        return;
    }
    else if(strcmp(cmd,"aggiungi-auto")==0){
        char * dist = strtok(NULL," ");
        char * arg = strtok(NULL," ");
        aggiungiAuto(atoi(dist), atoi(arg));
        return;
    }
    else if(strcmp(cmd,"rottama-auto")==0){
        char * dist = strtok(NULL," ");
        char * arg = strtok(NULL," ");
        rottamaAuto(atoi(dist), atoi(arg));
        return;
    }
    else if(strcmp(cmd,"pianifica-percorso")==0){
        char * start = strtok(NULL," ");
        char * finish = strtok(NULL," ");
        rottamaAuto(atoi(start), atoi(finish));
        return;
    }
    else{
        printf("errore nell' esecuzione del comando '");
        for(int i = 0; i < lineSize-1; i++){
            printf("%c",curr[i]);
        }
        printf("'\n");
    }
}

int main(int argc, char**argv){
    char * curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    while(1){
        lineSize = getline(&curr,&len,stdin);
        if(lineSize > 0){
            execute(curr,lineSize);
        }
        else break;
    }

    return 0;
}