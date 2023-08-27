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

struct tree
{
    node_t root;
    node_t nil;
};

typedef struct tree * tree_t;

node_t minimum(node_t node, tree_t T)
{
    while (node->left != T->nil)
    {
        node = node->left;
    }

    return node;
}

node_t maximum(node_t node, tree_t T)
{
    while (node->right != T->nil)
    {
        node = node->right;
    }

    return node;
}

node_t successor(node_t node, tree_t T)
{
    if (node != T->nil)
    {
        if (node->right != T->nil)
            return minimum(node->right, T);
        
        node_t y = node->parent;
        
        while (y != T->nil && node == y->right)
        {
            node = y;
            y = y->parent;
        }

        return y;
    }

    return T->nil;
}

node_t predecessor(node_t node, tree_t T)
{
    if (node != T->nil)
    {
        if (node->left != T->nil)
            return maximum(node->left, T);

        node_t y = node->parent;
        
        while (y != T->nil && node == y->left)
        {
            node = y;
            y = y->parent;
        }
        
        return y;
    }
    
    return T->nil;
}

node_t findStation(int dist, node_t node, tree_t T)
{
    if (node == T->nil || node->distance == dist)
        return node;
    else if (node->distance > dist)
        return findStation(dist, node->left,T);
    else
        return findStation(dist, node->right,T);

}

int insert(node_t node, int range, tree_t T) // poco efficiente
{
    int i = 0;
    if (node != T->nil && range > 0 && node->numCars < MAX)
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

void addCar(node_t node, int range, tree_t T)
{
    if (insert(node, range,T) == 0)
    {
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

void leftRotate(node_t node, tree_t T)
{
    node_t y = node->right;
    node->right = y->left;
    
    if (y->left != T->nil)
        y->left->parent = node;
    
    y->parent = node->parent;
    
    if (node->parent == NULL)
        T->root = y;
    else if (node == node->parent->left)
        node->parent->left = y;
    else
        node->parent->right = y;
    
    y->left = node;
    node->parent = y;

    return;
}

void rightRotate(node_t node, tree_t T)
{
    node_t y = node->left;
    node->left = y->right;
    if (y->right != T->nil)
        y->right->parent = node;
    y->parent = node->parent;
    if (node->parent == NULL)
        T->root = y;
    else if (node == node->parent->left)
        node->parent->left = y;
    else
        node->parent->right = y;
    y->right = node;
    node->parent = y;

    return;
}

void insertFixup(node_t node, tree_t T)
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
                    leftRotate(node,T);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent,T);
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
                    rightRotate(node,T);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent,T);
            }
        }
    }

    T->root->color = BLACK;
    
    return;
}

node_t createNilNode()
{
    node_t temp = (node_t)malloc(sizeof(struct station));
    temp->distance = -1;
    temp->color = BLACK;
    
    return temp;
}

node_t createNode(int dist, tree_t T)
{
    node_t temp = (node_t)malloc(sizeof(struct station));
    temp->distance = dist;
    temp->numCars = 0;
    temp->right = T->nil;
    temp->left = T->nil;
    temp->parent = NULL;
    temp->color = RED;
    temp->visited = false;

    return temp;
}

void createStation(int dist, int *cars, int amount, tree_t T)
{
    if (dist >= 0 && findStation(dist, T->root, T) == T->nil)
    {
        node_t y = NULL;
        node_t x = T->root;
        node_t new = createNode(dist,T);
        
        while (x != T->nil)
        {
            y = x;
            if (dist < x->distance)
                x = x->left;
            else
                x = x->right;
        }
        
        new->parent = y;
        
        if (y == NULL)
            T->root = new; // inserimento ad albero vuoto
        else if (dist < y->distance)
            y->left = new;
        else
            y->right = new;

        // inserimento auto
        int i = 0, check = 0;

        while (check == 0 && i <= amount)
        {
            int car = cars[i];
            check = insert(new, car,T);
            i++;
        }

        insertFixup(new,T);
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");

    return;
}

tree_t initTree(){
    tree_t temp = (tree_t)malloc(sizeof(struct tree));
    temp->nil = createNilNode();
    temp->root = temp->nil;

    return temp;
}

void deleteCar(node_t node, int range, tree_t T)
{
    int p = 0;
    if (node != T->nil && node != NULL && range > 0)
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

void transplant(node_t u, node_t v, tree_t T)
{
    if (u->parent == NULL)
    {
        T->root = v;
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

void deleteFixup(node_t node, tree_t T)
{
    while (node != T->root && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            node_t w = node->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                node->parent->color = RED;
                leftRotate(node->parent,T);
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
                    rightRotate(w,T);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(node->parent,T);
                node = T->root;
            }
        }
        else
        {
            node_t w = node->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                node->parent->color = RED;
                rightRotate(node->parent,T);
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
                    leftRotate(w,T);
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(node->parent,T);
                node = T->root;
            }
        }
    }
    node->color = BLACK;

    return;
}

void deleteStation(node_t node, tree_t T)
{
    if (node != T->nil && node != NULL)
    {
        node_t x = NULL;
        node_t y = node;
        enum nodeColor originalColor = y->color;

        if (node->left == T->nil)
        {
            x = node->right;
            transplant(node, node->right,T);
        }
        else if (node->right == T->nil)
        {
            x = node->left;
            transplant(node, node->left,T);
        }
        else
        {
            y = successor(node,T);
            originalColor = y->color;
            x = y->right;

            if (y->parent == node)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->right,T);
                y->right = node->right;
                y->right->parent = y;
            }

            transplant(node, y,T);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }

        if (originalColor == BLACK)
        {
            deleteFixup(x,T);
        }

        free(node);
        printf("demolita\n");
    }
    else
        printf("non demolita\n");
    
    return;
}

void destroySubtree(node_t node, tree_t T)
{
    if (node->left != T->nil && node->left != NULL) // prima sottoalbero sx
    {
        destroySubtree(node->left,T);
    }

    if (node->right != T->nil && node->right != NULL) // poi sottoalbero dx
    {
        destroySubtree(node->right,T);
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

void resetVisited(node_t node, tree_t T){
    if (node == T->nil) {
        return;
    }
    resetVisited(node->left,T);
    node->visited = false;
    node->step = NULL;
    resetVisited(node->right,T);

}

void findAdjacents(node_t node, node_t start, list * adjList, tree_t T)
{
    node_t ptr = node;

    while(ptr->distance < start->distance){
        ptr = successor(ptr,T);

        if(isReachable(ptr,node) && ptr->visited == false){
            ptr->visited = true;
            ptr->step = node;
            enqueue(adjList,ptr);
        }
    }

    return;
}

void findPath(node_t A, node_t B, list *candidate, tree_t T)
{

    if (A->distance == B->distance)         // stazione uguale
    {
        addList(candidate, A);

        return;
    }
    else if (A->distance < B->distance)     // verso dx
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
                    ptr = predecessor(ptr,T);
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
    else                                    // verso sx
    {
        node_t start = A;
        node_t finish = B;
        list adjList = NULL;
        
        finish->visited = true;

        findAdjacents(finish,start,&adjList,T);
        while (!isEmpty(adjList))
        {
            findAdjacents(adjList->node,start,&adjList,T);
            (void)dequeue(&adjList);
        }

        node_t ptr = start;
        while(ptr->step != NULL){
            enqueue(candidate,ptr);
            ptr = ptr->step;
        }
        enqueue(candidate,ptr);

        if ((*candidate)->tail->node != finish){
            flush(candidate);
        }

        resetVisited(T->root,T);


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

void execute(char *curr, int lineSize, tree_t T)
{
    char *cmd = strtok(curr, " ");

    if (strcmp(cmd, "aggiungi-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        int count = atoi(strtok(NULL, " "));
        int array[MAX] = {0};
        char *car = strtok(NULL, " ");

        for (int i = 0; i < count; i++)
        {
            array[i] = atoi(car);
            car = strtok(NULL, " ");
        }
        
        createStation(atoi(dist), array, count, T);
    }
    else if (strcmp(cmd, "demolisci-stazione") == 0)
    {
        char *dist = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), T->root, T);
        
        deleteStation(target,T);
    }
    else if (strcmp(cmd, "aggiungi-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), T->root, T);
        
        addCar(target, atoi(arg),T);
    }
    else if (strcmp(cmd, "rottama-auto") == 0)
    {
        char *dist = strtok(NULL, " ");
        char *arg = strtok(NULL, " ");
        node_t target = findStation(atoi(dist), T->root, T);
        
        deleteCar(target, atoi(arg),T);
    }
    else if (strcmp(cmd, "pianifica-percorso") == 0)
    {
        int start = atoi(strtok(NULL, " "));
        int finish = atoi(strtok(NULL, " "));
        
        node_t A, B;
        list candidate = NULL;

        A = findStation(start, T->root, T);
        B = findStation(finish, T->root, T);

        findPath(A, B, &candidate, T);
        printPath(candidate);
    }
    else printf("errore nell'esecuzione del comando %s\n", cmd);
    
    return;
}

int main(int argc, char **argv)
{
    char *curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;

    // inizializzo l'albero autostrada
    tree_t autostrada = initTree();
    autostrada->nil = createNilNode();
    autostrada->root = autostrada->nil;

    // scansione dell'input
    do
    {
        lineSize = getline(&curr, &len, stdin);
        if (lineSize > 0)
            execute(curr, lineSize, autostrada);

    } while (!feof(stdin));

    // libera la memoria
    destroySubtree(autostrada->root, autostrada);
    free(autostrada->nil);

    return 0;
}