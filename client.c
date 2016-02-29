//Sagar Saija

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

void filePut(char name[], char call[]);

int main()
{

  system("gcc Paillier.c -lgmp -o encOutput");
  printf("Start entering SQL commands:\n");
  while(1 == 1){
    char name[100];
    scanf("%s",name);

    if(strcmp(name,"exit") == 0)
        break;

    if (strcmp(name,"INSERT") == 0 || strcmp(name,"SELECT") == 0 || strcmp(name,"select") == 0 || strcmp(name,"insert") == 0) {
        char call[100];
        scanf("%[^\n]s",call);
        filePut(name, call);
        system("python client.py");
    }
    else{
        printf("invalid request.\n");
        break;
    }

  }

  return 0;
}

void filePut(char name[], char call[]){

    FILE *f = fopen("query.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "%s%s\n", name, call);

    fclose(f);

}
