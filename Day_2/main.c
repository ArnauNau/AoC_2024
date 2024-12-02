//
// Created by Arnau Sanz on 2/12/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME_TEST "Day_2/test.txt"
#define FILENAME_REAL "Day_2/input.txt"
#define MAX_LINE_LENGTH 256

typedef int Level;

typedef struct {
    Level *levels;
    int num_of_levels;
    bool safe;
} Report;

ssize_t ess_read_until(const int fd, char **const string_ptr, const char end) {
    ssize_t bytes_read = 0;
    ssize_t buffer_size = 8;
    char *string = nullptr;

    char c = '\0';
    int i = 0;
    while (c != end) {
        const ssize_t size = read(fd, &c, sizeof(char));
        if (c == '\0') c = '-';
        if (size == 0) {
            if (i == 0) {
                free(string);
                return -1;
            }
            break;
        }
        if (size < 0) {
            free(string);
            return -1;
        }
        bytes_read += size;
        if(c != end){
            if (i + 1 >= buffer_size || string == NULL) {
                buffer_size = buffer_size * 2;
                char *const temp = realloc(string, sizeof(char) * buffer_size);
                if (temp == NULL) {
                    free(string);
                    perror("realloc failed.");
                    return -1;
                }
                string = temp;
            }
            string[i++] = c;
        }
    }
    if (buffer_size > 0 && string != NULL) string[i] = '\0';
    *string_ptr = string;
    return bytes_read;
}
ssize_t ess_read_line(const int fd, char **const string_ptr) {
    return ess_read_until(fd, string_ptr, '\n');
}

void fill_report_from_line(Report *report, const char *const string) {
    int num_of_levels = 1;
    for (unsigned int i = 0; i < strlen(string); i++) {
        if (string[i] == ' ') num_of_levels++;
    }
    Level *levels = malloc(sizeof(Level) * num_of_levels);
    if (levels == NULL) {
        perror("malloc failed.");
        exit(EXIT_FAILURE);
    }
    int level = 0;
    char *endptr = nullptr;
    for (int i = 0; i < num_of_levels; i++) {
        levels[i] = (int)strtol(string + level, &endptr, 10);
        level = (int)(endptr - string + 1);
    }
    report->levels = levels;
    report->num_of_levels = num_of_levels;
}

void read_input_file(const char *const filename, Report **const reports, size_t *const size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    *size = 0;
    *reports = nullptr;

    char line[MAX_LINE_LENGTH];
    size_t capacity = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '\n') continue;
        if (*size >= capacity) {
            capacity = capacity == 0 ? 1 : capacity * 2;
            *reports = realloc(*reports, capacity * sizeof(Report));
            if (*reports == NULL) {
                perror("Error reallocating memory");
                exit(EXIT_FAILURE);
            }
        }
        fill_report_from_line(&(*reports)[*size], line);
        (*size)++;
    }
    fclose(file);
}

bool check_report_safety(Report *const report) {
    int increasing = 0;
    int decreasing = 0;
    for (int i = 0; i < report->num_of_levels - 1; i++) {
        if (report->levels[i] < report->levels[i + 1]) increasing++;
        if (report->levels[i] > report->levels[i + 1]) decreasing++;
    }
    if (increasing == report->num_of_levels - 1 || decreasing == report->num_of_levels - 1) {
        for (int i = 0; i < report->num_of_levels - 1; i++) {
            const int difference = abs(report->levels[i] - report->levels[i + 1]);
            if (difference < 1 || difference > 3) {
                report->safe = false;
                return false;
            }
        }
        report->safe = true;
    } else {
        report->safe = false;
    }

    return report->safe == true;
}

bool check_report_safety_with_problem_dampener(const Report *const report) {
    if (report->safe == true) {
        return true;
    }

    Level temp_levels[report->num_of_levels - 1];

    for (int i = 0; i < report->num_of_levels; i++) {
        int level_index = 0;
        for (int j = 0; j < report->num_of_levels; j++) {
            if (j != i) {
                temp_levels[level_index++] = report->levels[j];
            }
        }

        // Create a temporary sub-report
        Report sub_report;
        sub_report.levels = temp_levels;
        sub_report.num_of_levels = report->num_of_levels - 1;

        if (check_report_safety(&sub_report)) {
            return true;
        }
    }

    return false;
}

int main (void) {
    printf("\n****** Day 2 ******\n\n");

    Report *reports = nullptr;
    size_t size = 0;
    int safe_reports = 0;
    int safe_reports_with_problem_dampener = 0;

    read_input_file(FILENAME_REAL, &reports, &size);
    printf("Number of reports: %zu\n", size);

    for (size_t i = 0; i < size; i++) {
        //printf("\nReport %zu:", i);
        if (check_report_safety(&reports[i])) safe_reports++;
        if (check_report_safety_with_problem_dampener(&reports[i])) safe_reports_with_problem_dampener++;
        free(reports[i].levels);
    }
    free(reports);

    printf("\nSafe reports: %d\n", safe_reports);
    printf("Safe reports with problem dampener: %d\n", safe_reports_with_problem_dampener);

    exit(EXIT_SUCCESS);
}