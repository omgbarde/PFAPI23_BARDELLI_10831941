#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 512

enum nodeColor {
  RED,
  BLACK
};

struct stazione{
    int distanza;
    int parcoAuto[MAX];
    int numeroAuto;
    enum nodeColor color;
    struct stazione *parent;
    struct stazione *left;
    struct stazione *right;
};

typedef struct stazione * autostrada;

struct elemLista{
    autostrada node;
    struct elemLista * next;
};

typedef struct elemLista * lista;

autostrada nil;
autostrada root;
autostrada lastEdited = NULL; // ricontrollare se viene usato in tutti i metodi

autostrada minimum(autostrada nodo) 
{
    while (nodo->left != nil)
    {
        nodo = nodo->left;
    }
    return nodo;
    
}

autostrada maximum(autostrada nodo) 
{
    while (nodo->right != nil)
    {
        nodo = nodo->right;
    }
    return nodo;
}

autostrada successor(autostrada nodo) 
{
    if(nodo != nil)
    {
        if(nodo->right != nil)
            return minimum(nodo->right);
        autostrada y = nodo->parent;
        while(y != nil && nodo == y->right)
        {
            nodo = y;
            y = y->parent;
        }
        return y;
    }
    return nil;
}

autostrada predecessor(autostrada nodo) 
{
    if(nodo != nil)
    {
        if(nodo->left != nil)
            return maximum(nodo->left);
        autostrada y = nodo->parent;
        while(y != nil && nodo == y->left)
        {
            nodo = y;
            y = y->parent;
        }
        return y;
    }
    return nil;
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
        if(nodo == nil || nodo->distanza == dist)
            return nodo;
        if (nodo->distanza > dist)
            return cercaStazione(dist, nodo->left);
        else
            return cercaStazione(dist, nodo->right);
    }
}

int insert(autostrada nodo, int autonomia) 
{
    int i = 0;
    if (nodo != nil && autonomia > 0 && nodo->numeroAuto < MAX)
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
    //if(nodo != nil && nodo != NULL){
        autostrada y = nodo->right;
        nodo->right = y->left;
        if(y->left != nil)
            y->left->parent = nodo;
        y->parent = nodo->parent;
        if(nodo->parent == NULL)
            root = y;
        else if(nodo == nodo->parent->left) nodo->parent->left = y;
        else nodo->parent->right = y;
        y->left = nodo;
        nodo->parent = y;
    //}

}

void rightRotate(autostrada nodo) 
{
    //if(nodo != nil && nodo != NULL){
        autostrada y = nodo->left;
        nodo->left = y->right;
        if(y->right != nil)
            y->right->parent = nodo;
        y->parent = nodo->parent;
        if(nodo->parent == NULL)
            root = y;
        else if(nodo == nodo->parent->left) nodo->parent->left = y;
        else nodo->parent->right = y;
        y->right = nodo;
        nodo->parent = y;
    //}
}

void insertFixup(autostrada nodo) 
{
        while (nodo->parent != NULL && nodo->parent->color == RED) {
        if (nodo->parent == nodo->parent->parent->left) {
            autostrada zio = nodo->parent->parent->right;

            if (zio != NULL && zio->color == RED) {
                nodo->parent->color = BLACK;
                zio->color = BLACK;
                nodo->parent->parent->color = RED;
                nodo = nodo->parent->parent;
            } else {
                if (nodo == nodo->parent->right) {
                    nodo = nodo->parent;
                    leftRotate(nodo);
                }
                nodo->parent->color = BLACK;
                nodo->parent->parent->color = RED;
                rightRotate(nodo->parent->parent);
            }
        } else {
            autostrada zio = nodo->parent->parent->left;

            if (zio != NULL && zio->color == RED) {
                nodo->parent->color = BLACK;
                zio->color = BLACK;
                nodo->parent->parent->color = RED;
                nodo = nodo->parent->parent;
            } else {
                if (nodo == nodo->parent->left) {
                    nodo = nodo->parent;
                    rightRotate(nodo);
                }
                nodo->parent->color = BLACK;
                nodo->parent->parent->color = RED;
                leftRotate(nodo->parent->parent);
            }
        }
    }

    root->color = BLACK;
}

autostrada creaNodoNil() 
{
    autostrada temp = (autostrada)malloc(sizeof(struct stazione));
    temp->distanza = -1;
    temp->color = BLACK;
    return temp;
}

autostrada creaNodo(int dist) 
{
    autostrada temp = (autostrada)malloc(sizeof(struct stazione));
    temp->distanza = dist;
    temp->numeroAuto = 0;
    temp->right = nil;
    temp->left = nil;
    temp->parent = NULL;
    temp->color = RED;
    return temp;
}

void creaStazione(int dist, int *cars, int amount) 
{
    if(dist >= 0 && cercaStazione(dist,root) == nil){
        autostrada y = NULL;
        autostrada x = root;
        autostrada new = creaNodo(dist);
        while(x != nil)
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

        // inserimento auto
        int i = 0, check = 0;
        //int car = cars[i];
        while (check == 0 && i <= amount)
        {
            int car = cars[i];
            check = insert(new, car);
            i++;
        }

        insertFixup(new);
        lastEdited = new;
        printf("aggiunta\n");
        return;
    }
    printf("non aggiunta\n");
}

void rottamaAuto(autostrada nodo, int autonomia) 
{
    int p = 0;
    if (nodo != nil && nodo != NULL && autonomia > 0)
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

void trapianta(autostrada u, autostrada v) 
{
    if (u->parent == NULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void deleteFixup(autostrada nodo) 
{
    //if (nodo != nil ){
        while (nodo != root && nodo->color == BLACK) {
            if (nodo == nodo->parent->left) {
                autostrada w = nodo->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    nodo->parent->color = RED;
                    leftRotate(nodo->parent);
                    w = nodo->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    nodo = nodo->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = nodo->parent->right;
                    }
                    w->color = nodo->parent->color;
                    nodo->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(nodo->parent);
                    nodo = root;
                }
            } else {
                autostrada w = nodo->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    nodo->parent->color = RED;
                    rightRotate(nodo->parent);
                    w = nodo->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    nodo = nodo->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = nodo->parent->left;
                    }
                    w->color = nodo->parent->color;
                    nodo->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(nodo->parent);
                    nodo = root;
                }
            }
        }
        nodo->color = BLACK;
    //}
}

void demolisciStazione(autostrada nodo) 
{   
    if(nodo != nil && nodo != NULL){
        autostrada x = NULL;
        autostrada y = nodo;
        enum nodeColor originalColor = y->color;
        if (nodo->left == nil) {
            x = nodo->right;
            trapianta(nodo, nodo->right);
        } else if (nodo->right == nil) {
            x = nodo->left;
            trapianta(nodo, nodo->left);
        } else {
            y = successor(nodo);
            originalColor = y->color;
            x = y->right;

            if (y->parent == nodo) {
                x->parent = y;
            } else {
                trapianta(y, y->right);
                y->right = nodo->right;
                y->right->parent = y;
            }

            trapianta(nodo, y);
            y->left = nodo->left;
            y->left->parent = y;
            y->color = nodo->color;
        }

        if (originalColor == BLACK) {
            deleteFixup(x);
        }

        if(lastEdited == nodo) lastEdited = x;
        free(nodo);
        printf("demolita\n");
    }
    else printf("non demolita\n");
}

bool isReachable(autostrada a, autostrada b)
{
    if(b->distanza > a->distanza){
        int max_autonomia = a->parcoAuto[0];
        int distanza = b->distanza - a->distanza;
        if (max_autonomia >= distanza && distanza > 0) return true;
        else return false;
    }
    else{
        int max_autonomia = b->parcoAuto[0];
        int distanza = a->distanza - b->distanza;
        if (max_autonomia >= distanza && distanza > 0) return true;
        else return false;
    }
}

bool isEmpty(lista l){
    if(l == NULL)return true;
    else return false;
}

void enqueue(lista *l, autostrada nodo)
{
    lista punt, cursore;
    punt = malloc(sizeof(struct elemLista));
    punt->next = NULL;
    punt->node = nodo;
    if(isEmpty(*l)){
        *l = punt;
    }
    else{
        cursore = *l;
        while(cursore->next != NULL){
            cursore = cursore->next;
        }
        cursore->next = punt;
    }
}

autostrada dequeue(lista *l)
{
    autostrada punt;
    if(!isEmpty(*l)){
        punt = (*l)->node;
        if((*l)->next != NULL) *l = (*l)->next;
        else *l = NULL;
        return punt;
    }
    return NULL;
}

/*void pianificaPercorso(int start, int finish)
{
    //start e finish sono sicuramente presenti
    if (start == finish)
    {
        printf("%d\n", start);
        return;
    }

    autostrada A, B;
    autostrada max = maximum(root);
    //autostrada min = minimum(root);        

    if(lastEdited->distanza == start) A = lastEdited;
    else A = cercaStazione(start,root);
    
    if(lastEdited->distanza == finish) B = lastEdited;
    else B = cercaStazione(finish,root);

    //verso dx
    if(start < finish)
    {
        if(isReachable(A, B))                   //raggiungibile senza tappe intermedie
        { 
            printf("%d %d\n", A->distanza, B->distanza);
        }
        else if(A != max && successor(A) != B)              //controllo tappe intermedie
        {   
            autostrada next = successor(A);
            int n = 0;
            bool notFound = true;                        
            autostrada current = A;
            while(notFound){
                n++;
                autostrada intermedie[n];
                while(next != B && !isReachable(current,B)){
                    for(int i = 0; i < n; i++){
                        if(isReachable(current,next)){
                            intermedie[i] = next;
                        }
                        if(next != max && next != B){
                            current = next;
                            next = successor(next);
                        }
                        else break;
                    }
                    current = next;
                }
                if(current == B) break;
                else
                {
                    printf("%d ", start);
                    for (int i = 0; i < n; i++)
                    {
                        printf("%d ", intermedie[i]->distanza);
                    }
                    printf("%d\n", finish);
                    notFound = false;
                }
                return;
            }
        }
        printf("nessun percorso\n");
    }
    //verso sx
    else
    {
        printf("%d %d\n", A->distanza, B->distanza);
    }

}*/

/*void print(autostrada nodo)
{
    autostrada temp = nodo;
    if (temp != NULL)
    {
        print(temp->left);
        printf(" %d ", temp->distanza);
        print(temp->right);
    }
}*/

void pianificaPercorso(int start, int finish)
{
    //start e finish sono sicuramente presenti
    if (start == finish)
    {
        printf("%d\n", start);
        return;
    }

    autostrada A, B;
    if(lastEdited!= NULL && lastEdited->distanza == start) A = lastEdited;
    else A = cercaStazione(start,root);
    
    if(lastEdited!= NULL && lastEdited->distanza == finish) B = lastEdited;
    else B = cercaStazione(finish,root);
    autostrada max = maximum(root);
    //autostrada min = minimum(root);
    
    autostrada next = NULL;
    lista queue = NULL;
    enqueue(&queue, A);

    /*while(!isEmpty(queue)){
        autostrada curr = dequeue(&queue);
        printf("%d ",curr->distanza);
        autostrada temp = NULL;
        if(curr != max){
            temp = successor(curr); //crea segm. fault
        }
        else break;

        if (curr == B){
            printf("percorso trovato\n");
            return;
        }
        if(temp != NULL && temp != nil && isReachable(curr,temp)) next = temp;
        else next = NULL;

        while(next != nil && next != NULL){
            enqueue(&queue, next);
            if(next != B || next != max) temp = successor(next); //viene passato un nodo null ma perchèèèèèèèèèè
            else temp = NULL;
            if(temp != NULL &&temp != nil && isReachable(curr,temp)) next = temp;
            else next = NULL;
        }

    }*/
    printf("nessun percorso\n");
}

void execute(char *curr, int lineSize)
{
    char *cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *count = strtok(NULL, " ");
        int array[MAX] = {0};
        char *car = strtok(NULL, " ");
        int i = 0;
        while (car != NULL)
        {
            array[i] = atoi(car);
            car = strtok(NULL, " ");
            i++;
        }
        creaStazione(atoi(dist), array, atoi(count));
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

void stampaInOrdine(autostrada nodo) 
{
    if (nodo == nil) {
        return;
    }

    stampaInOrdine(nodo->left);
    printf("%d (%s) ", nodo->distanza, (nodo->color == RED) ? "ROSSO" : "NERO");
    stampaInOrdine(nodo->right);
}

int main(int argc, char **argv) 
{
    char *curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

   
    nil = creaNodoNil();
    root = nil;

    // scansione dell'input
    do
    {
        lineSize = getline(&curr, &len, stdin);
        if (lineSize > 0)
            execute(curr, lineSize);
    } while (!feof(stdin));

    //libera la memoriaaaa

    return 0;
}