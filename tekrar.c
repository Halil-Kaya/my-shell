#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int amount = atoi(argv[argc - 1]);
    int i = 0;
    for(i = 0 ; i < amount; i++){
        int i = 1; 
        while (i < argc - 1){
            printf("%s", argv[i]);
            printf(" ");
            i++;
        }
        printf("\n");
    }
    return 0;
}
