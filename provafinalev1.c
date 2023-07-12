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
    //printf("cerco %d...\n",dist);
    nodo cursore;
    if(head != NULL){
        cursore = head;
        while(cursore != NULL && cursore->distanza <= dist){
            if(cursore->distanza == dist){
                //printf("trovata!\n");
                return cursore;
            }
            cursore = cursore->next;
        }
    }
    //printf("non trovata\n"); 
    return NULL;
}

int insert(int dist, int autonomia){
    struct stazione * ptr = cercaStazione(dist);
    int i = 0;
    if(ptr != NULL && autonomia > 0){
        while (ptr->parcoAuto[i]!=0 && i < MAX)
        {
            i++;
        }
        if(i < MAX){
            ptr->parcoAuto[i] = autonomia;
            return 0;
        }
    }
    return 1;
}

void aggiungiAuto(int dist, int autonomia){
    //printf("aggiungo %d a %d...\n",autonomia, dist);
    
    if(insert(dist, autonomia)==0){
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");

    return;
}

void creaStazione(int dist, int * cars, int amount){
    //printf("creo %d...\n", dist);
    nodo new, current, prev;
    prev = NULL;
    current = head;
    //aggiunta alla lista
    if(dist >= 0){
        while(current != NULL && dist > current->distanza){
            prev = current;
            current = current->next;
        }
        new = malloc(sizeof(elemLista));
        //new->parcoAuto = 0;
        new->distanza = dist;
        new->next = current;
        if(current != NULL) current->prev = new;
        new->prev = prev;
        //inserimento in mezzo o alla fine
        if(prev != NULL){
            prev->next = new;
        }
        //inserimento in testa
        else{
            head = new;
            new->next = prev;
        }
        //inserimento auto
        int i = 0, check = 0;
        int car = cars[i];
        while(car > 0 && check == 0){
            check = insert(dist, car);
            i++;
            car = cars[i];
        }
        printf("aggiunta\n");
        return;
    }

    else{
        printf("non aggiunta\n");
        return;
    }
}

void rottamaAuto(int dist, int autonomia){
    struct stazione * ptr = cercaStazione(dist);
    int i = 0;
    if(ptr != NULL && autonomia > 0){
        while (ptr->parcoAuto[i] != autonomia && i < MAX && ptr->parcoAuto[i] != 0){
            i++;
        }
        if(i < MAX ){
            ptr->parcoAuto[i] = 0;
            printf("rottamata\n");
            return;
        }
    }

    printf("non rottamata\n");
    return;
}

void demolisciStazione(int dist){
    //printf("demolisco %d\n", dist);
    //cerco la stazione e salvo il ptr
    if(dist >= 0){
        struct stazione * ptr = cercaStazione(dist);
        if(ptr != NULL){
            //printf("aggiorno lista\n");
            if(ptr->prev == NULL){
                head = ptr->next;
                ptr->next->prev = NULL;
            }
            else if(ptr->next == NULL){
                printf("%s\n",(char*)ptr->prev);
                ptr->prev->next = NULL;
                ptr->prev = NULL;
            }
            else{
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
            }
            free(ptr);
            printf("demolita\n");
            return;
        }
    }
    printf("non demolita\n");
    return;
}

void update(){
    
}

void pianificaPercorso(int start, int finish){
    //update(); per aggiornare grafo di raggiungibilità
}

void execute(char * curr , int lineSize){
    char * cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0){
        char * dist = strtok(NULL, " ");
        int array [MAX] = {0};
        int i = 0;
        char * car = strtok(NULL, " ");
        while(car != NULL){
            array[i] = atoi(car);
            car = strtok(NULL, " ");
            i++;
        }
        creaStazione(atoi(dist), array, i);
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

int main(int argc, char ** argv){
    char * curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    //scansione dell'input
    while(1){
        lineSize = getline(&curr, &len, stdin);
        if(lineSize > 0){
            execute(curr, lineSize);
        }
        else break;
    }

    //prova di stampa in entrambi i versi
    nodo last=head;
    while(head != NULL){
        printf("%d :", head->distanza);
        int i = 0;
        //while (i< MAX){
        while (i<MAX){
            if(head->parcoAuto[i] != 0) printf("%d,", head->parcoAuto[i]);
            i++;
        }
        printf("-->");
        last = head;
        head = head->next;
    }
    printf("\n");
    while(last != NULL){
        printf("%d :", last->distanza);
        int i = 0;
        //while (i< MAX){
        while (i<MAX){
            if(last->parcoAuto[i] != 0) printf("%d,", last->parcoAuto[i]);
            i++;
        }
        printf("-->");
        last = last->prev;
    }
    printf("\n");
    //
    return 0;
}