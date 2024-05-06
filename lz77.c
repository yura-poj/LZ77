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
    char *temp_str = calloc(sizeof(char), WINDOW_SIZE);
    int buf_size = 1, cur = 0, temp_size = 0;
    int b_match = 0, b_dist = 0, flag = 0;
    char ch;

    ch = getc(input);
    buffer[cur++] = ch;

    node.shift = 0;
    node.size = 0;
    node.next = ch;
    fwrite(&node, sizeof(Node), 1, output);

    ch = getc(input);
    while (ch != EOF) {
        flag = 0;
        temp_str[temp_size++] = ch;
        for (int i = 1; i <= buf_size && i <= WINDOW_SIZE; i++) {
            if (!strncmp(&(buffer[buf_size - i]), temp_str, temp_size)) {
                b_match = temp_size;
                b_dist = i;
                flag = 1;
            }
        }

        if (temp_size == 15) {
            flag = 0;  // Останавливаем поиск
        }

        if (!flag) {
            buf_size += temp_size;
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

    if (temp_size > 0) {
        buf_size += temp_size;
        node.shift = b_dist;
        node.size = b_match;
        node.next = temp_str[temp_size - 1];
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
            fputc(node.next, output);
            continue;
        }
        j = buf_size - node.shift;
        for (int i = 0; i < node.size; i++) {
            buffer[buf_size++] = buffer[j + i];
            fputc(buffer[buf_size - 1], output);
        }
        buffer[buf_size++] = node.next;
        fputc(node.next, output);
    }
    free(buffer);
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "wb");
    FILE *output2 = fopen("output2.txt", "wb");

    if (input == NULL || output == NULL) {
        return 1;
    }

    encode(input, output);
    fclose(input);
    fclose(output);

    FILE *input2 = fopen("output.txt", "rb");

    if (input2 == NULL) {
        return 1;
    }

    decode(input2, output2);

    fclose(input2);
    fclose(output2);

    return 0;
}
