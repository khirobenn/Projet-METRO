#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE * f = fopen("test.csv", "r");
    char str[30];
    float a;
    float b;
    
    while(fscanf(f, "%f;%f;%[^\n]s", &a, &b, str) != EOF){
        printf("%f\t%f\t%s\n", a, b, str);
    }
    fclose(f);
    return 0;

}