#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512

enum nodeColor {
  RED,
  BLACK
};

struct stazione{
    int distanza;
    int parcoAuto[MAX];
    int numeroAuto;
    int color;
    struct stazione *parent;
    struct stazione *left;
    struct stazione *right;
};

typedef struct stazione * autostrada;

autostrada root = NULL;
autostrada lastEdited = NULL; //da aggiungere ai nuovi metodi

autostrada minimum(autostrada nodo)
{
    while (nodo->left != NULL)
    {
        nodo = nodo->left;
    }
    return nodo;
    
}

autostrada successor(autostrada nodo)
{
    if(nodo->right != NULL)
        return minimum(nodo->right);
    autostrada y = nodo->parent;
    while(y != NULL && nodo == y->right)
    {
        nodo = y;
        y = y->parent;
    }
    return y;
}

autostrada cercaStazione(int dist) //da sistemare (inorder walk?)
{
    // printf("cerco %d...\n",dist);
    if(lastEdited != NULL && lastEdited->distanza == dist)
    {
         return lastEdited;
    }
    else{
        struct stazione * cursore;
        if (root != NULL)
        {
            cursore = root;
            while (cursore != NULL && cursore->distanza <= dist)
            {
                if (cursore->distanza == dist)
                {
                    // printf("trovata!\n");
                    return cursore;
                }
                cursore = cursore->next;
            }
        }
        // printf("non trovata\n");
        return NULL;
    }
}

int insert(int dist, int autonomia) //da aggiungere ai nuovi metodi
{
    struct stazione *ptr;
    if(lastEdited != NULL && lastEdited->distanza == dist)
    {
        ptr = lastEdited;
    }
    else{
        ptr = cercaStazione(dist);
    }

    int i = 0;
    if (ptr != NULL && autonomia > 0 && ptr->numeroAuto < MAX)
    {
        for (i = 0; i < ptr->numeroAuto; i++)
        {
            if (ptr->parcoAuto[i] < autonomia)
            {
                break;
            }
        }

        for (int j = ptr->numeroAuto; j > i; j--)
        {
            ptr->parcoAuto[j] = ptr->parcoAuto[j - 1];
        }
        ptr->parcoAuto[i] = autonomia;
        ptr->numeroAuto++;
        return 0;
    }
    return 1;
}

void aggiungiAuto(int dist, int autonomia) //da aggiungere ai nuovi metodi
{
    // printf("aggiungo %d a %d...\n",autonomia, dist);

    if (insert(dist, autonomia) == 0)
    {
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

void leftRotate(autostrada nodo)
{
    autostrada y = nodo->right;
    nodo->right = y->left;
    if(y->left != NULL)
        y->left->parent = nodo;
    y->parent = nodo->parent;
    if(nodo->parent == NULL)
        root = y;
    else if(nodo = nodo->parent->left)
        nodo->parent->left = y;
    else nodo->parent->right = y;
    y->left = nodo;
    nodo->parent = y;
}

void rightRotate(autostrada nodo)
{
    autostrada y = nodo->left;
    nodo->left = y->right;
    if(y->right != NULL)
        y->right->parent = nodo;
    y->parent = nodo->parent;
    if(nodo->parent == NULL)
        root = y;
    else if(nodo = nodo->parent->left)
        nodo->parent->left = y;
    else nodo->parent->right = y;
    y->left = nodo;
    nodo->parent = y;
}

void insertFixup(autostrada nodo)
{
    autostrada x,y;
    if(root == nodo)
        root->color = BLACK;
    else
    {
        x = nodo->parent;
        if(x->color == RED){
            if(x == x->parent->left){
                y = x->parent->right;
                if(y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED,
                    insertFixup(x->parent);
                }
                else if(nodo == x->right)
                {
                    nodo = x;
                    leftRotate(nodo);
                    x = nodo->parent;
                }
            }
            x->color = BLACK;
            x->parent->color = RED;
            rightRotate(x->parent);
        }
        else{
            if(x == x->parent->right){
                y = x->parent->left;
                if(y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED,
                    insertFixup(x->parent);
                }
                else if(nodo == x->left)
                {
                    nodo = x;
                    rightRotate(nodo);
                    x = nodo->parent;
                }
            }
            x->color = BLACK;
            x->parent->color = RED;
            leftRotate(x->parent);
        }
    }
}

void creaStazione(int dist, int *cars, int amount)
{
    if(dist >= 0){
        autostrada y = NULL;
        autostrada x = root;
        autostrada new = malloc(sizeof(autostrada));
        new->distanza = dist;

        while(x != NULL)
        {
            y = x;
            if(dist < x->distanza)
                x = x->left;
            else
                x = x->right;
        }
        new->parent = y;
        if(y == NULL)
            root = new; //inserimento ad albero vuoto
        else if(dist < y->distanza)
            y->left = new;
        else
            y->right = new;
        new->left = NULL;
        new->right = NULL;
        new->color = RED;
        insertFixup(new);
    }
}

/*void creaStazione(int dist, int *cars, int amount)
{
    // printf("creo %d...\n", dist);
    struct stazione * new, current, prev;
    prev = NULL;
    current = root;

    // aggiunta alla lista
    if (dist >= 0)
    {
        while (current != NULL && dist > current->distanza)
        {
            prev = current;
            current = current->next;
        }
        new = malloc(sizeof(elemLista));
        new->distanza = dist;
        new->next = current;
        new->prev = prev;
        if (current != NULL)
            current->prev = new;

        // inserimento in mezzo o alla fine
        if (prev != NULL)
        {
            prev->next = new;
        }
        // inserimento in testa
        else
        {
            root = new;
        }

        lastEdited = new;
        // inserimento auto
        int i = 0, check = 0;
        int car = cars[i];
        while (car > 0 && check == 0)
        {
            check = insert(dist, car);
            i++;
            car = cars[i];
        }
        printf("aggiunta\n");
        return;
    }

    else
    {
        printf("non aggiunta\n");
        return;
    }
}
*/

void rottamaAuto(int dist, int autonomia)
{
    struct stazione *ptr;
    if(lastEdited != NULL && lastEdited->distanza == dist)
    {     

        ptr = lastEdited;
    }
    else{
        ptr = cercaStazione(dist);
    }
    int p = 0;
    if (ptr != NULL && autonomia > 0)
    {
        for (int i = 0; i < ptr->numeroAuto; i++)
        {
            if (ptr->parcoAuto[i] <= autonomia)
            {
                p = i;
                break;
            }
        }
        if (ptr->parcoAuto[p] == autonomia)
        {
            for (int i = p; i < ptr->numeroAuto - 1; i++)
            {
                ptr->parcoAuto[i] = ptr->parcoAuto[i + 1];
            }
            ptr->parcoAuto[ptr->numeroAuto - 1] = 0;
            ptr->numeroAuto--;
            printf("rottamata\n");
            return;
        }
    }
    printf("non rottamata\n");
    return;
}

void deleteFixup(autostrada nodo)
{
    autostrada w;
    if(nodo->color = RED || nodo->parent == NULL)
        nodo->color = BLACK;
    else if(nodo == nodo->parent->left){
        w = nodo->parent->right;
        if(w->color == RED){
            w->color = BLACK;
            nodo->parent->color = RED;
            leftRotate(nodo->parent);
            w = nodo->parent->right;
        }
        if(w->left->color == BLACK && w->right->color == BLACK)
        {
            w->color = RED;
            deleteFixup(nodo->parent);
        }
        else if(w->right->color == BLACK){
            w->left->color = BLACK;
            w->color = RED;
            rightRotate(w);
            w = nodo->parent->right;
        }
        w->color = nodo->parent->color;
        nodo->parent->color = BLACK;
        w->right->color = BLACK;
        leftRotate(nodo->parent);
    }
    else{
        w = nodo->parent->left;
        if(w->color == RED){
            w->color = BLACK;
            nodo->parent->color = RED;
            leftRotate(nodo->parent);
            w = nodo->parent->left;
        }
        if(w->left->color == BLACK && w->right->color == BLACK)
        {
            w->color = RED;
            deleteFixup(nodo->parent);
        }
        else if(w->left->color == BLACK){
            w->right->color = BLACK;
            w->color = RED;
            leftRotate(w);
            w = nodo->parent->right;
        }
        w->color = nodo->parent->color;
        nodo->parent->color = BLACK;
        w->left->color = BLACK;
        rightRotate(nodo->parent);
    }
}

void demolisciStazione(autostrada nodo)
{
    autostrada x,y;
    if(nodo->left == NULL || nodo->right == NULL) 
        y = nodo;
    else y = successor(nodo);
    if(y->left != NULL)
        x = y->left;
    else x = y->right;
    x->parent = y->parent;
    if(y->parent == NULL)
        root = x;
    else if(y = y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    nodo->distanza = y->distanza; //forse è il contrario??
    if(y->color == BLACK)
        deleteFixup(x);
}

/*void demolisciStazione(int dist){
    // printf("demolisco %d\n", dist);
    // cerco la stazione e salvo il ptr
    if (dist >= 0)
    {
        struct stazione *ptr;
        if(lastEdited != NULL && lastEdited->distanza == dist)
        {
            ptr = lastEdited;
        }
        else
        {
            ptr = cercaStazione(dist);
        }


        if (ptr != NULL)
        {
            // printf("aggiorno lista\n");
            // cancellazione testa
            if (ptr->prev == NULL)
            {
                root = ptr->next;
                if (ptr->next != NULL)
                    ptr->next->prev = NULL;
            }
            else if (ptr->next == NULL)
            {
                ptr->prev->next = NULL;
                ptr->prev = NULL;
            }
            else
            {
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
*/

void updateGraph(struct stazione * s)
{
    //crea il grafo delle stazioni raggiungibili da s in base ai vari parchi auto
    if(s != NULL)
    {
        printf("0 0\n");
    }
}

void pianificaPercorso(int start, int finish)
{
    struct stazione * stazioneA;
    struct stazione * next;
    if (start == finish)
    {
        printf("%d\n", start);
        return;
    }
    if(start==lastEdited->distanza)
    {
        stazioneA = lastEdited;
    }
    else stazioneA = cercaStazione(start);

    //verso dx
    if(start < finish){
        

    }
    //verso sx
    else{
        
    }
    
}

void execute(char *curr, int lineSize)
{
    char *cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        int array[MAX] = {0};
        int i = 0;
        char *car = strtok(NULL, " ");
        while (car != NULL)
        {
            array[i] = atoi(car);
            car = strtok(NULL, " ");
            i++;
        }
        creaStazione(atoi(dist), array, i);
        return;
    }
    else if (strcmp(cmd, "demolisci-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        demolisciStazione(atoi(dist));
        return;
    }
    else if (strcmp(cmd, "aggiungi-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        aggiungiAuto(atoi(dist), atoi(arg));
        return;
    }
    else if (strcmp(cmd, "rottama-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        rottamaAuto(atoi(dist), atoi(arg));
        return;
    }
    else if (strcmp(cmd, "pianifica-percorso") == 0)
    {
        char *start = strtok(NULL, " ");
        char *finish = strtok(NULL, " ");
        pianificaPercorso(atoi(start), atoi(finish));
        return;
    }
    /*else{
        printf("errore nell' esecuzione del comando: ");
        for(int i = 0; i < lineSize; i++){
            printf("%c", curr [i]);
        }
        printf("\n");
        return;
    }*/
}

int main(int argc, char **argv)
{
    char *curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    // scansione dell'input
    do
    {
        lineSize = getline(&curr, &len, stdin);
        if (lineSize > 0)
            execute(curr, lineSize);
    } while (!feof(stdin));

    /* prova di stampa in entrambi i versi
    struct stazione * last = root;
    while (root != NULL)
    {
        printf("%d :", root->distanza);
        int i = 0;
        while (i < MAX)
        {
            if (root->parcoAuto[i] != 0)
                printf("%d,", root->parcoAuto[i]);
            i++;
        }
        printf("-->");
        last = root;
        root = root->next;
    }
    printf("\n");
    while (last != NULL)
    {
        printf("%d :", last->distanza);
        int i = 0;
        // while (i< MAX){
        while (i < MAX)
        {
            if (last->parcoAuto[i] != 0)
                printf("%d,", last->parcoAuto[i]);
            i++;
        }
        printf("-->");
        last = last->prev;
    }
    printf("\n");
    */
    return 0;
}