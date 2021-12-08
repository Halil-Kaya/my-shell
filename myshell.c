#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void takeInput(char *value);
void runCommand(char **args, char **commands, int index);
void split(char *value, char **parsed, char *parser);
int isEmpty(char *value);
int runArgs(char **args);
int checkCommand(char **args, char **commands);
int checkShellState(char *command, char **args);
int checkTekrarState(char **args);
int checkIslemState(char **args);
int isNumber(char *value);

//parametre olarak gonderilen degerin bos olup olmadigini kontrol eder
int isEmpty(char *value){
    while (*value != '\0'){
        if (*value != ' ') return 0;
        value++;
    }
    return 1;
}

//kullanicidan input alir
void takeInput(char *value){
    printf("\nmyshell>>");
    fgets(value, 1000, stdin);
    while (*value != '\0'){
        if (*value == '\n'){
            *value = '\0'; 
            break;
        }
        value++; 
    }
}

//komutlari calistirir
int runArgs(char **args){
    int pid = fork();
    int result = 0;
    int i;
    if (pid == 0){
        if(strcmp("ls", args[0]) == 0){
            i = execv("/bin/ls", args);
        }if (strcmp("cat", args[0]) == 0){
            printf("cat:%s\n",args[1]);
            exit(0);
        }else if(strcmp("bash", args[0]) == 0){
            i = execv("/bin/bash", args);
        }else{
            i = execv(args[0], args);
        }
        if (i < 0){
            printf("yanlis bir komut girdiniz\n");
            exit(0);
        }
    }
    else{
        wait(&i); 
        result = WEXITSTATUS(i);
    }
    return result;
}

//kisinin girdigi komutlari kontrol eder
int checkCommand(char **args, char **commands){
    int isThereCommand = 0;
    int i = 0;
    while (commands[i] != NULL){
        if (strcmp(args[0], commands[i]) == 0){
            isThereCommand = 1;
            break;
        }
        i++;
    }
    if (isThereCommand){
        if (!checkShellState(args[0], args)) runCommand(args, commands, i);
        else return -1;
    }
    if(isThereCommand != 1) return -1;
    return 0;
}

//komutlarin calistirildigi yer
void runCommand(char **args, char **commands, int index){
    if (strcmp("islem", commands[index]) == 0){
        int result = runArgs(args);
        if (strcmp("topla", args[1]) == 0){
            printf("\n%s + %s = %d", args[2], args[3], result);
        }
        else if (strcmp("cikar", args[1]) == 0){
            printf("\n%s - %s = %d", args[2], args[3], result);
        }
    }
    else if (strcmp("clear", commands[index]) == 0){
        printf("\033[H\033[J");
    }
    else if (strcmp("exit", commands[index]) == 0) {
        exit(0);
    }
    else{
        runArgs(args);
    }
}

//gonderilen degerin split edilmesini sagliyor
void split(char *value, char **parsed, char *parser){
    int i = 0;
    while ((parsed[i] = strsep(&value, parser)) != NULL){
        if (isEmpty(parsed[i])) continue;
        i++;
    }
}

//ana komutlar kontrol ediliyor
int checkShellState(char *command, char **args){
    if (strcmp("islem", command) == 0){
        return checkIslemState(args);
    }
    else if (strcmp("tekrar", command) == 0){
        return checkTekrarState(args);
    }
    return 0;
}

//tekrar komutuna uyuyor mu diye kontrol ediliyor
int checkTekrarState(char **args){
    int i = 0;
    while (args[i] != NULL) i++;
    if (i < 3) return -1;
    if (atoi(args[i - 1]) < 0 || !isNumber(args[i - 1])) return -1;
    return 0;
}

//islem komutuna uyuyor mu diye kontrol ediliyor
int checkIslemState(char **args){
    int i = 0;
    while (args[i] != NULL) i++;
    if (i != 4) return -1;
    if (!(strcmp("topla", args[1]) == 0 || strcmp("cikar", args[1]) == 0)) return -1;
    if (!isNumber(args[2]) && !isNumber(args[3])) return -1;
    return 0;
}

//gonderilen deger sayi mi diye kontrol ediliyor
int isNumber(char *value){
    int i = 0;
    while (*value != '\0'){
        if (i == 0){
            if (value[0] == '-'){
                value++;
            }
            i++;
        }
        if (*value < '0' || *value > '9'){
            return 0;
        }
        value++;
    }
    return 1;
}

int main(){
    char *commands[8] = {"islem", "tekrar", "clear","cat","bash", "exit","ls", NULL};
    char *args[100] = {NULL};
    char inputValue[100] = {'\0'};
    char *processes[100] = {NULL};
    int i;
    while (1){
        takeInput(inputValue);//kullanicidan input aliyor
        //alinan input kontrol ediliyor
        if (isEmpty(inputValue)) continue;
        //birden fazla komut olabilir '|' isareti ile komutlar bolunuyor
        split(inputValue, processes, "|");
        i = 0;
        while (processes[i] != NULL){
            split(processes[i], args, " ");
            if (checkCommand(args, commands)){
                printf("yanlis bir komut girdiniz!\n");
            }
            i++;
        }
    }
    return 0;
}