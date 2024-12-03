//
// Created by Arnau Sanz on 3/12/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#define FILENAME_TEST "Day_3/test.txt"
#define FILENAME_REAL "Day_3/input.txt"

char* read_file(const char *const filename, size_t *const size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: fseek failed on file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    const ssize_t file_size = ftell(file);
    if (file_size < 0) {
        fprintf(stderr, "Error: ftell failed on file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error: fseek to start failed on file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed for buffer\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    const size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error: fread failed. Expected %ld bytes, got %zu bytes\n", file_size, bytes_read);
        free(buffer);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    buffer[file_size] = '\0';
    *size = bytes_read;

    fclose(file);
    return buffer;
}

long long sum_mul_instructions(const char *const buffer, const size_t size) {
    long long total_sum = 0;
    size_t i = 0;

    while (i + 3 < size) { //enough characters for 'mul('
        if (buffer[i] == 'm' && buffer[i+1] == 'u' && buffer[i+2] == 'l' && buffer[i+3] == '(') {
            size_t current = i + 4; //position after 'mul('

            int X = 0;
            int digits_X = 0;
            while (current < size && isdigit(buffer[current]) && digits_X < 3) {
                X = X * 10 + (buffer[current] - '0');
                current++;
                digits_X++;
            }
            if (digits_X == 0) {
                i++;
                continue;
            }

            if (current >= size || buffer[current] != ',') {
                i++;
                continue;
            }
            current++;

            int Y = 0;
            int digits_Y = 0;
            while (current < size && isdigit(buffer[current]) && digits_Y < 3) {
                Y = Y * 10 + (buffer[current] - '0');
                current++;
                digits_Y++;
            }
            if (digits_Y == 0) {
                i++;
                continue;
            }

            if (current >= size || buffer[current] != ')') {
                i++;
                continue;
            }
            current++;
            //valid mul(X,Y) found
            total_sum += (long long)(X * Y);
            //skip ')'
            i = current;
        } else {
            i++;
        }
    }
    return total_sum;
}

int main(const int argc, char *argv[]) {
    const char *filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = FILENAME_REAL;
    }

    size_t size;
    char *buffer = read_file(filename, &size);

    const long long total_sum = sum_mul_instructions(buffer, size);

    printf("Total sum of all valid mul(X,Y) instructions: %lld\n", total_sum);

    free(buffer);
    exit(EXIT_SUCCESS);
}
