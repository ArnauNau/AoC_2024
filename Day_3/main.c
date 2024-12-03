//
// Created by Arnau Sanz on 3/12/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#define FILENAME_TEST "Day_3/test.txt"
#define FILENAME_REAL "Day_3/input.txt"

typedef enum {
    INSTR_MUL,
    INSTR_DO,
    INSTR_DONT,
    INSTR_INVALID
} InstructionType;

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

InstructionType get_instruction_type(const char *const buffer, const size_t size, const size_t current) {
    // Check for 'do()' instruction
    if (current + 3 < size &&
        buffer[current] == 'd' &&
        buffer[current + 1] == 'o' &&
        buffer[current + 2] == '(' &&
        buffer[current + 3] == ')') {
        return INSTR_DO;
        }

    // Check for 'don't()' instruction
    if (current + 6 < size &&
        buffer[current] == 'd' &&
        buffer[current + 1] == 'o' &&
        buffer[current + 2] == 'n' &&
        buffer[current + 3] == '\'' &&
        buffer[current + 4] == 't' &&
        buffer[current + 5] == '(' &&
        buffer[current + 6] == ')') {
        return INSTR_DONT;
        }

    // Check for 'mul(' instruction
    if (current + 3 < size &&
        buffer[current] == 'm' &&
        buffer[current + 1] == 'u' &&
        buffer[current + 2] == 'l' &&
        buffer[current + 3] == '(') {
        return INSTR_MUL;
        }

    // No valid instruction found at the current position
    return INSTR_INVALID;
}

long long sum_mul_instructions(const char *const buffer, const size_t size) {
    long long total_sum = 0;
    size_t i = 0;
    bool mul_enabled = true;

    while (i < size) {
        const InstructionType instr_type = get_instruction_type(buffer, size, i);

        switch (instr_type) {
            case INSTR_DO:
                mul_enabled = true;
                i += 4; //skip 'do()'
                break;

            case INSTR_DONT:
                mul_enabled = false;
                i += 7; //skip 'don't()'
                break;

            case INSTR_MUL:
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
                    break;
                }

                if (current >= size || buffer[current] != ',') {
                    i++;
                    break;
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
                    break;
                }

                if (current >= size || buffer[current] != ')') {
                    i++;
                    break;
                }
                current++;
                //valid mul(X,Y) found
                if (mul_enabled)
                    total_sum += (long long)(X * Y);
                //skip ')'
                i = current;
                break;

            case INSTR_INVALID:
            default: //INSTR_INVALID
                i++;
                break;
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
