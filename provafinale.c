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
autostrada lastEdited = NULL; // ricontrollare se viene usato in tutti i metodi

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

autostrada cercaStazione(int dist, autostrada nodo) 
{
    // printf("cerco %d...\n",dist);
    if(lastEdited != NULL && lastEdited->distanza == dist)
    {
         return lastEdited;
    }
    else
    {
        if(nodo == NULL || nodo->distanza == dist)
            return nodo;
        if (nodo->distanza < dist)
            if(nodo->left != NULL) return cercaStazione(dist, nodo->left);
            else return NULL;
        else
            if(nodo->right != NULL) return cercaStazione(dist, nodo->right);
            else return NULL;
    }
}

int insert(autostrada nodo, int autonomia)
{
    int i = 0;
    if (nodo != NULL && autonomia > 0 && nodo->numeroAuto < MAX)
    {
        for (i = 0; i < nodo->numeroAuto; i++)
        {
            if (nodo->parcoAuto[i] < autonomia)
            {
                break;
            }
        }

        for (int j = nodo->numeroAuto; j > i; j--)
        {
            nodo->parcoAuto[j] = nodo->parcoAuto[j - 1];
        }
        nodo->parcoAuto[i] = autonomia;
        nodo->numeroAuto++;
        return 0;
    }
    else return 1;
}

void aggiungiAuto(autostrada nodo, int autonomia) 
{
    if (insert(nodo, autonomia) == 0)
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
    if(root == NULL){
        root = nodo;
        root->color = 2;
    }
    else if(root == nodo){
        root->color = 2;
    }
    else
    {
        x = nodo->parent;
        if(x->color == 1){
            if(x->parent != NULL && x->parent->left != NULL && x == x->parent->left){
                if (x->parent->right != NULL) y = x->parent->right;
                if(y->color == 1){
                    x->color = 2;
                    y->color = 2;
                    if(x->parent != NULL){
                        x->parent->color = 1,
                        insertFixup(x->parent);
                    }
                }
                else if(x->right != NULL && nodo == x->right)
                {
                    nodo = x;
                    leftRotate(nodo);
                    if(nodo->parent != NULL) x = nodo->parent;
                }
            }
            x->color = 2;
            if(x->parent != NULL){
                x->parent->color = 1;
                rightRotate(x->parent);
            }
        }
        else{
            if(x->parent != NULL && x->parent->right != NULL && x == x->parent->right){
                if(x->parent->left != NULL) y = x->parent->left;
                if(y->color == 1){
                    x->color = 2;
                    y->color = 2;
                    if(x->parent != NULL){
                        x->parent->color = 1;
                        insertFixup(x->parent);
                    }
                }
                else if(x->left != NULL && nodo == x->left)
                {
                    nodo = x;
                    rightRotate(nodo);
                    if(nodo->parent != NULL) x = nodo->parent;
                }
            }
            x->color = 2;
            if(x->parent != NULL){
                x->parent->color = 1;
                leftRotate(x->parent);
            }
        }
    }
}

void creaStazione(int dist, int *cars, int amount) //evitare duplicati
{
    if(dist >= 0){
        printf("creo stazione %d\n", dist);
        autostrada y = NULL;
        autostrada x = root;
        autostrada new = malloc(sizeof(struct stazione));

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
        printf("fixup...\n");
        insertFixup(new);
        lastEdited = new;
        // inserimento auto
        int i = 0, check = 0;
        int car = cars[i];
        while (check == 0 && i < amount)
        {
            check = insert(lastEdited, car);
            i++;
            car = cars[i];
        }
        printf("aggiunta\n");
        return;
    }
}

void rottamaAuto(autostrada nodo, int autonomia)
{
    int p = 0;
    if (nodo != NULL && autonomia > 0)
    {
        for (int i = 0; i < nodo->numeroAuto; i++)
        {
            if (nodo->parcoAuto[i] <= autonomia)
            {
                p = i;
                break;
            }
        }
        if (nodo->parcoAuto[p] == autonomia)
        {
            for (int i = p; i < nodo->numeroAuto - 1; i++)
            {
                nodo->parcoAuto[i] = nodo->parcoAuto[i + 1];
            }
            nodo->parcoAuto[nodo->numeroAuto - 1] = 0;
            nodo->numeroAuto--;
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
{   if(nodo != NULL){
        printf("demolisco %d", nodo->distanza);
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
        free(nodo);
        printf("demolita\n");
    }
    else printf("non demolita\n");

}

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
    else stazioneA = cercaStazione(start, root);

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
        autostrada target = cercaStazione(atoi(dist), root);
        demolisciStazione(target);
        return;
    }
    else if (strcmp(cmd, "aggiungi-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        autostrada target = cercaStazione(atoi(dist), root);
        aggiungiAuto(target, atoi(arg));
        return;
    }
    else if (strcmp(cmd, "rottama-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        autostrada target = cercaStazione(atoi(dist), root);
        rottamaAuto(target, atoi(arg));
        return;
    }
    else if (strcmp(cmd, "pianifica-percorso") == 0)
    {
        char *start = strtok(NULL, " ");
        char *finish = strtok(NULL, " ");
        printf("ancora da implementare\n");
        //pianificaPercorso(atoi(start), atoi(finish));
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