#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define WINDOW_SIZE 255
#define ARRAY_SIZE 10000

typedef struct Node {
    unsigned char shift;
    unsigned char size;
    char next;
} Node;

void encode(FILE *input, FILE *output) {
    Node node;
    char *buffer = calloc(sizeof(char), ARRAY_SIZE);
    char *temp_str = calloc(sizeof(char), ARRAY_SIZE);
    int buf_size = 1, cur = 0, window = 0, temp_size = 0;
    int b_match = 0, b_dist = 0, flag = 0;
    char ch;

    ch = getc(input);
    buffer[cur++] = ch;

    printf("%d %d %c -", 0, 0, ch);
    node.shift = 0;
    node.size = 0;
    node.next = ch;
    fwrite(&node, sizeof(Node), 1, output);

    ch = getc(input);
    while (ch != EOF) {
        flag = 0;
        temp_str[temp_size] = ch;
        buffer[buf_size + temp_size++] = temp_str[temp_size];
        for (int i = 1; i < buf_size + 1 && i < WINDOW_SIZE + 1; i++) {
            if (!strncmp(&(buffer[buf_size - i]), temp_str, temp_size)) {
                b_match = temp_size;
                b_dist = i;
                flag = 1;
            }
        }
        if (!flag) {
            buf_size += temp_size;
            printf("%d %d %c - ", b_dist, b_match, temp_str[temp_size - 1]);
            node.shift = b_dist;
            node.size = b_match;
            node.next = temp_str[temp_size - 1];
            fwrite(&node, sizeof(Node), 1, output);
            temp_size = 0;
            b_match = 0;
            b_dist = 0;
        }
        ch = getc(input);
    }
    if (flag) {
        buf_size += temp_size;
        printf("%d %d %c - ", b_dist, b_match, 0);
        node.shift = b_dist;
        node.size = b_match;
        node.next = 0;
        fwrite(&node, sizeof(Node), 1, output);
    }

    free(buffer);
    free(temp_str);
}

void decode(FILE *input, FILE *output) {
    int x, j;
    Node node;
    char *buffer = calloc(sizeof(char), ARRAY_SIZE);
    int buf_size = 0;

    while ((x = fread(&node, sizeof(Node), 1, input)) > 0) {
        if (node.shift == 0) {
            buffer[buf_size++] = node.next;
            printf("%c", node.next);
            fputc(node.next, output);
            continue;
        }
        j = buf_size - node.shift;
        for (int i = 0; i < node.size; i++) {
            buffer[buf_size++] = buffer[j + i];
            printf("%c", buffer[buf_size - 1]);
            fputc(buffer[buf_size - 1], output);
        }
        if(node.next != 0){
            buffer[buf_size++] = node.next;
            printf("%c", node.next);
            fputc(node.next, output);
        }
    }
    free(buffer);
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "wb");
    FILE *output2 = fopen("output2.txt", "wb");

    if (input == NULL || output == NULL) {
        printf("Ошибка открытия файлов!\n");
        return 1;
    }

    encode(input, output);
    fclose(input);
    fclose(output);

    FILE *input2 = fopen("output.txt", "rb");

    if (input2 == NULL) {
        printf("Ошибка открытия файла output.txt для чтения!\n");
        return 1;
    }

    printf("\n-------------------------\n");
    decode(input2, output2);

    fclose(input2);
    fclose(output2);

    return 0;
}
