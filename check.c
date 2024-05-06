#include "stdio.h"
#include "string.h"
int main(){
    char a[] = "abcdef";
    char b[] = "ab";
    int x = strncmp(&(a[0]), &(b[0]), 2);
    printf("%d", x);
}