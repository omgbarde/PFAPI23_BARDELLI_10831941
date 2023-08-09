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
    enum nodeColor color;
    struct stazione *parent;
    struct stazione *left;
    struct stazione *right;
};

typedef struct stazione * autostrada;

autostrada nil;
autostrada root;
autostrada lastEdited = NULL; // ricontrollare se viene usato in tutti i metodi

autostrada minimum(autostrada nodo) //ok
{
    while (nodo->left != nil)
    {
        nodo = nodo->left;
    }
    return nodo;
    
}

autostrada successor(autostrada nodo) //ok
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

autostrada cercaStazione(int dist, autostrada nodo) //ok
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

int insert(autostrada nodo, int autonomia) //ok
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

void aggiungiAuto(autostrada nodo, int autonomia) //ok
{
    if (insert(nodo, autonomia) == 0)
    {
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

void leftRotate(autostrada nodo) //ok
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

void rightRotate(autostrada nodo) //ok
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

/*void insertFixup(autostrada nodo) //ok
{
    if(nodo != nil){
        if(nodo == root){
            root->color = BLACK;
        }
        else
        {
            autostrada x = nodo->parent;
            if(x->color == RED){
                if(x == x->parent->left) 
                {
                    autostrada y = x->parent->right;
                    if(y->color == RED)         //caso 1
                    {
                        x->color = BLACK;
                        y->color = BLACK;           
                        x->parent->color = RED;
                        insertFixup(x->parent);
                        
                    }
                    else if(nodo == x->right)   //caso 2
                    {
                        nodo = x;
                        leftRotate(nodo);
                       
                    }
                    x = nodo->parent;           //caso 3 QUESTO ASSEGNAMENTO VA QUA O SOPRAAAAAAAA???
                    x->color = BLACK;   
                    x->parent->color = RED;
                    rightRotate(x->parent);
                }
                
            }
            else                                //scambiando dx e sx
            {
                if(x == x->parent->right){
                    autostrada y = x->parent->left;
                    if(y != nil && y->color == RED){
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        insertFixup(x->parent);
                    
                    }
                    else if(nodo == x->left)
                    {
                        nodo = x;
                        rightRotate(nodo);
                    }
                }
                x = nodo->parent;
                x->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
            }
        }
    }
}
*/

void insertFixup(autostrada nodo){
        while (nodo->parent != NULL && nodo->parent->color == RED) {
        if (nodo->parent == nodo->parent->parent->left) {
            autostrada nodoio = nodo->parent->parent->right;

            if (nodoio != NULL && nodoio->color == RED) {
                nodo->parent->color = BLACK;
                nodoio->color = BLACK;
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
            autostrada nodoio = nodo->parent->parent->left;

            if (nodoio != NULL && nodoio->color == RED) {
                nodo->parent->color = BLACK;
                nodoio->color = BLACK;
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

autostrada creaNodoNil() //ok
{
    autostrada temp = (autostrada)malloc(sizeof(struct stazione));
    temp->color = BLACK;
    return temp;
}

autostrada creaNodo(int dist) //ok
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

void creaStazione(int dist, int *cars, int amount) //ok
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
        int car = cars[i];
        while (check == 0 && i < amount)
        {
            check = insert(new, car);
            i++;
            car = cars[i];
        }

        insertFixup(new);
        lastEdited = new;
        printf("aggiunta\n");
        return;
    }
    printf("non aggiunta\n");
}

void rottamaAuto(autostrada nodo, int autonomia) //ok
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

void trapianta(autostrada u, autostrada v) {
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

/*void demolisciStanodoione(autostrada nodo)
{
    if(nodo != nil)
    {
        int originalColor = nodo->color;
        autostrada x = NULL,y = NULL;
        if(nodo->left == nil)
        {
            x = nodo->right;
            nodo = x;
        }
        else if (nodo->right == nil){
            x = nodo->left;
            nodo = x;
        }
        else
        {
            y = successor(nodo);
            originalColor = y->color;
            x = y->right;
            if(y == nodo->right || y == nodo->left)
            {
                x->parent = y;
            }
            else
            {
                y = y->right;
            }
            nodo = y;
            nodo->color = originalColor;
            if(originalColor == BLACK) deleteFixup(x);
            printf("demolita\n");
        }
    }
    else printf("non demolita\n");
}*/

/*void updateGraph(struct stanodoione * s)
{
    //crea il grafo delle stanodoioni raggiungibili da s in base ai vari parchi auto
    if(s != NULL)
    {
        printf("0 0\n");
    }
}*/

/*void pianificaPercorso(int start, int finish)
{
    struct stanodoione * stanodoioneA;
    struct stanodoione * next;
    if (start == finish)
    {
        printf("%d\n", start);
        return;
    }
    if(start==lastEdited->distanza)
    {
        stanodoioneA = lastEdited;
    }
    else stanodoioneA = cercaStanodoione(start, root);

    //verso dx
    if(start < finish){
        

    }
    //verso sx
    else{
        
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

void execute(char *curr, int lineSinodoe)
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
        //char *start = strtok(NULL, " ");
        //char *finish = strtok(NULL, " ");
        printf("ancora da implementare\n");
        //pianificaPercorso(atoi(start), atoi(finish));
        return;
    }
    /*else{
        printf("errore nell' esecunodoione del comando: ");
        for(int i = 0; i < lineSinodoe; i++){
            printf("%c", curr [i]);
        }
        printf("\n");
        return;
    }*/
}

void stampaInOrdine(autostrada nodo) {
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
    __ssize_t lineSinodoe = 0;

    //creanodoione albero vuoto
    nil = creaNodoNil();
    root = nil;

    // scansione dell'input
    do
    {
        lineSinodoe = getline(&curr, &len, stdin);
        if (lineSinodoe > 0)
            execute(curr, lineSinodoe);
    } while (!feof(stdin));
    
    printf("%d\n", root->distanza);
    stampaInOrdine(root);
    printf("\n");
    return 0;
}