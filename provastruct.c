#include <stdio.h>
#define MAX 512

typedef struct{
    int autonomia;
}macchina;

typedef struct{
    macchina parcoAuto [MAX];
    int distanza;
}stazione;

int main(){
    printf("this is a tets\n");
    return 0;
}