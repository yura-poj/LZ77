#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <time.h>



#define WINDOW_SIZE 255

int array_size;

typedef struct Node {
    unsigned char shift;
    unsigned char size;
    char next;
} Node;

void encode(FILE *input, FILE *output) {
    Node node;
    char *buffer = calloc(sizeof(char), array_size);
    char *temp_str = calloc(sizeof(char), array_size);
    int buf_size = 1, cur = 0, window = 0, temp_size = 0;
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
        temp_str[temp_size] = ch;
        buffer[buf_size + temp_size++] = temp_str[temp_size];
        for (int i = 1; i < buf_size + 1 && i < WINDOW_SIZE + 1; i++) {
            if (!strncmp(&(buffer[buf_size - i]), temp_str, temp_size)) {
                b_match = temp_size;
                b_dist = i;
                flag = 1;
            }
        }

        if(temp_size == WINDOW_SIZE) flag = 0;

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
    if (flag) {
        buf_size += temp_size;
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
    char *buffer = calloc(sizeof(char), array_size);
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
        if(node.next != 0){
            buffer[buf_size++] = node.next;
            fputc(node.next, output);
        }
    }
    free(buffer);
}

long long size_file(char* input){
    struct stat st;
    stat(input, &st);

    return (long long) st.st_size;
}

long long difference(char* input, char* output){
    return size_file(input) - size_file(output);
}

void test(){
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "wb");
    FILE *output2 = fopen("output2.txt", "wb");

    if (input == NULL || output == NULL) {
        printf("Ошибка открытия файлов!\n");
    }
    array_size = (int)size_file("input.txt");
    clock_t start = clock();
    encode(input, output);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время выполнения программы: %.6f секунд\n", elapsed_time);

    array_size = array_size = (int)size_file("output.txt") * 100;
    fclose(output);
    fclose(input);

    FILE *input2 = fopen("output.txt", "rb");

    if (input2 == NULL) {
        printf("Ошибка открытия файла output.txt для чтения!\n");
    }

    start = clock();
    decode(input2, output2);
    end = clock();
    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время выполнения программы: %.6f секунд\n", elapsed_time);

    fclose(output2);
    input = fopen("input.txt", "r");
    output2 = fopen("output2.txt", "r");

    char ch1 = 1, ch2 = 1;
    while (ch1 != EOF || ch2 != EOF) {
        if (ch1 != ch2) {
            printf("%c %c - Error!\n", ch1, ch2);
        }
        ch1 = getc(input);
        ch2 = getc(output2);
    }
    printf("%lld\n", difference("input.txt", "output.txt"));

    fclose(input2);
    fclose(output2);
    printf("Success");
}

int main(int argc, char *argv[]) {
    test();
    return 0;
    if (argc != 4) {
        printf("usage: %s <encode/decode> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];

    FILE *input = fopen(input_file, "rb");
    FILE *output = fopen(output_file, "wb");

    if (input == NULL) {
        printf("Error of opening file!\n");
        return 1;
    }

    if (output == NULL) {
        printf("Error of opening output file!\n");
        fclose(input);
        return 1;
    }

    if (strcmp(mode, "encode") == 0) {
        encode(input, output);
    } else if (strcmp(mode, "decode") == 0) {
        decode(input, output);
    } else {
        printf("Unknown mode: %s. Use 'encode' or 'decode'.\n", mode);
        fclose(input);
        fclose(output);
        return 1;
    }

    fclose(input);
    fclose(output);

    return 0;
}