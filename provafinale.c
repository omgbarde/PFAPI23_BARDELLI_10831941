#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 512

enum nodeColor
{
    RED,
    BLACK
};

struct station
{
    int distance;
    int cars[MAX];
    int numCars;
    enum nodeColor color;
    struct station *parent;
    struct station *left;
    struct station *right;
    bool visited;
    struct station *step;
    
};

typedef struct station *node_t;

struct listElement
{
    node_t node;
    struct listElement *next;
    struct listElement *prec;
    struct listElement *tail;
};

typedef struct listElement *list;

// SISTEMARE VARIABILI GLOBALI
node_t nil;
node_t root;
node_t lastEdited = NULL; // ricontrollare se viene usato in tutti i metodi

node_t minimum(node_t node)
{
    while (node->left != nil)
    {
        node = node->left;
    }

    return node;
}

node_t maximum(node_t node)
{
    while (node->right != nil)
    {
        node = node->right;
    }

    return node;
}

node_t successor(node_t node)
{
    if (node != nil)
    {
        if (node->right != nil)
            return minimum(node->right);
        
        node_t y = node->parent;
        
        while (y != nil && node == y->right)
        {
            node = y;
            y = y->parent;
        }

        return y;
    }

    return nil;
}

node_t predecessor(node_t node)
{
    if (node != nil)
    {
        if (node->left != nil)
            return maximum(node->left);

        node_t y = node->parent;
        
        while (y != nil && node == y->left)
        {
            node = y;
            y = y->parent;
        }
        
        return y;
    }
    
    return nil;
}

node_t findStation(int dist, node_t node)
{
    if (lastEdited != NULL && lastEdited->distance == dist)
        return lastEdited;

    else
    {
        if (node == nil || node->distance == dist)
            return node;
        if (node->distance > dist)
            return findStation(dist, node->left);
        else
            return findStation(dist, node->right);
    }
}

int insert(node_t node, int range)
{
    int i = 0;
    if (node != nil && range > 0 && node->numCars < MAX)
    {
        for (i = 0; i < node->numCars; i++)
        {
            if (node->cars[i] < range)
            {
                break;
            }
        }

        for (int j = node->numCars; j > i; j--)
        {
            node->cars[j] = node->cars[j - 1];
        }

        node->cars[i] = range;
        node->numCars++;
        
        return 0;
    }
    else
        return 1;
}

void addCar(node_t node, int range)
{
    if (insert(node, range) == 0)
    {
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

void leftRotate(node_t node)
{
    node_t y = node->right;
    node->right = y->left;
    
    if (y->left != nil)
        y->left->parent = node;
    
    y->parent = node->parent;
    
    if (node->parent == NULL)
        root = y;
    else if (node == node->parent->left)
        node->parent->left = y;
    else
        node->parent->right = y;
    
    y->left = node;
    node->parent = y;

    return;
}

void rightRotate(node_t node)
{
    node_t y = node->left;
    node->left = y->right;
    if (y->right != nil)
        y->right->parent = node;
    y->parent = node->parent;
    if (node->parent == NULL)
        root = y;
    else if (node == node->parent->left)
        node->parent->left = y;
    else
        node->parent->right = y;
    y->right = node;
    node->parent = y;

    return;
}

void insertFixup(node_t node)
{
    while (node->parent != NULL && node->parent->color == RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            node_t uncle = node->parent->parent->right;

            if (uncle != NULL && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    leftRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent);
            }
        }
        else
        {
            node_t uncle = node->parent->parent->left;

            if (uncle != NULL && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rightRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent);
            }
        }
    }

    root->color = BLACK;
    
    return;
}

node_t createNilNode()
{
    node_t temp = (node_t)malloc(sizeof(struct station));
    temp->distance = -1;
    temp->color = BLACK;
    
    return temp;
}

node_t createNode(int dist)
{
    node_t temp = (node_t)malloc(sizeof(struct station));
    temp->distance = dist;
    temp->numCars = 0;
    temp->right = nil;
    temp->left = nil;
    temp->parent = NULL;
    temp->color = RED;
    temp->visited = false;

    return temp;
}

void createStation(int dist, int *cars, int amount)
{
    if (dist >= 0 && findStation(dist, root) == nil)
    {
        node_t y = NULL;
        node_t x = root;
        node_t new = createNode(dist);
        
        while (x != nil)
        {
            y = x;
            if (dist < x->distance)
                x = x->left;
            else
                x = x->right;
        }
        
        new->parent = y;
        
        if (y == NULL)
            root = new; // inserimento ad albero vuoto
        else if (dist < y->distance)
            y->left = new;
        else
            y->right = new;

        // inserimento auto
        int i = 0, check = 0;

        while (check == 0 && i <= amount)
        {
            int car = cars[i];
            check = insert(new, car);
            i++;
        }

        insertFixup(new);
        lastEdited = new;
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

void deleteCar(node_t node, int range)
{
    int p = 0;
    if (node != nil && node != NULL && range > 0)
    {
        for (int i = 0; i < node->numCars; i++)
        {
            if (node->cars[i] <= range)
            {
                p = i;
                break;
            }
        }

        if (node->cars[p] == range)
        {
            for (int i = p; i < node->numCars - 1; i++)
            {
                node->cars[i] = node->cars[i + 1];
            }

            node->cars[node->numCars - 1] = 0;
            node->numCars--;
            printf("rottamata\n");
            return;
        }
    }
    printf("non rottamata\n");
    
    return;
}

void transplant(node_t u, node_t v)
{
    if (u->parent == NULL)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void deleteFixup(node_t node)
{
    while (node != root && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            node_t w = node->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                node->parent->color = RED;
                leftRotate(node->parent);
                w = node->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                node = node->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(node->parent);
                node = root;
            }
        }
        else
        {
            node_t w = node->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                node->parent->color = RED;
                rightRotate(node->parent);
                w = node->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                node = node->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;

    return;
}

void deleteStation(node_t node)
{
    if (node != nil && node != NULL)
    {
        node_t x = NULL;
        node_t y = node;
        enum nodeColor originalColor = y->color;

        if (node->left == nil)
        {
            x = node->right;
            transplant(node, node->right);
        }
        else if (node->right == nil)
        {
            x = node->left;
            transplant(node, node->left);
        }
        else
        {
            y = successor(node);
            originalColor = y->color;
            x = y->right;

            if (y->parent == node)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }

            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }

        if (originalColor == BLACK)
        {
            deleteFixup(x);
        }

        if (lastEdited == node)
            lastEdited = x;

        free(node);
        printf("demolita\n");
    }
    else
        printf("non demolita\n");
    
    return;
}

void destroySubtree(node_t node)
{
    if (node->left != nil && node->left != NULL) // prima sottoalbero sx
    {
        destroySubtree(node->left);
    }

    if (node->right != nil && node->right != NULL) // poi sottoalbero dx
    {
        destroySubtree(node->right);
    }

    free(node); // poi se stesso
    
    return;
}

bool isReachable(node_t a, node_t b)
{
    int max_autonomia = a->cars[0];
    int distance = 0;

    if (b->distance > a->distance)
        distance = b->distance - a->distance;
    else
        distance = a->distance - b->distance;

    if (max_autonomia >= distance && distance > 0)
        return true;
    else
        return false;
}

bool isEmpty(list l)
{
    if (l == NULL)
        return true;
    else
        return false;
}

void addList(list *l, node_t node)
{
    list punt;
    punt = malloc(sizeof(struct listElement));
    punt->node = node;
    punt->next = *l;
    if (!isEmpty(*l))
        punt->tail = (*l)->tail;
    else
        punt->tail = punt;
    *l = punt;

    return;
}

void enqueue(list *l, node_t node)
{
    list punt, cursor;
    punt = malloc(sizeof(struct listElement));
    punt->next = NULL;
    punt->node = node;

    if (isEmpty(*l))
    {
        *l = punt;
    }
    else
    {
        cursor = *l;
        while (cursor->next != NULL)
        {
            cursor = cursor->next;
        }
        cursor->next = punt;
    }

    (*l)->tail = punt;

    return;
}

node_t dequeue(list *l)
{
    node_t node;
    list punt;

    if (!isEmpty(*l))
    {
        node = (*l)->node;
        punt = *l;
        *l = (*l)->next;
        free(punt);
    
        return node;
    }

    return NULL;
}

void flush(list *l)
{
    list punt;

    while (!isEmpty(*l))
    {
        punt = *l;
        *l = (*l)->next;
        free(punt);
    }

    return;
}

void resetVisited(node_t node){
    if (node == nil) {
        return;
    }
    resetVisited(node->left);
    node->visited = false;
    node->step = NULL;
    resetVisited(node->right);

}

void findAdjacents(node_t node, node_t start, list * adjList)
{
    node_t ptr = node;
    while(ptr->distance < start->distance){
        /*if (ptr != start)*/ ptr = successor(ptr);
        /*else break;*/

        if(isReachable(ptr,node) && ptr->visited == false){
            ptr->visited = true;
            ptr->step = node;
            enqueue(adjList,ptr);
        }
    }
    return;
}

void findPath(node_t A, node_t B, list *candidate)
{

    if (A->distance == B->distance)         //stazione uguale
    {
        addList(candidate, A);

        return;
    }
    if (A->distance < B->distance)          //verso dx
    { 
        node_t start = A;
        node_t finish = B;
        node_t best = B;
        addList(candidate, finish);

        while (start != finish)
        {
            node_t ptr = finish;
            while (ptr->distance >= start->distance)
            {
                if (isReachable(ptr, finish))
                {
                    best = ptr;
                }
                if (ptr != start)
                    ptr = predecessor(ptr);
                else
                    break;
            }

            if (best != finish)
            {
                finish = best;
                addList(candidate, best);
            }
            else
                break;
        }
        if ((*candidate)->node != A){
            flush(candidate);
        }
    }
    /*else                                  //verso sx best
    { 
        node_t start = A;
        node_t finish = B;
        node_t best = B;
        enqueue(candidate,start);
        steps++;

        while(start != finish){    
            node_t ptr = start;                                                                 
            while(ptr->distance <= start->distance){
                if(isReachable(start, ptr)){
                    best = ptr;
                }
                if(ptr != finish) ptr = predecessor(ptr);
                else break;
            }

            if(best != start){
                start = best;
                enqueue(candidate,best);
                steps++;
            }
            else break;
        }
        if((*candidate)->tail->node != B){
            flush(candidate);
            steps = 0;
        }
        
    }*/
    else                                    // verso sx reverse bfs
    {
        node_t start = A;
        node_t finish = B;
        list adjList = NULL;
        
        finish->visited = true;

        findAdjacents(finish,start,&adjList);
        while (!isEmpty(adjList))
        {
            findAdjacents(adjList->node,start,&adjList);
            (void)dequeue(&adjList);
            //se gia visitata ma step vecchio piu lontano dall origine rispetto a quello corrente, e la distanza dallo start >= del nuovo lo sostistuisco
        }

        node_t ptr = start;
        while(ptr->step != NULL){
            enqueue(candidate,ptr);
            ptr = ptr->step;
        }
        enqueue(candidate,ptr);

        //printf("%d\n",(*candidate)->tail->node->distance);

        if ((*candidate)->tail->node != finish){
            flush(candidate);
        }

        resetVisited(root);


    }
    return;
}

void printPath(list candidate)
{
    if (isEmpty(candidate))
        printf("nessun percorso\n");
    else
    {
        node_t ptr = dequeue(&candidate);
        printf("%d", ptr->distance);
        while (!isEmpty(candidate))
        {
            printf(" ");
            ptr = dequeue(&candidate);
            printf("%d", ptr->distance);
        }
        printf("\n");
    }

    return;
}

void execute(char *curr, int lineSize)
{
    char *cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        int count = atoi(strtok(NULL, " "));
        int array[MAX] = {0};
        char *car = strtok(NULL, " ");

        for (int i = 0; i < count; i++) // poco efficiente
        {
            array[i] = atoi(car);
            car = strtok(NULL, " ");
        }
        
        createStation(atoi(dist), array, count);
    }
    else if (strcmp(cmd, "demolisci-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), root);
        
        deleteStation(target);
    }
    else if (strcmp(cmd, "aggiungi-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), root);
        
        addCar(target, atoi(arg));
    }
    else if (strcmp(cmd, "rottama-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), root);
        
        deleteCar(target, atoi(arg));
    }
    else if (strcmp(cmd, "pianifica-percorso") == 0)
    {
        int start = atoi(strtok(NULL, " "));
        int finish = atoi(strtok(NULL, " "));
        
        node_t A, B;

        if (lastEdited != NULL && lastEdited->distance == start)
            A = lastEdited;
        else
            A = findStation(start, root);

        if (lastEdited != NULL && lastEdited->distance == finish)
            B = lastEdited;
        else
            B = findStation(finish, root);
        
        list candidate = NULL;

        findPath(A, B, &candidate);
        printPath(candidate);
    }
    else{
        printf("errore nell'esecuzione del comando %s\n", cmd);
    }
    
    return;
}

int main(int argc, char **argv)
{
    char *curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    // inizializzo l'albero
    nil = createNilNode();
    root = nil;

    // scansione dell'input
    do
    {
        lineSize = getline(&curr, &len, stdin);
        if (lineSize > 0)
            execute(curr, lineSize);

    } while (!feof(stdin));

    // libera la memoria
    destroySubtree(root);
    free(nil);

    return 0;
}