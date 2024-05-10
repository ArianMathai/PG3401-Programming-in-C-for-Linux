#include <stdio.h>

int main(int argc, char* argv[]){
   char hei[2];
    char *name = "rolf";
   int b = 5;

    int a = 0;
    while (a < b){
        printf("a = %d\n", a);
        a++;
    }

   int c = 0;
    while (c < b){
        printf("c = %d\n", c);
        c++;
    }
    int f = 0;
    while(f<b){
        printf("f = %d\n", f);
        // hei
        f++;
    }

    return 0;
}
