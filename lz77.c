#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define WINDOW_SIZE 6

typedef struct Node{
    char shift;
    char size;
    char next;
};

void encode(char * input){
    char* buffer = calloc(sizeof (char ), 1000);
    int buf_size = 1;
    int cur = 0;
    buffer[cur] = input[cur];
    cur++;

    printf("%d %d %c -", 0,0,input[cur-1]);
    int window = 0;
    char* temp_str = calloc(sizeof (char ), 1000);
    int temp_size = 0;
    int b_match = 0;
    int b_dist = 0;
    int flag = 0;
    while (input[cur] != 0){
        flag = 0;
        temp_str[temp_size] = input[cur++];
        buffer[buf_size + temp_size++] = temp_str[temp_size];
        for(int i = 1; i < buf_size+1 && i < WINDOW_SIZE; i++){
            if(! strncmp(&(buffer[buf_size - i]), temp_str, temp_size)){
                b_match = temp_size;
                b_dist = i;
                flag = 1;
            }
        }
        if(!flag){
            buf_size += temp_size;
            printf("%d %d %c - ", b_dist, b_match,temp_str[temp_size-1]);
            temp_size = 0;
            b_match = 0;
            b_dist = 0;
        }

    }
}

int main() {
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
    char* input = malloc(sizeof (char ) * 1000);
    scanf("%s", input);
    encode(input);

    return 0;
}