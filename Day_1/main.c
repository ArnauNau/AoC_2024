//
// Created by Arnau Sanz on 2/12/24.
//

#include <stdio.h>
#include <stdlib.h>

typedef signed int LocationID;

#define FILENAME_TEST "../Day_1/test.txt"
#define FILENAME_REAL "../Day_1/input.txt"
#define MAX_LINE_LENGTH 100

void read_input_file(const char *const filename, LocationID **const col1, LocationID **const col2, size_t *const size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *size = 0;
    *col1 = nullptr;
    *col2 = nullptr;

    char line[MAX_LINE_LENGTH];
    size_t capacity = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (*size >= capacity) {
            capacity = capacity == 0 ? 1 : capacity * 2;
            *col1 = realloc(*col1, capacity * sizeof(LocationID));
            *col2 = realloc(*col2, capacity * sizeof(LocationID));
            if (*col1 == NULL || *col2 == NULL) {
                perror("Error reallocating memory");
                exit(EXIT_FAILURE);
            }
        }
        char *endptr;
        (*col1)[*size] = (int)strtol(line, &endptr, 10);
        (*col2)[*size] = (int)strtol(endptr, nullptr, 10);
        (*size)++;
    }

    fclose(file);
}

int compare (const void *restrict a, const void *restrict b) {
    return *(const LocationID *)a - *(const LocationID *)b;
}

void calculate_total_distance_between_pairs (LocationID *const left_list, LocationID *const right_list, const size_t size) {
    qsort(left_list, size, sizeof(LocationID), compare);
    qsort(right_list, size, sizeof(LocationID), compare);

    int sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += abs(left_list[i] - right_list[i]);
    }
    printf("Total distance: %d\n", sum);
}

void calculate_similarity_score (const LocationID *const left_list, const LocationID *const right_list, const size_t size) {
    int sum = 0;

    for (size_t i = 0; i < size; i++) {
        int repetitions = 0;
        for (size_t j = 0; j < size; j++) {
            if (left_list[i] == right_list[j]) {
                repetitions++;
            }
        }
        sum += left_list[i] * repetitions;
    }

    printf("Similarity score: %d\n", sum);
}

int main(void) {
    printf("\n****** Day 1 ******\n\n");

    LocationID *left_list = nullptr;
    LocationID *right_list = nullptr;
    size_t size = 0;

    read_input_file(FILENAME_REAL, &left_list, &right_list, &size);

    calculate_total_distance_between_pairs(left_list, right_list, size);
    calculate_similarity_score(left_list, right_list, size);

    free(left_list);
    free(right_list);

    return EXIT_SUCCESS;
}