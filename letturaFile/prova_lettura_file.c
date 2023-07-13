#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
    char* curr = NULL;
    size_t len = 0;
    __ssize_t lineSize = 0;
    do{
        lineSize = getline(&curr,&len,stdin);
        for(int i = 0; i < lineSize; i++){
            printf("%c",curr[i]);
        }
    }while(lineSize!=-1);
    printf("\n");
    return 0;
}