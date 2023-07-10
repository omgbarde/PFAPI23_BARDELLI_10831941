#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512

typedef struct{
    int autonomia;
}macchina;

typedef struct{
    macchina parcoAuto [MAX];
    int distanza;
    
}stazione;


void aggiungiStazione(){

}

void aggiungiMacchina(){

}

void demolisciStazione(){

}

void rottamaAuto(){

}

void pianificaPercorso(){

}

int main(int argc, char**argv){
    char* curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;
    char* cmd;
    char* token;

    do{
        lineSize = getline(&curr,&len,stdin);
        token = strtok(curr," ");
        if (strcmp(token,"aggiungi-stazione")==0){
            printf("aggiunta\n");
        }
        else if (strcmp(token,"demolisci-stazione")==0){
            printf("demolita\n");
        }
        else if(strcmp(token,"aggiungi-auto")==0){
            printf("aggiunta\n");
        }
        else if(strcmp(token,"rottama-auto")==0){
            printf("rottamata\n");
        }
        else if(strcmp(token,"pianifica-percorso")==0){
            printf("pianificato\n");
        }
    }while(lineSize!=-1);

    return 0;
}