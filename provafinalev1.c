#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512

struct stazione{
    int parcoAuto [MAX];
    int distanza;
    //struct stazione * fwd [MAX];
    //struct stazione * bwd [MAX];
    struct stazione * next;
    struct stazione * prev;
};

typedef struct stazione elemLista;
typedef elemLista * nodo;

nodo head = NULL;

nodo cercaStazione(int dist){
    nodo cursore;
    if(head != NULL){
        cursore = head;
        while(cursore != NULL && cursore->distanza <= dist){
        printf("cerco %d...\n",dist);
            if(cursore->distanza == dist){
                printf("trovata!\n");
                return cursore;
            }
            cursore = cursore->next;
        }
    }
    printf("non trovata\n"); 
    return NULL;
}

void creaStazione(int dist){
    printf("creo %d...\n", dist);
    nodo new, current, prev;
    prev = NULL;
    current = head;

    if(dist >= 0){
        while(current != NULL && dist > current->distanza){
            prev = current;
            current = current->next;
        }
        new = malloc(sizeof(elemLista));
        //new->parcoAuto[] = {(0)};
        new->distanza = dist;
        new->next = current;
        if(prev != NULL){
            prev->next = new;
            new->prev = prev;
        }
        else{
            new->prev = NULL;
            head = new;
            new->next=current;
        }
        //update(new); per aggiornare grafo di raggiungibilità (dalle altre stazioni verso la nuova)
        printf("aggiunta\n");
        return;
    }
    else{
        printf("non aggiunta\n");
        return;
    }
}

void demolisciStazione(int dist){
    //cerco la stazione e salvo il ptr
    if(dist >= 0){
        struct stazione * ptr = cercaStazione(dist);
        if(ptr != NULL){
            if(ptr->prev == NULL){
                head = ptr->next;
                ptr->next->prev = NULL;
            }
            else{
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
            }
            free(ptr);
        }
        //update(NULL); per aggiornare grafo di raggiungibilità (dalla stazione modificata verso le altre)
    }
}

void aggiungiAuto(int dist, int autonomia){
    printf("aggiungo %d a %d...\n",autonomia, dist);
    struct stazione * ptr = cercaStazione(dist);
    int i = 0;
    if(ptr != NULL){
        while (ptr->parcoAuto[i]!=0 && i < MAX)
        {
            i++;
        }
        if(i < MAX){
            ptr->parcoAuto[i] = autonomia;
            printf("aggiunta\n");
            //update(NULL); per aggiornare grafo di raggiungibilità (dalla stazione modificata verso le altre)
            return;
        }
    }
    printf("non aggiunta\n");
    return;
}

void rottamaAuto(int dist, int autonomia){
    printf("%d %d\n", dist ,autonomia);
}

void pianificaPercorso(int start, int finish){

}

void update(struct stazione * new){
    if(new != NULL){

    }
}


void execute(char * curr , int lineSize){
    char * cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0){
        char * dist = strtok(NULL, " ");
        creaStazione(atoi(dist));
        char * car = strtok(NULL, " ");
        while(car != NULL){
            aggiungiAuto(atoi(dist), atoi(car));
            car = strtok(NULL, " ");
        }
        return;
    }
    else if(strcmp(cmd, "demolisci-stazione") == 0){
        char * dist = strtok(NULL, " ");
        demolisciStazione(atoi(dist));
        return;
    }
    else if(strcmp(cmd, "aggiungi-auto") == 0){
        char * dist = strtok(NULL, " ");
        char * arg = strtok(NULL, " ");
        aggiungiAuto(atoi(dist), atoi(arg));
        return;
    }
    else if(strcmp(cmd, "rottama-auto") == 0){
        char * dist = strtok(NULL, " ");
        char * arg = strtok(NULL, " ");
        rottamaAuto(atoi(dist), atoi(arg));
        return;
    }
    else if(strcmp(cmd, "pianifica-percorso") == 0){
        char * start = strtok(NULL, " ");
        char * finish = strtok(NULL, " ");
        rottamaAuto(atoi(start), atoi(finish));
        return;
    }
    else{
        printf("errore nell' esecuzione del comando '");
        for(int i = 0; i < lineSize-1; i++){
            printf("%c", curr [i]);
        }
        printf("'\n");
        return;
    }
}

int main(int argc, char**argv){
    char * curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    while(1){
        lineSize = getline(&curr, &len, stdin);
        if(lineSize > 0){
            execute(curr, lineSize);
        }
        else break;
    }
    
    /*
    stampa l'autostrada
    while(head!= NULL){
        printf("%d-->",head->distanza);
        head = head->next;
    }*/

    return 0;
}