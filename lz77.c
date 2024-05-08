#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>




#define WINDOW_SIZE 255

int array_size;

typedef struct Node {
    unsigned char shift;
    unsigned char size;
    char next;
} Node;

void encode(const char *input_str, const char *output_str) {
    FILE *input = fopen(input_str, "r");
    FILE *output = fopen(output_str, "w");
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

    fclose(input);
    fclose(output);
    free(buffer);
    free(temp_str);
}

void decode(const char *input_str, const char *output_str) {
    FILE *input = fopen(input_str, "r");
    FILE *output = fopen(output_str, "w");
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
    fclose(input);
    fclose(output);
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
    char input[] = "input.txt";
    char output[] = "output.txt";
    char output2[] = "output2.txt";

    array_size = (int)size_file(input) * 100;
    clock_t start = clock();
    encode(input, output);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время выполнения программы: %.6f секунд\n", elapsed_time);

    array_size = (int)size_file(output) * 100;

    start = clock();
    decode(output, output2);
    end = clock();
    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время выполнения программы: %.6f секунд\n", elapsed_time);

    char ch1 = 1, ch2 = 1;

    FILE *input_file = fopen(input,"r");
    FILE *output2_file = fopen(output2,"r");
    while (ch1 != EOF || ch2 != EOF) {
        if (ch1 != ch2) {
            printf("%c %c - Error!\n", ch1, ch2);
        }
        ch1 = getc(input_file);
        ch2 = getc(output2_file);
    }
    fclose(input_file);
    fclose(output2_file);
    printf("%lld\n", difference(input, output));

    printf("Success");
}

void listDirectory(const char *path, const char* out_path,const char *mode) {
    char new_path[100];
    char new_out_path[100];
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Не удалось открыть каталог: %s\n", path);
        return;
    }
    int status = mkdir(out_path, 0700);

    if (status != 0) {
        perror("Error creating directory");
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) {

            strcpy(new_path, path); //make path to each file
            strcat(new_path, "/");
            strcat(new_path, entry->d_name);
            strcpy(new_out_path, out_path); //make path to each out file
            strcat(new_out_path, "/");
            strcat(new_out_path, entry->d_name);

            array_size = (int)size_file(new_path) * 100;

            if (strcmp(mode, "encode") == 0) {
                clock_t start = clock();
                encode(new_path, new_out_path);
                clock_t end = clock();
                double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Source file: %s\n", entry->d_name);
                printf("Время выполнения программы: %.6f секунд\n", elapsed_time);
                printf("Разница сжатого файла от исходного в байтах %lld\n", difference(new_path, new_out_path));

            } else if (strcmp(mode, "decode") == 0) {
                clock_t start = clock();
                decode(new_path, new_out_path);
                clock_t end = clock();
                double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Source file: %s\n", entry->d_name);
                printf("Время выполнения программы: %.6f секунд\n", elapsed_time);
            } else {
                printf("Unknown mode: %s. Use 'encode' or 'decode'.\n", mode);
            }
        }
    }

    closedir(dir);
}


int main(int argc, char *argv[]) {
//    test();
//    return 0;

    if (argc != 5) {
        printf("usage: %s <encode/decode> <input_file> <output_file> <folder/file>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];
    char *type_file = argv[4];

    if (strcmp(type_file, "folder") == 0) {
        listDirectory(input_file,output_file, mode);
    } else if (strcmp(type_file, "file") == 0){
        array_size = (int)size_file(input_file) * 100;
        if (strcmp(mode, "encode") == 0) {
            clock_t start = clock();
            encode(input_file, output_file);
            clock_t end = clock();
            double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Время выполнения программы: %.6f секунд\n", elapsed_time);
            printf("Разница сжатого файла от исходного в байтах %lld\n", difference(input_file, output_file));

        } else if (strcmp(mode, "decode") == 0) {
            clock_t start = clock();
            decode(input_file, output_file);
            clock_t end = clock();
            double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Время выполнения программы: %.6f секунд\n", elapsed_time);
        } else {
            printf("Unknown mode: %s. Use 'encode' or 'decode'.\n", mode);
        }
    } else {
        printf("Unknown type: %s. Use 'folder' or 'file", type_file);
    }
    return 0;
}