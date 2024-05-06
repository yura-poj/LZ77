#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Node{
    char shift;
    char size;
    char next;
}Node;

int main() {
    FILE *output = fopen("output.txt", "r");
    Node node;
    int x = fread(&node, sizeof(Node), 8, output);
    printf("%d %d %c\n", node.size, node.shift, node.next);
    return 0;
}