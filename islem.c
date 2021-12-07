#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int i;
    int chld = fork();
    int result;
    if (chld == 0){
        i = execl(argv[1], argv[2], argv[3], (char*)0);
    }else{
        wait(&i);
        result = WEXITSTATUS(i);
    }
    return result;
}
